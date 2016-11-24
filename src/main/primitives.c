#pragma once

#include "../runtime/environment.h"
#include "../runtime/primitives.c"

int main(){
	//nothing. just wanted to test these included files
	// syntaxnode* plus = emptySyntaxnodeAlloc();
	// plus->carType = SYNTAX_CAR_TYPE_PRIMITIVE;
	// plus->primitive = &plus;

	syntaxnode* one = emptySyntaxnodeAlloc();
	one->carType = SYNTAX_CAR_TYPE_NUMBER;
	one->floatValue = 1.0f;
	// one->carType = SYNTAX_CAR_TYPE_TOKEN;
	// one->atom = allocAndInitializeToken(TYPE_TOKEN_NUMBER, -1, -1, "1");
	syntaxnode* two = emptySyntaxnodeAlloc();
	two->carType = SYNTAX_CAR_TYPE_NUMBER;
	two->floatValue = 2.0f;
	// two->carType = SYNTAX_CAR_TYPE_TOKEN;
	// two->atom = allocAndInitializeToken(TYPE_TOKEN_NUMBER, -1, -1, "2");
	syntaxnode* listOfTwo = allocSyntaxnodeFromCons(one,
			allocSyntaxnodeFromCons(two,NULL));
	// syntaxnode* listOfThree = allocSyntaxnodeFromCons(plus, listOfTwo);

	printASTToStdout(NULL, quote( NULL, plus(NULL, listOfTwo))); //the first is necessary to emulate how it would be called
	return 0;
}