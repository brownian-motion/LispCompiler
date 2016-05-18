#pragma once
#include "states.h"
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include "token.h"
#define MEMORY_ALLOCATION_ERROR -2

#define addToTokenBuffer(c) (tokenBufferEnd < MAX_TOKEN_SIZE || c=='\0')?(tokenBuffer[tokenBufferEnd++] = c):0

//TODO: add functions that read directly from a FILE* file buffer and return a single token

char fPeekChar(FILE*);
char fConsumeChar(FILE*);

//Reads from stdin
char peekChar();
char consumeChar();