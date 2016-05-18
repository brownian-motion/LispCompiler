#pragma once

#include "syntax.c"
#include "environment.h"

/**
 * Defines a collection of primitive macros written in C instead of JLisp,
 * accessible in the global scope.
 */

syntaxnode* printASTToStdout(environmentNode * environment, syntaxnode* ast){
	assert(ast != NULL);
	printSyntaxnode(ast);
	putc('\n',stdout);
	return NIL;
}