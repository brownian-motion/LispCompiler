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
/*
token * makeToken(int state, int lineNumber, char* text){
	token * out = tokenAlloc(1);
	out -> state = state;
	out -> lineNumber = lineNumber;
	out -> text = (char * ) malloc(sizeof(char) * (strlen(text) + 1));
	strcpy(out -> text, text);
	return out;
}

void freeToken(token * t){
	free (t->text);
	free (t);
}
*/

/**
 * Performs a shallow copy of the fields of the token pointed to by ptr
 */
void fillToken(token * ptr, int type, int lineNumber, char * text){
	ptr -> type = type;
	ptr -> lineNumber = lineNumber;
	ptr -> text = text;
}

void fgetToken(FILE *file, token * container){
	if(!feof(file)){
		char* buffer = (char *) malloc(sizeof(char) * (MAX_TOKEN_SIZE+1));
		buffer[MAX_TOKEN_SIZE] = '\0';
		int state, lineNumber;
		fscanf(file, "%d %d %s\n",&state, &lineNumber, buffer);
		fillToken(container, state, lineNumber, buffer);
	} else {
		char * buffer = (char*) malloc(sizeof(char));
		buffer[0] = '\0';
		fillToken(container, TYPE_TOKEN_EOF, -1, buffer);
	}
}

void getToken(token * container){
	fgetToken(stdin, container);
}

void fprintToken(FILE* file, token t){
	fprintf(file, "%d %d %s\n",t.type, t.lineNumber, t.text);
}

void printToken(token t){
	fprintToken(stdout, t);
}

void fprintTokenText(FILE * file, token t){
	fprintf(file, "%s",t.text);
}

void printTokenText(token t){
	fprintTokenText(stdout, t);
}

void printTokenData(int state, int lineNumber, char * text){
	token t = {state, lineNumber, text};
	printToken(t);
}

void printTokenDebug(token t){
	printf("{token type:%d, line:%d, text:\"%s\"}",t.type, t.lineNumber, t.text);
}