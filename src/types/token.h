#pragma once

#include "../parser/states.h"

#include <stdio.h>
#include <string.h>

#define MAX_TOKEN_SIZE 100

#define TYPE_TOKEN_LPAREN STATE_LIST_START
#define TYPE_TOKEN_RPAREN STATE_LIST_END
#define TYPE_TOKEN_NUMBER STATE_NUMBER
#define TYPE_TOKEN_ID STATE_ID
#define TYPE_TOKEN_PLUS STATE_ADD
#define TYPE_TOKEN_MINUS STATE_SUBTRACT
#define TYPE_TOKEN_EOF STATE_EOF
#define TYPE_TOKEN_ERROR STATE_ERROR

typedef struct token_t {
    int type;
    int lineNumber;
    int colNumber;
    char *text;
} token_t;

char *getTokenTypeName(int type) {
    switch (type) {
        case TYPE_TOKEN_LPAREN:
            return "<LParen>";
        case TYPE_TOKEN_RPAREN:
            return "<RParen>";
        case TYPE_TOKEN_NUMBER:
            return "<Number>";
        case TYPE_TOKEN_ID:
            return "<ID>";
        case TYPE_TOKEN_PLUS:
            return "<Plus>";
        case TYPE_TOKEN_MINUS:
            return "<Minus>";
        case TYPE_TOKEN_EOF:
            return "<EOF>";
        case TYPE_TOKEN_ERROR:
            return "<Error!>";
        default:
            return "<Unknown token_t type!>";
    }
}

token_t *tokenAlloc(int numTokens) {
    return (token_t *) malloc(numTokens * sizeof(token_t));
}

/**
 * Performs a deep copy of the fields of the token pointed to by uninitializedTokenPtr,
 * allocating a new cstring to store in uninitializedTokenPtr->text.
 * This function does not free uninitializedTokenPtr->text, so any calls to fillToken
 * providing an initialized token will result in a memory leak.
 */
void initializeToken(token_t *uninitializedTokenPtr, int type, int lineNumber, int colNumber, char *text) {
    uninitializedTokenPtr->type = type;
    uninitializedTokenPtr->lineNumber = lineNumber;
    uninitializedTokenPtr->colNumber = colNumber;
    int len = strlen(text);
    uninitializedTokenPtr->text = (char *) malloc((len + 1) * sizeof(char));
    strcpy(uninitializedTokenPtr->text, text);
}

/**
 * Allocates a token using malloc() and fills it with the given data
 */
token_t *allocAndInitializeToken(int type, int lineNumber, int colNumber, char *text) {
    token_t *tokenPtr = tokenAlloc(1);
    initializeToken(tokenPtr, type, lineNumber, colNumber, text);
    return tokenPtr;
}

void fgetAnnotatedToken(FILE *file, token_t *uninitializedContainer) {
    if (!feof(file)) {
        char *buffer = (char *) malloc(sizeof(char) * (MAX_TOKEN_SIZE + 1));
        buffer[MAX_TOKEN_SIZE] = '\0';
        int state, lineNumber, colNumber;
        fscanf(file, "%d %d %d %s\n", &state, &lineNumber, &colNumber, buffer);
        initializeToken(uninitializedContainer, state, lineNumber, colNumber, buffer);
    } else { //EOF encountered
        char *buffer = (char *) malloc(sizeof(char));
        buffer[0] = '\0';
        initializeToken(uninitializedContainer, TYPE_TOKEN_EOF, -1, -1, buffer);
    }
}

void getAnnotatedToken(token_t *uninitializedContainer) {
    fgetAnnotatedToken(stdin, uninitializedContainer);
}

//TODO: change to token_t pointer to conserve memory and speed compilation time
void fprintToken(FILE *file, token_t t) {
    fprintf(file, "%d %d %d %s\n", t.type, t.lineNumber, t.colNumber, t.text);
}

void printToken(token_t t) {
    fprintToken(stdout, t);
}

void fprintTokenText(FILE *file, token_t t) {
    fprintf(file, "%s", t.text);
}

void printTokenText(token_t t) {
    fprintTokenText(stdout, t);
}

void printTokenData(int state, int lineNumber, int colNumber, char *text) {
    token_t t = {state, lineNumber, colNumber, text};
    printToken(t);
}

void printTokenDebug(token_t *t) {
    if (t == NULL)
        printf("NULL_TOKEN");
    else
        printf("{token_t type:%s, line:%d, col:%d, text:\"%s\"}", getTokenTypeName(t->type), t->lineNumber,
               t->colNumber, t->text);
}

/**
 * Returns true if c is considered the start of an identifier or operator.
 * Any operator can bee looked up in the environment's symbol table, so 
 * any operator character can also be considered the start of an id.
 */
int is_id_start(char c) {
    return isalpha(c) || (c == '_') || is_op_char(c);
}

int is_digit(char c) {
    return isdigit(c);
}

/**
 * Returns true if c is considered part of the second (or further) characters of an identifier.
 */
int is_id(char c) {
    return is_id_start(c) || is_digit(c) || c == '?' || c == '!' || c == '-' || c == '<' || c == '>';
}

/**
 * Returns true if c is a whitespace character in the language.
 * EOF is not considered whitespace.
 */
int is_whitespace(char c) {
    return c != EOF && isspace(c);
}

/**
 * Returns true if the character is considered an operator in the language.
 * Valid operators are + - * / %  = & | < > !
 */
int is_op_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '=' || c == '&' || c == '|' || c == '<' ||
           c == '>' || c == '!';
}

/**
 * Returns true if the character is considered a brace -
 * that is, if the character is a parenthesis, curly brace, or square brace
 */
int is_brace(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']';
}

/**
 * Returns true if the cstring s represents a valid integer in the language, and false otherwise.
 * Currently, s is an integer if it contains only numeric digits.
 */
int is_integer(char *s) {
    int len = strlen(s);
    for (int i = 0; i < len; i++) {
        if (!is_digit(s[i]))
            return 0;
    }
    return 1;
}

/**
 * Returns true if the cstring s represents a valid float in the language, and false otherwise.
 * Currently, s is an float if it contains only numeric digits and a single, optional decimal point.
 * All integers are valid floats.
 */
int is_float(char *s) {
    int hasFoundDecimalPoint = 0;
    int len = strlen(s);
    for (int i = 0; i < len; i++) {
        if (s[i] == '.') {
            if (hasFoundDecimalPoint)
                return 0;
            else
                hasFoundDecimalPoint = 1;
        } else if (!is_digit(s[i])) {
            return 0;
        }
    }
    return 1;
}

/**
 * Returns true if the cstring s represents a valid string in the language, and false otherwise.
 * Currently, s is a valid string iff if begins and ends with the quote character \", and any
 * quote character in the middle of the string is escaped with a backslash \\.
 * Literal regex: "([^"]|\")*"
 */
int is_string(char *s) {
    int len = strlen(s);
    if (len < 2)
        return 0;
    if (s[0] != '\"')
        return 0;
    for (int i = 1; i < len - 1; i++) {
        if (s[i] == '\"' && s[i - 1] != '\\')
            return 0;
    }
    return s[len - 1] == '\"' && s[len - 2] != '\\';
}