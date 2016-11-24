#pragma once
#include "states.h"
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include "../types/token.h"
#define MEMORY_ALLOCATION_ERROR -2

//TODO: add functions that read directly from a FILE* file buffer and return a single token

struct _tokenizer {
	FILE * file;

	int lineNumber;
	int colNumber;

	char * tokenBuffer;
	int tokenBufferEnd;
};

typedef struct _tokenizer tokenizer;

/**
 * Allocates a cstring buffer for use in a tokenizer, using malloc(), and returns a pointer to the result.
 * The default size is MAX_TOKEN_SIZE (plus one to hold the string terminator).
 */
char * allocTokenBuffer(){
	char *tokenBuffer = (char *) malloc(MAX_TOKEN_SIZE+1);
	if(tokenBuffer == NULL){
		fputs("Could not allocate heap memory for token buffer.", stderr);
	}
	return tokenBuffer;
}

/**
 * Allocates a tokenizer on the stack that reads from a given file f.
 */
tokenizer makeTokenizerFromFile(FILE* f){
	char * tokenBuffer = allocTokenBuffer();
	tokenizer out = {
		f,
		1,				//start the line number at 1
		0, 				//start the column number at 0 - it'll increment to 1 as soon as we read a character
		tokenBuffer,	//assume the allocation of the token buffer went okay
		0
	  };
	return out;
}

/**
 * Tries to add the char c to the given tokenizer.
 * Returns 1 on success, 0 on failure, to enable statements like if(!addToTokenBuffer(t,c)) printf("Error");
 */
int addToTokenBuffer(tokenizer * t, char c);

void skipWhitespaceUntilNextToken(tokenizer * t);
void skipCommentUntilStartOfLine(tokenizer * t);
void skipWhitespaceAndCommentsUntilNextToken(tokenizer * t);

/**
 * Returns true if this tokenizer can read more tokens from its file, and false otherwise.
 */
int hasNextToken(tokenizer * t);

/**
 * Reads the next token from the tokenizer's FILE* buffer,
 * allocates a block of memory to hold it using malloc(),
 * and returns a pointer to it.
 * If there are no more tokens, or there was an error allocating a token, returns a tokenizer representing EOF.
 */
token * getNextToken(tokenizer * t);

char fPeekChar(FILE*);
char fConsumeChar(FILE*);

//Reads from stdin
char peekChar();
char consumeChar();
