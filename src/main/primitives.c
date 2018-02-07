#pragma once

#include "../runtime/runtime.h"

int main(){
	//nothing. just wanted to test these included files
	environment_t * environment = definePrimitive(NULL, "plus", &plus);
	AST_node_t* plusNode = emptySyntaxnodeAlloc();
	plusNode->carType = SYNTAX_CAR_TYPE_TOKEN;
	plusNode->atom = allocAndInitializeToken(TYPE_TOKEN_ID, -1, -1, "plus");

	AST_node_t* one = emptySyntaxnodeAlloc();
	one->carType = SYNTAX_CAR_TYPE_TOKEN;
	one->atom = allocAndInitializeToken(TYPE_TOKEN_NUMBER, -1, -1, "1");
	// one = eval(NULL, one);
	AST_node_t* two = emptySyntaxnodeAlloc();
	two->carType = SYNTAX_CAR_TYPE_TOKEN;
	two->atom = allocAndInitializeToken(TYPE_TOKEN_NUMBER, -1, -1, "2");
	// two = eval(NULL, two);
	AST_node_t* listOfTwo = allocASTNodeFromCons(one,
                                                 allocASTNodeFromCons(two, NULL));
	AST_node_t* listOfThree = allocASTNodeFromCons(plusNode, listOfTwo);
	printListToStdout(environment, allocASTNodeFromCons(eval(environment, listOfThree), NULL));

	// printListToStdout(NULL, allocASTNodeFromCons(plus(NULL, listOfTwo), NULL));
	return 0;
}