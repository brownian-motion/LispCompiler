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
		int state;
		fscanf(file, "%d %s\n",&state, buffer);
		token out = {state, buffer};
		return out;
	} else {
		char * buffer = (char*) malloc(sizeof(char));
		buffer[0] = '\0';
		token out = {TYPE_TOKEN_EOF, buffer};
		return out;
	}
}

token getToken(){
	return fgetToken(stdin);
}

void printToken(token t){
	printf(t.text);
}

void printTokenDebug(token t){
	printf("{token type:%d, text:\"%s\"}",t.type, t.text);
}