#pragma once
#include "states.h"
#include <stdio.h>
#define MAX_TOKEN_SIZE 100

#define TYPE_TOKEN_LPAREN STATE_LIST_START
#define TYPE_TOKEN_RPAREN STATE_LIST_END
#define TYPE_TOKEN_NUMBER STATE_NUMBER
#define TYPE_TOKEN_ID STATE_ID
#define TYPE_TOKEN_PLUS STATE_ADD
#define TYPE_TOKEN_MINUS STATE_SUBTRACT
#define TYPE_TOKEN_EOF STATE_EOF
#define TYPE_TOKEN_ERROR STATE_ERROR

struct _token{
	int type;
	int lineNumber;
	char * text;
};

typedef struct _token token;

token * tokenAlloc(int numTokens){
	return (token *) malloc(numTokens * sizeof(token));
}

token fgetToken(FILE *file){
	if(!feof(file)){
		char* buffer = (char *) malloc(sizeof(char) * (MAX_TOKEN_SIZE+1));
		buffer[MAX_TOKEN_SIZE] = '\0';
		int state, lineNumber;
		fscanf(file, "%d %d %s\n",&state, &lineNumber, buffer);
		token out = {state, lineNumber, buffer};
		return out;
	} else {
		char * buffer = (char*) malloc(sizeof(char));
		buffer[0] = '\0';
		token out = {TYPE_TOKEN_EOF, -1, buffer};
		return out;
	}
}

token getToken(){
	return fgetToken(stdin);
}

void printToken(token t){
	printf("%d %d %s\n",t.type, t.lineNumber, t.text);
}

void printTokenText(token t){
	printf("%s",t.text);
}

void printTokenData(int state, int lineNumber, char * text){
	token t = {state, lineNumber, text};
	printToken(t);
}

void printTokenDebug(token t){
	printf("{token type:%d, line: %d, text:\"%s\"}",t.type, t.lineNumber, t.text);
}