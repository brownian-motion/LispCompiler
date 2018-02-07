#pragma once

#include "tokenizer.h"

int addToTokenBuffer(tokenizer * t, char c){
	if(t->tokenBufferEnd < MAX_TOKEN_SIZE || c=='\0') {
		t->tokenBuffer[t->tokenBufferEnd++] = c;
		return 1; //indicate success
	} else {
		return 0; //indicate failure
	}
}

int hasNextToken(tokenizer * t){
	skipWhitespaceAndCommentsUntilNextToken(t);
	return fPeekChar(t->file) != EOF;
}

token_t * getNextToken(tokenizer * t){
	char c; //to hold the current character of interest
	int colNumberAtStartOfToken = 0; //to keep track of the position where the token_t started
	int state = STATE_EMPTY;

	while( (c = fPeekChar(t->file)) != EOF){
		switch(state){
			case STATE_EMPTY:
				assert(t->tokenBufferEnd == 0);
				if(is_whitespace(c)){
					skipWhitespaceUntilNextToken(t);
					continue; // don't add to the buffer
				} else if(is_digit(c)){
					state = STATE_NUMBER;
				} else if(c == '('){ // TODO: change to general brace token_t state
					state = STATE_LIST_START;
				} else if(c == ')'){
					state = STATE_LIST_END;
				} else if(is_id_start(c)){
					state = STATE_ID; // TODO: add operator state
				} else if(c == ';'){
					state = STATE_COMMENT;
					continue; //avoid adding to the buffer
				} else {
					state = STATE_ERROR;
				}
				addToTokenBuffer(t,c);
				t->colNumber++;
				colNumberAtStartOfToken = t->colNumber; //save this so that we can print the correct spot later
				fConsumeChar(t->file);
				break;
			case STATE_NUMBER:
				if(is_digit(c)){
					addToTokenBuffer(t,c);
					fConsumeChar(t->file);
					t->colNumber++;
				} else {
					addToTokenBuffer(t,'\0');
					t->tokenBufferEnd = 0;
					return allocAndInitializeToken(state, t->lineNumber, colNumberAtStartOfToken, t->tokenBuffer);
				}
				break;
			default:
				fputs("Encountered an undefined state. Interpreting as an erroneous token_t.",stderr);
				state = STATE_ERROR;
				//fall through to STATE_ERROR without waiting for the next loop because we encountered an error
			case STATE_ERROR:
				if(is_whitespace(c)){
					addToTokenBuffer(t,'\0');
					t->tokenBufferEnd = 0;
					fprintf(stderr,"Erroneous token_t encountered: %s\n",t->tokenBuffer);
					return allocAndInitializeToken(state, t->lineNumber, colNumberAtStartOfToken, t->tokenBuffer);
				} else {
					addToTokenBuffer(t,c);
					fConsumeChar(t->file);
					t->colNumber++;
				}
				break;
			case STATE_LIST_START:
			case STATE_LIST_END: //The process is identical for both cases
				//Expected to only be one char.
				//This char would have already been added by the empty state,
				//so just end the token_t here.
				addToTokenBuffer(t,'\0');
				t->tokenBufferEnd = 0;
				return allocAndInitializeToken(state, t->lineNumber, colNumberAtStartOfToken, t->tokenBuffer);
				break;
			case STATE_ID:
				if(is_id(c)){
					addToTokenBuffer(t,c);
					fConsumeChar(t->file); //don't nest this call, because c is already equal to the value this would return
				} else {
					addToTokenBuffer(t,'\0');
					t->tokenBufferEnd = 0;
					return allocAndInitializeToken(state, t->lineNumber, colNumberAtStartOfToken, t->tokenBuffer);
				}
				break;
			case STATE_COMMENT:
				assert(t->tokenBufferEnd == 0);
				skipCommentUntilStartOfLine(t);
				break;
		}
	}
	//Handle the final token_t when EOF is encountered
	if(state != STATE_EMPTY && state != STATE_COMMENT){
		addToTokenBuffer(t,'\0');
		t->tokenBufferEnd = 0;
		return allocAndInitializeToken(state, t->lineNumber, colNumberAtStartOfToken, t->tokenBuffer);
	} else {
		return allocAndInitializeToken(TYPE_TOKEN_EOF,-1,-1,"EOF");
	}
}

void skipWhitespaceAndCommentsUntilNextToken(tokenizer * t){
	char c;
	while( (c = fPeekChar(t->file)) != EOF) {
		if(is_whitespace(c)){
			skipWhitespaceUntilNextToken(t);
		} else if(c == ';'){
			skipCommentUntilStartOfLine(t);
		} else { //reached something that isn't a comment
			return;
		}
	}
}

void skipWhitespaceUntilNextToken(tokenizer * t){
	char c = fPeekChar(t->file);
	while(c!= EOF && is_whitespace(c)){
		if(is_whitespace(c)){
			if(c == '\n'){
				t->lineNumber++;
				t->colNumber = 0; //will increment to 1 on the next read
			} else {
				t->colNumber++;
			}
			fConsumeChar(t->file); //consume this one whitespace character
		} else {
			break;
		}
		c = fPeekChar(t->file);
	}
}

void skipCommentUntilStartOfLine(tokenizer * t){
	char c = fConsumeChar(t->file);
	while(c != EOF && c != '\n'){
		c = fConsumeChar(t->file);
	}
	t->lineNumber++;
	t->colNumber = 0;
}

char fPeekChar(FILE* f){
	char out = fgetc(f);
	ungetc(out,f);
	return out;
}

char fConsumeChar(FILE* f){
	return fgetc(f);
}

char peekChar(){
	return fPeekChar(stdin);
}

char consumeChar(){
	return fConsumeChar(stdin);
}