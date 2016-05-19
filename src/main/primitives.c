#pragma once

#include "../environment.h"
#include "../primitives.h"

int main(){
	//nothing. just wanted to test these included files
	syntaxnode* one = emptySyntaxnodeAlloc();
	one->carType = SYNTAX_CAR_TYPE_TOKEN;
	one->atom = allocAndInitializeToken(TYPE_TOKEN_NUMBER, -1, -1, "1");
	syntaxnode* two = emptySyntaxnodeAlloc();
	two->carType = SYNTAX_CAR_TYPE_TOKEN;
	two->atom = allocAndInitializeToken(TYPE_TOKEN_NUMBER, -1, -1, "2");
	syntaxnode* listOfTwo = allocSyntaxnodeFromCons(one, allocSyntaxnodeFromCons(two,NIL));

	printASTToStdout(NULL, quote(NULL,quote(NULL, listOfTwo))); //the first is necessary to emulate how it would be called
	return 0;
}