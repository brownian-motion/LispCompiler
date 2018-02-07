#pragma once

#include "token.h"
#include "primitiveFunction.h"
#include "../util/annotations.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

/**
 * This file describes a struct type representing a node in a syntax tree.
 * It also includes functions to generate, manipulate, and print these nodes.
 */
//These aren't static const int because they won't work in switch/case
typedef enum {
    SYNTAX_CAR_TYPE_EMPTY,
    SYNTAX_CAR_TYPE_TOKEN,
    SYNTAX_CAR_TYPE_SYNTAX_NODE,
    //The following three (_D, _NUMBER, and _STRING) are not yet implemented:
    SYNTAX_CAR_TYPE_IDENTIFIER,
    SYNTAX_CAR_TYPE_NUMBER,
    SYNTAX_CAR_TYPE_STRING,
    SYNTAX_CAR_TYPE_PRIMITIVE,
    SYNTAX_CAR_TYPE_LAMBDA
} carType_t;

#define EMPTY_SYNTAX_NODE_CDR NULL
#define NIL &EMPTY_SYNTAX_NODE

char *getSyntaxnodeCarTypeName(carType_t carType) {
    switch (carType) {
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
struct AST_node_t;

typedef struct AST_node_t AST_node_t;

/**
 * Allocates a block of memory that can hold num AST_node_t structs using malloc(),
 * and returns a pointer to the first one.
 * This can be used to allocate a single value, or an array.
 * These syntaxnodes are not initialized.
 */
AST_node_t *ASTNodeAlloc(int num);

/**
 * Returns true if the given syntax node is the empty syntax node nil.
 * A syntax node is determined to be nil if its carType is SYNTAX_CAR_TYPE_EMPTY.
 *
 * Precondition: node is not NULL
 */
bool isNil(NILLABLE AST_node_t *node);

/**
 * Initializes the given syntaxnode node to nil, the empty node.
 *
 * Precondition: node is not NULL
 */
void initializeToEmptySyntaxNode(NOT_NULL AST_node_t *node);

/**
 * Allocates a single syntaxnode struct in heap memory,
 * initializes it to nil (the empty syntax node), and 
 * returns a pointer to it.
 */
AST_node_t *emptySyntaxnodeAlloc();

/**
 * Allocates and returns a single syntax node using malloc()
 * which is initialized as a non-leaf node of an asbtract syntax
 * tree, with the given car and cdr.
 */
NOT_NIL AST_node_t *allocASTNodeFromCons(NILLABLE AST_node_t *car, NILLABLE AST_node_t *cdr);

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
bool isASTNodeAnAtom(NILLABLE AST_node_t *n);

/**
 * Prints out the value of the syntaxnode n, which is assumed to be an atom, to f.
 */
void fprintAtom(FILE *f, ATOM AST_node_t *n);

/**
 * Prints out a textual representation of the syntax subtree n to the FILE f
 *
 * TODO: add support for quoting
 * TODO: make it not print nil at the ends of sublists when quoting
 */
void fprintAST(FILE *f, NILLABLE AST_node_t *n);


void printAST(NILLABLE AST_node_t *n);