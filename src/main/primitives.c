#pragma once

#include "../runtime/runtime.h"

int main(){
	//nothing. just wanted to test these included files
	environmentNode * environment = definePrimitive(NULL, "plus", &plus);
	syntaxnode* plusNode = emptySyntaxnodeAlloc();
	plusNode->carType = SYNTAX_CAR_TYPE_TOKEN;
	plusNode->atom = allocAndInitializeToken(TYPE_TOKEN_ID, -1, -1, "plus");

	syntaxnode* one = emptySyntaxnodeAlloc();
	one->carType = SYNTAX_CAR_TYPE_TOKEN;
	one->atom = allocAndInitializeToken(TYPE_TOKEN_NUMBER, -1, -1, "1");
	// one = eval(NULL, one);
	syntaxnode* two = emptySyntaxnodeAlloc();
	two->carType = SYNTAX_CAR_TYPE_TOKEN;
	two->atom = allocAndInitializeToken(TYPE_TOKEN_NUMBER, -1, -1, "2");
	// two = eval(NULL, two);
	syntaxnode* listOfTwo = allocSyntaxnodeFromCons(one,
			allocSyntaxnodeFromCons(two,NULL));
	syntaxnode* listOfThree = allocSyntaxnodeFromCons(plusNode, listOfTwo);
	printListToStdout(environment, allocSyntaxnodeFromCons(eval(environment, listOfThree), NULL));

	// printListToStdout(NULL, allocSyntaxnodeFromCons(plus(NULL, listOfTwo), NULL));
	return 0;
}