#pragma once
#include "states.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include "token.h"
#define MEMORY_ALLOCATION_ERROR -2

#define addToBuffer(c) (tokenBufferEnd < MAX_TOKEN_SIZE || c=='\0')?(tokenBuffer[tokenBufferEnd++] = c):0


//int main(int argc, char** argv);