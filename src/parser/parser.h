#pragma once
#include "../types/token.h"
#include <stdio.h>
#include <assert.h>
#include "parsenode.h"
#include "tokenizer.c"
#include "stack.h"

#define PARSE_SUCCESS 0
#define PARSE_ERROR_INVALID_TOKEN -1
#define PARSE_ERROR_SYNTAX_ERROR -2
#define PARSE_ERROR_EARLY_EOF -3
#define PARSE_ERROR_NON_EMPTY_STACK -4
#define PARSE_ERROR_UNKNOWN_NODE_REDUCED -5
#define PARSE_ERROR_PROGRAM_NODE_REDUCED -6

int buildParseTree(FILE *, struct parsenode *);
int shouldReduce(struct parseStack, token_t *);
// int isTokenInPredictSet(struct parsenode, token_t);