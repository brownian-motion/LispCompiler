#pragma once

#include "primitives.h"

AST_node_t* printListToStdout(environment_t * environment, AST_node_t* listOfArguments){
	assert(listOfArguments != NULL);
	if(isNil(listOfArguments)){
		puts("print: contract violation\n\texpected at least one argument");
	} else {
		while(listOfArguments != NULL && !isNil(listOfArguments)){
            printAST(listOfArguments->car);
			putc('\n',stdout);
			listOfArguments = _cdr(environment, listOfArguments);
		}
	}
	return NIL;
}

AST_node_t* car(environment_t* environment, AST_node_t* listOfArguments){
	return _car(environment, eval(environment, listOfArguments));
}

AST_node_t* _car(environment_t* environment, AST_node_t* listOfArguments){
	assert(listOfArguments != NULL);
	if(listOfArguments->carType == AST_NODE_CAR_TYPE_AST_NODE){
		return listOfArguments->car;
	} else {
		puts("car: contract violation\n\texpected: pair");
		return NULL; //indicate an error using the best means we have in C
	}
}

AST_node_t* cdr(environment_t* environment, AST_node_t* listOfArguments){
	return _cdr(environment, eval(environment,listOfArguments));
}

AST_node_t* _cdr(environment_t* environment, AST_node_t* listOfArguments){
	assert(listOfArguments != NULL);
	if(listOfArguments->cdr != NULL){
		return listOfArguments->cdr;
	} else {
		return NIL;
		// puts("cdr: contract violation\n\texpected: pair");
		// return NULL; //indicate an error using the best means we have in C
	}
}

AST_node_t* cons(environment_t* environment, AST_node_t* listOfArguments){
	assert(listOfArguments != NULL);

	AST_node_t* carNode = eval(environment, _car(environment, listOfArguments));
	AST_node_t* cdrNode = _car(environment,_cdr(environment,listOfArguments));
	if(cdrNode->carType != AST_NODE_CAR_TYPE_AST_NODE)
		cdrNode = eval(environment, cdrNode);

	return allocASTNodeFromCons(carNode, cdrNode);
}

AST_node_t* quote(environment_t* environment, AST_node_t* listOfArguments){
	assert(listOfArguments != NULL);
	return allocASTNodeFromCons(listOfArguments, NIL); //its one argument should be a list
}

/**
 * Adds a definition to the scope of the body, then returns the same result as evaluating that body with that scope
 * usage:
 * (let ((<key> <value>) ... ) <body>)
 **/
AST_node_t* let(environment_t* environment, AST_node_t* listOfArguments){
	assert(listOfArguments != NULL);
	environment_t* newEnvironment = environment;
	assert(listOfArguments->carType == AST_NODE_CAR_TYPE_AST_NODE);
	AST_node_t* definitions = _car(environment, listOfArguments);
	while(definitions != NULL && !isNil(definitions)){
		assert(definitions->carType == AST_NODE_CAR_TYPE_AST_NODE);
		AST_node_t* definition = _car(environment, definitions);
		definitions = _cdr(environment, definitions);
		assert (definition->carType == AST_NODE_CAR_TYPE_AST_NODE);
		AST_node_t* keyNode = _car(environment, definition);

		assert(keyNode->carType == AST_NODE_CAR_TYPE_TOKEN);
		token_t* key = keyNode->atom;
		assert(key->type == TYPE_TOKEN_ID);

		newEnvironment = define(newEnvironment, key->text, _car(environment, _cdr(environment, definition)));
	}

	AST_node_t * out = eval(newEnvironment, _car(newEnvironment, _cdr(newEnvironment, listOfArguments)));

	while(environment != newEnvironment){
		newEnvironment = freeTopDefinition(newEnvironment);
	}

	return out;
}


AST_node_t * plus(environment_t* environment, AST_node_t* listOfArguments){
	assert(listOfArguments != NULL);
	float sum = 0.0f;
	while(listOfArguments != NULL && !isNil(listOfArguments)){
		// printListToStdout(environment, listOfArguments);
		AST_node_t* addend = _car(environment, listOfArguments);
		addend = eval(environment, addend); //get the number value of this addend
		assert(addend->carType == AST_NODE_CAR_TYPE_NUMBER);
		sum += addend->floatValue;
		listOfArguments = _cdr(environment, listOfArguments);
	}
	AST_node_t* result = emptyASTNodeAlloc();
	result->carType = AST_NODE_CAR_TYPE_NUMBER;
	result->floatValue = sum;
	return result;
}

AST_node_t * minus(environment_t* environment, AST_node_t* listOfArguments){
	assert(listOfArguments != NULL);

	AST_node_t* addend = _car(environment, listOfArguments);
	addend = eval(environment, addend); //get the number value of this addend
	assert(addend->carType == AST_NODE_CAR_TYPE_NUMBER);
	
	AST_node_t* subtends = _cdr(environment, listOfArguments);

	AST_node_t* result = emptyASTNodeAlloc();
	result->carType = AST_NODE_CAR_TYPE_NUMBER;
	if(subtends == NULL || isNil(subtends)){
		result->floatValue = -addend->floatValue;
	} else {
		AST_node_t* sumOfSubtends = plus(environment, subtends);
		result->floatValue = addend->floatValue - sumOfSubtends->floatValue;
	}
	return result;
}