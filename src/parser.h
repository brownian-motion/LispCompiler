#pragma once
#include "token.h"
#include <stdio.h>
#include <assert.h>
#include "parsenode.h"
#include "stack.h"

#define PARSE_SUCCESS 0
#define PARSE_ERROR_INVALID_TOKEN -1
#define PARSE_ERROR_SYNTAX_ERROR -2
#define PARSE_ERROR_EARLY_EOF -3
#define PARSE_ERROR_NON_EMPTY_STACK -4
#define PARSE_ERROR_UNKNOWN_NODE_REDUCED -5
#define PARSE_ERROR_PROGRAM_NODE_REDUCED -6

int buildParseTree(FILE *, struct parsenode *);
int shouldReduce(struct parseStack, token);
// int isTokenInPredictSet(struct parsenode, token);