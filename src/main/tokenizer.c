#pragma once

#include "../tokenizer.c"

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
	int lineNumber = 1;
	int colNumber = 0; //will increment to 1 on the first read
	int colNumberAtStartOfToken = 0;

	while( (c = peekChar()) != EOF){
		//printf("STATE %d CHAR '%c'\n",state, c);
		switch(state){
			case STATE_EMPTY:
				assert(tokenBufferEnd == 0);
				if(is_whitespace(c)){
					consumeChar(); //consume this one whitespace character
					if(c == '\n'){
						lineNumber++;
						colNumber = 0; //will increment to 1 on the next read
					} else {
						colNumber++;
					}
					continue; // don't add to the buffer
				} else if(is_digit(c)){
					state = STATE_NUMBER;
				} else if(c == '('){ // TODO: change to general brace token state
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
				addToTokenBuffer(c);
				colNumber++;
				colNumberAtStartOfToken = colNumber; //save this so that we can print the correct spot later
				consumeChar();
				break;
			case STATE_NUMBER:
				if(is_digit(c)){
					addToTokenBuffer(c);
					consumeChar();
					colNumber++;
				} else {
					addToTokenBuffer('\0');
					printTokenData(state, lineNumber, colNumberAtStartOfToken, tokenBuffer);
					tokenBufferEnd = 0;
					state = STATE_EMPTY;
				}
				break;
			default:
				fputs("Encountered an undefined state. Interpreting as an erroneous token.",stderr);
				state = STATE_ERROR; //fallthrough to STATE_ERROR because we encountered an error
			case STATE_ERROR:
				if(is_whitespace(c)){
					addToTokenBuffer('\0');
					printTokenData(state, lineNumber, colNumber, tokenBuffer); //choose colNumber to report the precise spot where the error happened
					tokenBufferEnd = 0;
					state = STATE_EMPTY;
					fprintf(stderr,"Erroneous token encountered: %s\n",tokenBuffer);
				} else {
					addToTokenBuffer(c);
					consumeChar();
					colNumber++;
				}
				break;
			case STATE_LIST_START:
				//Expected to only be one char.
				//This char would have already been added by the empty state,
				//so just end the token here.
				addToTokenBuffer('\0');
				printTokenData(state, lineNumber, colNumberAtStartOfToken, tokenBuffer);
				tokenBufferEnd = 0;
				state = STATE_EMPTY;
				//let the next pass takes care of it
				break;
			case STATE_LIST_END:
				addToTokenBuffer('\0');
				printTokenData(state, lineNumber, colNumberAtStartOfToken, tokenBuffer);
				tokenBufferEnd = 0;
				state = STATE_EMPTY;
				break;
			case STATE_ID:
				if(is_id(c)){
					addToTokenBuffer(c);
					consumeChar(); //don't nest this call, because c is already equal to the value this would return
				} else {
					addToTokenBuffer('\0');
					printTokenData(state, lineNumber, colNumberAtStartOfToken, tokenBuffer);
					state = STATE_EMPTY;
					//don't consume the buffered char, though, to let the empty state handle it
				}
				break;
			case STATE_COMMENT:
				if(c == '\n'){
					state = STATE_EMPTY;
				} else { //only consume up UNTIL the newline, and let the empty state handle the wraparound
					consumeChar();	
				}
				break;
		}
	}
	//Handle the final token when EOF is encountered
	if(state != STATE_EMPTY && state != STATE_COMMENT){
		addToTokenBuffer('\0');
		printTokenData( state, lineNumber, colNumberAtStartOfToken, tokenBuffer);
	}
}