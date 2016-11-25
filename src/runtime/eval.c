#pragma once

#include "eval.h"

void convertTokenToFloat(syntaxnode* atomNode){
	assert(atomNode->carType == SYNTAX_CAR_TYPE_TOKEN
		&& atomNode->atom->type == TYPE_TOKEN_NUMBER);
	atomNode->carType = SYNTAX_CAR_TYPE_NUMBER;
	char* floatText = atomNode->atom->text;
	free(atomNode->atom); //no longer need this token, since we can safely replace it for all syntax nodes with its value
	atomNode->floatValue = parseFloat(floatText);
}

void convertTokenToPrimitive(syntaxnode* primitiveNode, PRIMITIVE_FUNCTION* primitive){
	assert(primitiveNode->carType == SYNTAX_CAR_TYPE_TOKEN
		&& primitiveNode->atom->type == TYPE_TOKEN_ID);
	free(primitiveNode->atom); //no longer need this token, since we can safely replace it for all syntax nodes with its value
	primitiveNode->carType = SYNTAX_CAR_TYPE_PRIMITIVE;
	primitiveNode->primitive = primitive;
}

syntaxnode* lookupIdentifier(environmentNode* scope, syntaxnode* identifierNode){
	assert(identifierNode != NULL 
		&& identifierNode->carType == SYNTAX_CAR_TYPE_TOKEN 
		&& identifierNode->atom->type == TYPE_TOKEN_ID);
	char* identifier = identifierNode->atom->text;
	assert(isDefined(scope, identifier));
	environmentNode* definition = findDefinitionForIdentifier(scope, identifier);
	switch(definition->nodeType){
		case ENVIRONMENT_NODE_TYPE_AST:
			return eval(scope, definition->ast);
		case ENVIRONMENT_NODE_TYPE_PRIMITIVE:
			convertTokenToPrimitive(identifierNode, definition->primitive);
			return identifierNode;
		default:
			assert(0);
			return NULL;
	}
}

syntaxnode* eval(environmentNode * environment, syntaxnode* listOfArguments){
	assert(listOfArguments != NULL); //todo: return nil?
	if(DO_PRINT_RUNTIME_TRACE){
		fprintf(stderr, "Evaluating the following (%s): ",getSyntaxnodeCarTypeName(listOfArguments->carType));
		printListToStdout(environment, quote(environment, listOfArguments));
	}
	switch(listOfArguments->carType){
		case SYNTAX_CAR_TYPE_TOKEN:
			//then look up the identifier
			switch(listOfArguments->atom->type){
				case TYPE_TOKEN_ID:
					if(DO_PRINT_RUNTIME_TRACE){
						fprintf(stderr,"Looking up identifier %s\n",listOfArguments->atom->text);
					}
					return eval(environment, lookupIdentifier(environment, listOfArguments));
				case TYPE_TOKEN_NUMBER:
					if(DO_PRINT_RUNTIME_TRACE){
						fprintf(stderr,"Converting number %s to float\n",listOfArguments->atom->text);
					}
					convertTokenToFloat(listOfArguments);
					return listOfArguments;
				default:
					if(DO_PRINT_RUNTIME_TRACE){
						fprintf(stderr,"Encountered unexpected kind of token: %s %s\n", getTokenTypeName(listOfArguments->atom->type), listOfArguments->atom->text);
					}
					assert(0);
					return NULL;
			}
		//The following series are atomic types that evaluate to themselves:
		case SYNTAX_CAR_TYPE_NUMBER:
		case SYNTAX_CAR_TYPE_PRIMITIVE:
		case SYNTAX_CAR_TYPE_LAMBDA:
			return listOfArguments;
		case SYNTAX_CAR_TYPE_SYNTAX_NODE:
			//this is a list. Try to evaluate it as evaluating a lambda or evaluating a primitive
			if(DO_PRINT_RUNTIME_TRACE)
				fprintf(stderr, "Evaluating what to apply to the list...\n");
			syntaxnode* whatToApply = eval(environment, car(environment, listOfArguments));
			if(DO_PRINT_RUNTIME_TRACE)
				fprintf(stderr, "Evaluating the body of the list...\n");
			syntaxnode* whatToApplyItTo = cdr(environment, listOfArguments);
			switch(whatToApply->carType){
				case SYNTAX_CAR_TYPE_PRIMITIVE:
					if(DO_PRINT_RUNTIME_TRACE)
						fprintf(stderr,"Executing primitive...\n");
					return (whatToApply->primitive)(environment,whatToApplyItTo);
				default:
					if(DO_PRINT_RUNTIME_TRACE){
						fprintf(stderr,"Encountered unexpected kind of syntax node at front of list: %s\n", getSyntaxnodeCarTypeName(listOfArguments->carType));
						fprintf(stderr, "Applying ");
						fprintSyntaxnode(stderr,whatToApply);
						fprintf(stderr, " to ");
						fprintSyntaxnode(stderr,whatToApplyItTo);
						putc('\n',stderr);
					}
					assert(0);
					return NULL;
			}
		default:
			fprintf(stderr,"Encountered unexpected kind of syntax node: %s\nNode: ", getSyntaxnodeCarTypeName(listOfArguments->carType));
			fprintSyntaxnode(stderr,listOfArguments);
			assert(0);
			return NULL;
	}
}