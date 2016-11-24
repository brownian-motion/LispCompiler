#pragma once
#include "environment.h"

/**
 * This file defines the surrounding environment for the language.
 * The environment consists of a singly-linked-list of identifier definitions.
 * Each definition matches an identifier (cstring) to an AST (syntaxnode *)
 */




/**
 * Iterates through the given environment to see if the given identifier has a definition.
 * Returns true if a definition exists, and false otherwise.
 */
int isDefined(const environmentNode * environment, char * identifier){
	while(environment != NULL){
		if(strcmp(identifier, environment->identifier) == 0){
			return 1;
		}
		environment = environment->next;
	}
	return 0;
}

/**
 * Iterates through the given environment to see if the given identifier 'key' has a definition.
 * Returns a pointer to the environmentNode that describes that definition, or NULL if none exists.
 *
 * This is particularly useful because it allows static scope:
 * the only definitions that are active in some scope (such as a lambda) are all of the definitions
 * that existed AT or BEFORE that scope's creation...
 * which is what is returned by this function.
 */
const environmentNode * findDefinitionForIdentifier(const environmentNode * environment, char * identifier){
	while(environment != NULL){
		if(strcmp(identifier, environment->identifier) == 0){
			return environment;
		}
		environment = environment->next;
	}
	return NULL;
}

/** 
 * Allocates using malloc() a block of memory big enough to hold num environmentNodes.
 * Returns a pointer to that block.
 */
environmentNode * allocEnvironmentNode(int num){
	return (environmentNode*) malloc( num * sizeof(environmentNode) );
}

/**
 * Allocates and initializes a single empty environmentNode that points to NULL.
 */
environmentNode * allocInitializedEmptyEnvironmentNode(){
	environmentNode* out = allocEnvironmentNode(1);
	out->nodeType = ENVIRONMENT_NODE_TYPE_EMPTY;
	out->next = NULL;
	return out;
}

/**
 * Overwrites the provided environmentNode* to the given [identifier => ast] definition
 *
 * Copies the pointer value of the identifier; memory management of this cstring is up to
 * the caller. Once set, this identifier must not be deleted until after this definition is destroyed.
 *
 * Precondition: definitionToOverwrite is not NULL
 */
void overwriteNodeDefinitionToAST(environmentNode * definitionToOverwrite, char * identifier, syntaxnode * ast){
	definitionToOverwrite->nodeType = ENVIRONMENT_NODE_TYPE_AST;
	definitionToOverwrite->identifier = identifier;
	definitionToOverwrite->ast = ast;
}

/**
 * Overwrites the provided environmentNode* to the given [identifier => primitive] definition
 *
 * A primitive is any C function that can accept an AST and return an AST (or nil). They are dangerous!
 * An example would be a function that accepts an AST with atoms in each car and similar ASTs
 * in each cdr and prints each of them.
 *
 * Copies the pointer value of the identifier; memory management of this cstring is up to
 * the caller. Once set, this identifier must not be deleted until after this definition is destroyed.
 *
 * Precondition: definitionToOverwrite is not NULL
 */
void overwriteNodeDefinitionToPrimitive(environmentNode * definitionToOverwrite, char * identifier, PRIMITIVE_FUNCTION* primitive){
	definitionToOverwrite->nodeType = ENVIRONMENT_NODE_TYPE_PRIMITIVE;
	definitionToOverwrite->identifier = identifier;
	definitionToOverwrite->primitive = primitive;
}

/**
 * Addes a new [identifier => AST] definition to the given environment
 * and returns a pointer to the new enviroment without modifying the old one.
 */
environmentNode* define(const environmentNode* previousEnvironment, char* identifier, syntaxnode* ast){
	environmentNode* newDefinition = allocEnvironmentNode(1);
	overwriteNodeDefinitionToAST(newDefinition, identifier, ast);
	newDefinition->next = previousEnvironment;
	return newDefinition;
}

/**
 * Addes a new [identifier => primitive] definition to the given environment
 * and returns a pointer to the new enviroment without modifying the old one.
 */
environmentNode* definePrimitive(const environmentNode* previousEnvironment, char* identifier, PRIMITIVE_FUNCTION* primitive){
	environmentNode* newDefinition = allocEnvironmentNode(1);
	overwriteNodeDefinitionToPrimitive(newDefinition, identifier, primitive);
	newDefinition->next = previousEnvironment;
	return newDefinition;
}

/**
 * Removes the top-level definition, if present, and frees the memory that environmentNode* occupies.
 * Returns the resulting environment (what the given environment considered its parent).
 *
 * The data stored in the given environmentNode is not modified or freed. That control is 
 * the responsibility of the caller.
 */
const environmentNode* freeTopDefinition(environmentNode* environmentToDestroy){
	if(environmentToDestroy == NULL)
		return NULL;
	const environmentNode* newEnvironment = environmentToDestroy->next;
	free(environmentToDestroy);
	return newEnvironment;
}