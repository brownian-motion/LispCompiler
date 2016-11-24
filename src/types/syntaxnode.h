#pragma once
#include "token.h"
#include "primitiveFunction.h"
#ifndef NULL
#define NULL 0
#endif

/**
 * This file describes a struct type representing a node in a syntax tree.
 * It also includes functions to generate, manipulate, and print these nodes.
 */

#define SYNTAX_CAR_TYPE_EMPTY -1
#define SYNTAX_CAR_TYPE_TOKEN 1
#define SYNTAX_CAR_TYPE_SYNTAX_NODE 3
//The following three (_D, _NUMBER, and _STRING) are not yet implemented
#define SYNTAX_CAR_TYPE_IDENTIFIER 2
#define SYNTAX_CAR_TYPE_NUMBER 4
#define SYNTAX_CAR_TYPE_STRING 5
#define SYNTAX_CAR_TYPE_PRIMITIVE 6
#define SYNTAX_CAR_TYPE_LAMBDA 7

#define EMPTY_SYNTAX_NODE_CDR NULL
#define NIL &EMPTY_SYNTAX_NODE

char* getSyntaxnodeCarTypeName(int carType){
	switch(carType){
		case SYNTAX_CAR_TYPE_EMPTY:
			return "<CAR EMPTY>";
		case SYNTAX_CAR_TYPE_TOKEN:
			return "<CAR TOKEN>";
		case SYNTAX_CAR_TYPE_SYNTAX_NODE:
			return "<CAR SYNTAXNODE>";
		case SYNTAX_CAR_TYPE_IDENTIFIER:
			return "<CAR IDENTIFIER>";
		case SYNTAX_CAR_TYPE_NUMBER:
			return "<CAR NUMBER>";
		case SYNTAX_CAR_TYPE_STRING:
			return "<CAR STRING>";
		case SYNTAX_CAR_TYPE_PRIMITIVE:
			return "<CAR PRIMITIVE>";
		case SYNTAX_CAR_TYPE_LAMBDA:
			return "<CAR LAMBDA>";
		default:
			return "<CAR ???UNKNOWN???>";
	}
}

/**
 * A small struct that represents a single node of an abstract syntax tree,
 * including both subtrees and atoms.
 * Whether this node is an atom or a subtree, and what sort of atom it is,
 * is determined by its carType.
 *
 * TODO: bundle the car and cdr together, since only non-atoms have a cdr
 *       and everything except SYNTAX_CAR_TYPE_SYNTAX_NODE is an atom.
 */
struct _syntaxnode;

typedef struct _syntaxnode syntaxnode;

/**
 * Allocates a block of memory that can hold num syntaxnodes using malloc(),
 * and returns a pointer to the first one.
 * This can be used to allocate a single value, or an array.
 * These syntaxnodes are not initialized.
 */
syntaxnode * syntaxnodeAlloc(int num);

/**
 * Returns true if the given syntax node is the empty syntax node nil.
 * A syntax node is determined to be nil if its carType is SYNTAX_CAR_TYPE_EMPTY.
 *
 * Precondition: node is not NULL
 */
int isEmptySyntaxNode(syntaxnode * node);

/**
 * Initializes the given syntaxnode node to nil, the empty node.
 *
 * Precondition: node is not NULL
 */
void initializeToEmptySyntaxNode(syntaxnode * node);

/**
 * Allocates a single syntaxnode struct in heap memory,
 * initializes it to nil (the empty syntax node), and 
 * returns a pointer to it.
 */
syntaxnode * emptySyntaxnodeAlloc();

/**
 * Allocates and returns a single syntax node using malloc()
 * which is initialized as a non-leaf node of an asbtract syntax
 * tree, with the given car and cdr.
 */
syntaxnode* allocSyntaxnodeFromCons(syntaxnode* car, syntaxnode* cdr);

/**
 * Returns true if the given syntax node is an atom or not.
 * A syntax node is considered an atom if its carType is not
 * another syntax node. In use, this means that this syntax node
 * and its car are treated as the same thing; within the language,
 * the car is the actual object being manipulated. Such an atom
 * is represented as a syntax node for type consistency in the host
 * language, C.
 *
 * For example, the number 5 in the language would be represented
 * in this source code as a syntax node with a carType field of
 * SYNTAX_CAR_TYPE_NUMBER and a value field of 5.
 */
int isSyntaxNodeAnAtom(syntaxnode * n);

/**
 * Prints out the value of the syntaxnode n, which is assumed to be an atom, to f.
 */
fprintSyntaxnodeAtom(FILE * f, syntaxnode * n);

/**
 * Prints out a textual representation of the syntax subtree n to the FILE f
 *
 * TODO: add support for quoting
 * TODO: make it not print nil at the ends of sublists when quoting
 */
void fprintSyntaxnode(FILE * f, syntaxnode * n);



void printSyntaxnode(syntaxnode * n);