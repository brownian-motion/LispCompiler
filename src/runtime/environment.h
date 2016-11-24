#pragma once
#include "../types/syntaxnode.c"
#include "../types/primitiveFunction.h"
#include "../types/environmentNode.h"

/**
 * This file defines the surrounding environment for the language.
 * The environment consists of a singly-linked-list of identifier definitions.
 * Each definition matches an identifier (cstring) to an AST (syntaxnode *)
 */

#define ENVIRONMENT_NODE_TYPE_EMPTY 0
#define ENVIRONMENT_NODE_TYPE_AST 1
#define ENVIRONMENT_NODE_TYPE_PRIMITIVE 2

struct _environmentNode {
	char * identifier;
	union {
		syntaxnode* ast;
		PRIMITIVE_FUNCTION* primitive; 	//a primitive is a C function that takes in an enironment and an AST and gives back an AST
	};
	const struct _environmentNode * next;
	int nodeType; //either ENVIRONMENT_NODE_TYPE_AST or ENVIRONMENT_NODE_TYPE_PRIMITIVE
};

typedef struct _environmentNode environmentNode;

/**
 * Iterates through the given environment to see if the given identifier has a definition.
 * Returns true if a definition exists, and false otherwise.
 */
int isDefined(const environmentNode * environment, char * identifier);

/**
 * Iterates through the given environment to see if the given identifier 'key' has a definition.
 * Returns a pointer to the environmentNode that describes that definition, or NULL if none exists.
 *
 * This is particularly useful because it allows static scope:
 * the only definitions that are active in some scope (such as a lambda) are all of the definitions
 * that existed AT or BEFORE that scope's creation...
 * which is what is returned by this function.
 */
const environmentNode * findDefinitionForIdentifier(const environmentNode * environment, char * identifier);

/** 
 * Allocates using malloc() a block of memory big enough to hold num environmentNodes.
 * Returns a pointer to that block.
 */
environmentNode * allocEnvironmentNode(int num);

/**
 * Allocates and initializes a single empty environmentNode that points to NULL.
 */
environmentNode * allocInitializedEmptyEnvironmentNode();

/**
 * Overwrites the provided environmentNode* to the given [identifier => ast] definition
 *
 * Copies the pointer value of the identifier; memory management of this cstring is up to
 * the caller. Once set, this identifier must not be deleted until after this definition is destroyed.
 *
 * Precondition: definitionToOverwrite is not NULL
 */
void overwriteNodeDefinitionToAST(environmentNode * definitionToOverwrite, char * identifier, syntaxnode * ast);

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
void overwriteNodeDefinitionToPrimitive(environmentNode * definitionToOverwrite, char * identifier, PRIMITIVE_FUNCTION* primitive);

/**
 * Addes a new [identifier => AST] definition to the given environment
 * and returns a pointer to the new enviroment without modifying the old one.
 */
environmentNode* define(const environmentNode* previousEnvironment, char* identifier, syntaxnode* ast);

/**
 * Addes a new [identifier => primitive] definition to the given environment
 * and returns a pointer to the new enviroment without modifying the old one.
 */
environmentNode* definePrimitive(const environmentNode* previousEnvironment, char* identifier, PRIMITIVE_FUNCTION* primitive);
/**
 * Removes the top-level definition, if present, and frees the memory that environmentNode* occupies.
 * Returns the resulting environment (what the given environment considered its parent).
 *
 * The data stored in the given environmentNode is not modified or freed. That control is 
 * the responsibility of the caller.
 */
const environmentNode* freeTopDefinition(environmentNode* environmentToDestroy);