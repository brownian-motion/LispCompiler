#pragma once
#include "states.h"
#define MAX_TOKEN_SIZE 100

#define TYPE_TOKEN_LPAREN STATE_LIST_START
#define TYPE_TOKEN_RPAREN STATE_LIST_END
#define TYPE_TOKEN_NUMBER STATE_NUMBER
#define TYPE_TOKEN_ID STATE_ID
#define TYPE_TOKEN_PLUS STATE_ADD
#define TYPE_TOKEN_MINUS STATE_SUBTRACT

struct token{
	int type;
	char * text;
};

struct token * tokenAlloc(int numTokens){
	return (struct token *) malloc(numTokens * sizeof(struct token));
}

struct token getToken(){
	char* buffer = (char *) malloc(MAX_TOKEN_SIZE+1);
	buffer[MAX_TOKEN_SIZE] = '\0';
	int state;
	scanf("%d %s\n",&state, buffer);
	struct token out = {state, buffer};
	return out;
}

void printToken(struct token t){
	printf(t.text);
}