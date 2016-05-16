#pragma once
#include "states.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include "token.h"
#define MEMORY_ALLOCATION_ERROR -2

#define addToTokenBuffer(c) (tokenBufferEnd < MAX_TOKEN_SIZE || c=='\0')?(tokenBuffer[tokenBufferEnd++] = c):0

int is_id_start(char c){
	return isalpha(c) || (c == '_');
}

int is_digit(char c){
	return isdigit(c);
}

int is_id(char c){
	return is_id_start(c) || is_digit(c) || c=='?' || c=='!' || c=='-' || c=='<' || c=='>';
}

/**
 * Returns true if c is a whitespace character
 */
int is_whitespace(char c){
	return isspace(c);
}

/**
 * Returns true if the character is considered an operator.
 * Valid operators are + - * / %  = & | < > !
 */
int is_op_char(char c){
	return c=='+' || c=='-' || c=='*' || c=='/' || c=='%' || c=='=' || c=='&' || c=='|' || c== '<' || c=='>' || c=='!';
}

/**
 * Returns true if the character is considered a brace -
 * that is, if the character is a parenthesis, curly brace, or square brace
 */
int is_brace(char c){
	return c=='(' || c==')' || c=='{' || c=='}' || c=='[' || c==']';
}

char fPeekChar(FILE*);
char fConsumeChar(FILE*);

//Reads from stdin
char peekChar();
char consumeChar();