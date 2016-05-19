#pragma once

#include "syntax.c"
#include "environment.h"

/**
 * Defines a collection of primitive macros written in C instead of JLisp,
 * accessible in the global scope.
 */

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
 * Modifies the given syntaxnode atom, whose car is a token, so that is represents its actual value.
 * Also frees the token ??? //TODO: Decide this
 *
 * Floats get a floatValue and carType _NUMBER.
 * Strings get a stringValue and carType _STRING
 * Identifiers get a carType _IDENTIFIER
 */
void evaluateTokenType(syntaxnode* atomWithUnresolvedToken){
	token t = *atomWithUnresolvedToken->atom;
	//free(atomWithUnresolvedToken->atom);
	if(is_float(t.text)){
		atomWithUnresolvedToken->carType = SYNTAX_CAR_TYPE_NUMBER;
		atomWithUnresolvedToken->floatValue = strtod(t.text,NULL);
	} else if(is_string(t.text)){
		atomWithUnresolvedToken->carType = SYNTAX_CAR_TYPE_STRING;
		atomWithUnresolvedToken->stringValue = t.text;
	} else {
		atomWithUnresolvedToken->carType = SYNTAX_CAR_TYPE_IDENTIFIER;
		atomWithUnresolvedToken->identifier = t.text;
	}
}