#pragma once

#include "primitives.h"

syntaxnode* printASTToStdout(environmentNode * environment, syntaxnode* listOfArguments){
	assert(listOfArguments != NULL);
	if(isEmptySyntaxNode(listOfArguments) || !isEmptySyntaxNode(listOfArguments->cdr)){
		puts("print: contract violation\n\texpected exactly one argument");
	} else {
		printSyntaxnode(listOfArguments->car);
		putc('\n',stdout);
	}
	return NIL;
}

syntaxnode* car(environmentNode* environment, syntaxnode* listOfArguments){
	assert(listOfArguments != NULL);
	if(listOfArguments->carType == SYNTAX_CAR_TYPE_SYNTAX_NODE){
		return listOfArguments->car;
	} else {
		puts("car: contract violation\n\texpected: pair");
		return NULL; //indicate an error using the best means we have in C
	}
}

syntaxnode* cdr(environmentNode* environment, syntaxnode* listOfArguments){
	assert(listOfArguments != NULL);
	if(listOfArguments->cdr != NULL){
		return listOfArguments->cdr;
	} else {
		puts("cdr: contract violation\n\texpected: pair");
		return NULL; //indicate an error using the best means we have in C
	}
}

syntaxnode* cons(environmentNode* environment, syntaxnode* listOfArguments){
	assert(listOfArguments != NULL);
	return allocSyntaxnodeFromCons(
		car(environment, listOfArguments),
		car(environment,cdr(environment,listOfArguments))
		);
}

syntaxnode* quote(environmentNode* environment, syntaxnode* listOfArguments){
	assert(listOfArguments != NULL);
	return allocSyntaxnodeFromCons(listOfArguments,NIL); //its one argument should be a list
}

/**
 * Adds a definition to the scope of the body, then returns the same result as evaluating that body with that scope
 * usage:
 * (let ((var1 value1) [(var2 value2) ...] ) body)
 **/
 /**
syntaxnode* let(environmentNode* environment, syntaxnode* listOfArguments){
	assert(listOfArguments != NULL);
	environmentNode* newEnvironment = enviroment;
	syntaxnode* definitions = car(environment, listOfArguments);
	while(definitions != NULL){
		syntaxnode* definition = car(definition);
		definitions = cdr(definitions);

		assert(definition->carType == SYNTAX_CAR_TYPE_TOKEN);
		token key = definition->atom;
		assert(key.type == TYPE_TOKEN_ID);

		newEnvironment = define(newEnvironment, key.text, cdr(environment, definition));
	}

	syntaxnode * out = eval(newEnvironment, cdr(listOfArguments));

	while(environment != newEnvironment){
		newEnvironment = freeTopDefinition(newEnvironment);
	}

	return out;
}
*/

syntaxnode * plus(environmentNode* environment, syntaxnode* listOfArguments){
	assert(listOfArguments != NULL);
	float sum = 0.0f;
	while(listOfArguments != NULL){
		syntaxnode* addend = car(environment, listOfArguments);
		assert(addend->carType == SYNTAX_CAR_TYPE_NUMBER);
		sum += addend->floatValue;
		listOfArguments = cdr(environment, listOfArguments);
	}
	syntaxnode* result = emptySyntaxnodeAlloc();
	result->carType = SYNTAX_CAR_TYPE_NUMBER;
	result->floatValue = sum;
	return result;
}