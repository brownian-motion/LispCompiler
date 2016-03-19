#pragma 
#include "token.h"
#include <stdio.h>
#include <assert.h>
#include "parsenode.h"
#include "stack.h"

struct parsenode buildProgram(FILE * file);
int shouldReduce(struct parseStack, token);
// int isTokenInPredictSet(struct parsenode, token);