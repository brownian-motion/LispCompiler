#pragma once

#include "eval.h"

void convertTokenToFloat(syntaxnode* atomNode){
	assert(atomNode->carType == SYNTAX_CAR_TYPE_TOKEN
		&& atomNode->type == TYPE_TOKEN_NUMBER);
	atomNode->carType = SYNTAX_CAR_TYPE_NUMBER;
	char* floatText = atomNode->atom->text;
	free(atomNode->atom); //no longer need this token, since we can safely replace it for all syntax nodes with its value
	atomNode->floatValue = strtof(floatText);
}

void convertTokenToPrimitive(syntaxnode* primitiveNode, PRIMITIVE_FUNCTION* primitive){
	assert(atomNode->carType == SYNTAX_CAR_TYPE_TOKEN
		&& atomNode->type == TYPE_TOKEN_ID);
	free(atomNode->atom); //no longer need this token, since we can safely replace it for all syntax nodes with its value
	atomNode->carType = SYNTAX_CAR_TYPE_PRIMITIVE;
	atomNode->primitive = primitive;
}

syntaxnode* lookupIdentifier(environmentNode* scope, syntaxnode* identifierNode){
	assert(identifierNode != NULL 
		&& identifierNode->carType == SYNTAX_CAR_TYPE_TOKEN 
		&& identifierNode->atom.type == TYPE_TOKEN_ID);
	char* identifier = *(atomNode->atom).text;
	assert(isDefined(scope, identifier));
	environmentNode* definition = findDefinitionForIdentifier(scope, identifier);
	switch(definition->nodeType){
		case ENVIRONMENT_NODE_TYPE_AST:
			return eval(scope, definition->ast);
		case ENVIRONMENT_NODE_TYPE_PRIMITIVE:
			convertTokenToPrimitive(identifierNode, definition->primitive);
		default:
			assert(false);
	}
}

syntaxnode* eval(environmentNode * enviroment, syntaxnode* listOfArguments){
	assert(listOfArguments != NULL);
	if(isEmptySyntaxNode(listOfArguments) || !isEmptySyntaxNode(listOfArguments->cdr)){
		puts("eval: contract violation\n\texpected exactly one argument");
	} else {
		switch(listOfArguments->carType){
			case SYNTAX_CAR_TYPE_TOKEN:
				//then look up the identifier
				return eval(environment, lookupIdentifier(listOfArguments));
			case SYNTAX_CAR_TYPE_NUMBER:
				return listOfArguments;
			case SYNTAX_CAR_TYPE_PRIMITIVE:
				return *(listOfArguments->primitive)(environment, listOfArguments);
			default:
				assert(false);
		}
	}
}