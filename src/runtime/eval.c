#pragma once

#include "eval.h"
#include "primitives.h"

void convertTokenToFloat(AST_node_t* atomNode){
	assert(atomNode->carType == AST_NODE_CAR_TYPE_TOKEN
		&& atomNode->atom->type == TYPE_TOKEN_NUMBER);
	atomNode->carType = AST_NODE_CAR_TYPE_NUMBER;
	char* floatText = atomNode->atom->text;
	free(atomNode->atom); //no longer need this token_t, since we can safely replace it for all syntax nodes with its value
	atomNode->floatValue = parseFloat(floatText);
}

void convertTokenToPrimitive(AST_node_t* primitiveNode, PRIMITIVE_FUNCTION* primitive){
	assert(primitiveNode->carType == AST_NODE_CAR_TYPE_TOKEN
		&& primitiveNode->atom->type == TYPE_TOKEN_ID);
	free(primitiveNode->atom); //no longer need this token_t, since we can safely replace it for all syntax nodes with its value
	primitiveNode->carType = AST_NODE_CAR_TYPE_PRIMITIVE;
	primitiveNode->primitive = primitive;
}

AST_node_t* lookupIdentifier(environment_t* scope, AST_node_t* identifierNode){
	assert(identifierNode != NULL 
		&& identifierNode->carType == AST_NODE_CAR_TYPE_TOKEN
		&& identifierNode->atom->type == TYPE_TOKEN_ID);
	char* identifier = identifierNode->atom->text;
	assert(isDefined(scope, identifier));
	environment_t* definition = findDefinitionForIdentifier(scope, identifier);
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

AST_node_t* eval(environment_t * environment, AST_node_t* listOfArguments){
	assert(listOfArguments != NULL); //todo: return nil?
	if(DO_PRINT_RUNTIME_TRACE){
		fprintf(stderr, "Evaluating the following (%s): ", getASTNodeCarTypeName(listOfArguments->carType));
		printListToStdout(environment, quote(environment, listOfArguments));
	}
	switch(listOfArguments->carType){
		case AST_NODE_CAR_TYPE_TOKEN:
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
						fprintf(stderr,"Encountered unexpected kind of token_t: %s %s\n", getTokenTypeName(listOfArguments->atom->type), listOfArguments->atom->text);
					}
					assert(0);
					return NULL;
			}
		//The following series are atomic types that evaluate to themselves:
		case AST_NODE_CAR_TYPE_NUMBER:
		case AST_NODE_CAR_TYPE_PRIMITIVE:
		case AST_NODE_CAR_TYPE_LAMBDA:
			return listOfArguments;
		case AST_NODE_CAR_TYPE_AST_NODE:
			//this is a list. Try to evaluate it as evaluating a lambda or evaluating a primitive
			if(DO_PRINT_RUNTIME_TRACE)
				fprintf(stderr, "Evaluating what to apply to the list...\n");
			AST_node_t* whatToApply = eval(environment, _car(environment, listOfArguments));
			if(DO_PRINT_RUNTIME_TRACE)
				fprintf(stderr, "Evaluating the body of the list...\n");
			AST_node_t* whatToApplyItTo = _cdr(environment, listOfArguments);
			switch(whatToApply->carType){
				case AST_NODE_CAR_TYPE_PRIMITIVE:
					if(DO_PRINT_RUNTIME_TRACE)
						fprintf(stderr,"Executing primitive...\n");
					return (whatToApply->primitive)(environment,whatToApplyItTo);
				default:
					if(DO_PRINT_RUNTIME_TRACE){
						fprintf(stderr,"Encountered unexpected kind of syntax node at front of list: %s\n",
								getASTNodeCarTypeName(listOfArguments->carType));
						fprintf(stderr, "Applying ");
                        fprintAST(stderr, whatToApply);
						fprintf(stderr, " to ");
                        fprintAST(stderr, whatToApplyItTo);
						putc('\n',stderr);
					}
					assert(0);
					return NULL;
			}
		case AST_NODE_CAR_TYPE_EMPTY:
			return NIL;
		default:
			fprintf(stderr,"Encountered unexpected kind of syntax node: %s\nNode: ",
					getASTNodeCarTypeName(listOfArguments->carType));
            fprintAST(stderr, listOfArguments);
			assert(0);
			return NULL;
	}
}