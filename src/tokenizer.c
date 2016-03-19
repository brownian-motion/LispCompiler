#pragma once

#include "tokenizer.h"

int main(int argc, char* argv[]){
	if(argc == 2 && (strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"/?") == 0) ){
		puts("Usage: tokenizer < inputfile.jlisp");
		return 0;
	}
	//assume that the stdin now refers to a valid input file

	char *tokenBuffer = (char *) malloc(MAX_TOKEN_SIZE+1);
	if(tokenBuffer == NULL){
		fputs("Could not allocate heap memory.", stderr);
		return MEMORY_ALLOCATION_ERROR;
	}

	int tokenBufferEnd = 0;

	char c;

	int state = 0;

	while( (c = getchar()) != EOF){
		//printf("STATE %d CHAR '%c'\n",state, c);
		switch(state){
			case STATE_EMPTY:
				assert(tokenBufferEnd == 0);
				if(isspace(c)){
					continue;
				} else if(isdigit(c)){
					state = STATE_NUMBER;
				} else if(c == '('){
					state = STATE_LIST_START;
				} else if(c == ')'){
					state = STATE_LIST_END;
				} else if(isalpha(c)){
					state = STATE_ID;
				} else {
					state = STATE_ERROR;
				}
				addToBuffer(c);
				break;
			case STATE_NUMBER:
				if(isdigit(c)){
					addToBuffer(c);
				} else if(isspace(c)){
					addToBuffer('\0');
					printToken(tokenBuffer,state);
					tokenBufferEnd = 0;
					state = STATE_EMPTY;
				} else if(c == '(' || c==')') {
					addToBuffer('\0');
					printToken(tokenBuffer,state);
					state = STATE_EMPTY;
					ungetc(c,stdin);
					tokenBufferEnd = 0;
				} else {
					state = STATE_ERROR;
					addToBuffer(c);
				}
				break;
			default:
				fputs("Encountered an undefined state. Interpreting as an erroneous token.",stderr);
				state = STATE_ERROR;
			case STATE_ERROR:
				if(isspace(c)){
					addToBuffer('\0');
					printToken(tokenBuffer,state);
					tokenBufferEnd = 0;
					state = STATE_EMPTY;
					fprintf(stderr,"Erroneous token encountered: %s\n",tokenBuffer);
				} else {
					addToBuffer(c);
				}
				break;
			case STATE_LIST_START:
				addToBuffer('\0');
				printToken(tokenBuffer,state);
				tokenBufferEnd = 0;
				//set it up so that the next pass takes care of it
				if(isspace(c)){
					state = STATE_EMPTY;
					continue;
				}else if(isalpha(c)){
					state = STATE_ID;
				} else if(isdigit(c)){
					state = STATE_NUMBER;
				} else if(c == '('){
					state = STATE_LIST_START;
				} else if( c == ')'){
					state = STATE_LIST_END;
				}
				addToBuffer(c);
				break;
			case STATE_LIST_END:
				addToBuffer('\0');
				printToken(tokenBuffer,state);
				state = STATE_EMPTY;
				tokenBufferEnd = 0;
				break;
			case STATE_ID:
				if(isspace(c)){
					addToBuffer('\0');
					printToken(tokenBuffer,state);
					state = STATE_EMPTY;
				} else if(isalnum(c) || c=='_'){
					addToBuffer(c);
				} else if(c == '(' || c==')'){
					addToBuffer('\0');
					printToken(tokenBuffer, state);
					state = STATE_EMPTY;
					ungetc(c,stdin);
				} else {
					addToBuffer(c);
					state = STATE_ERROR;
				}
				break;
		}
	}
	if(state != STATE_EMPTY){
		addToBuffer('\0');
		printToken(tokenBuffer, state);
	}
}

void printToken(char * token, int state){
	printf("%d %s\n", state, token);
}