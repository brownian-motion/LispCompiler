#pragma once

#include "ASTnode.h"
#include <stdlib.h>

/**
 * A small struct that represents a single node of an abstract syntax tree,
 * including both subtrees and atoms.
 * Whether this node is an atom or a subtree, and what sort of atom it is,
 * is determined by its carType.
 *
 * TODO: bundle the car and cdr together, since only non-atoms have a cdr
 *       and everything except SYNTAX_CAR_TYPE_SYNTAX_NODE is an atom.
 */
struct AST_node_t {
    union {
        AST_node_t *car;
        token_t *atom;
        float floatValue;
        char *stringValue;
        char *identifier;
        PRIMITIVE_FUNCTION *primitive;
    };
    AST_node_t *cdr;
    carType_t carType;
} EMPTY_SYNTAX_NODE = {NULL, EMPTY_SYNTAX_NODE_CDR, AST_NODE_CAR_TYPE_EMPTY};

/**
 * Allocates a block of memory that can hold num syntaxnodes using malloc(),
 * and returns a pointer to the first one.
 * This can be used to allocate a single value, or an array.
 * These syntaxnodes are not initialized.
 */
AST_node_t *ASTNodeAlloc(int num) {
    return (AST_node_t *) malloc(num * sizeof(AST_node_t));
}

/**
 * Returns true if the given syntax node is the empty syntax node nil.
 * A syntax node is determined to be nil if its carType is SYNTAX_CAR_TYPE_EMPTY.
 *
 * Precondition: node is not NULL
 */
int isNil(AST_node_t *node) {
    return node == NIL || node->carType == AST_NODE_CAR_TYPE_EMPTY;
}

/**
 * Initializes the given syntaxnode node to nil, the empty node.
 *
 * Precondition: node is not NULL
 */
void initializeToEmptySyntaxNode(NOT_NULL AST_node_t *node) {
    node->cdr = EMPTY_SYNTAX_NODE_CDR;
    node->carType = AST_NODE_CAR_TYPE_EMPTY;
}

/**
 * Allocates a single syntaxnode struct in heap memory,
 * initializes it to nil (the empty syntax node), and 
 * returns a pointer to it.
 */
AST_node_t *emptyASTNodeAlloc() {
    AST_node_t *node = ASTNodeAlloc(1);
    initializeToEmptySyntaxNode(node);
    return node;
}

/**
 * Allocates and returns a single syntax node using malloc()
 * which is initialized as a non-leaf node of an asbtract syntax
 * tree, with the given car and cdr.
 */
LIST AST_node_t *allocASTNodeFromCons(NILLABLE AST_node_t *car, NILLABLE AST_node_t *cdr) {
    AST_node_t *out = ASTNodeAlloc(1);
    out->carType = AST_NODE_CAR_TYPE_AST_NODE;
    out->car = car;
    out->cdr = cdr;
    return out;
}

/**
 * Returns true if the given syntax node is an atom or not.
 * An AST node is considered an atom if its carType is not
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
bool isASTNodeAnAtom(AST_node_t *n) {
    return n == NULL || n->carType != AST_NODE_CAR_TYPE_AST_NODE;
}

/**
 * Prints out the value of the syntaxnode n, which is assumed to be an atom, to f.
 */
void fprintAtom(FILE *f, ATOM AST_node_t *n) {
//Use a switch/case because of the enumerated car types like _TOKEN and _NUMBER
    switch (n->carType) {
        case AST_NODE_CAR_TYPE_TOKEN:
            fprintTokenText(f, *(n->atom));
            break;
        case AST_NODE_CAR_TYPE_NUMBER:
            fprintf(f, "%f", n->floatValue);
            break;
        case AST_NODE_CAR_TYPE_STRING:
            fprintf(f, "%s", n->stringValue);
            break;
        case AST_NODE_CAR_TYPE_IDENTIFIER:
            fprintf(f, "%s", n->identifier);
            break;
        case AST_NODE_CAR_TYPE_PRIMITIVE:
            fprintf(f, "<# procedure>");
            break;
        default:
            fprintf(stderr, "%s\n",
                    "Encountered AST_node_t atom with unexpected carType. Printing <Unexpected atom type>");
            fprintf(f, "%s", "<Unexpected atom type>");
            break;
    }
}

/**
 * Prints out a textual representation of the syntax subtree n to the FILE f
 *
 * TODO: add support for quoting
 * TODO: make it not print nil at the ends of sublists when quoting
 */
void fprintAST(FILE *f, NILLABLE AST_node_t *n) {
    if (n == NULL) {
        // fprintf(f,"nil"); //do nothing.
        return;
    }

    if (isNil(n)) { //nil
        //do nothing, assume end of list
        fprintf(f, "nil");
    } else if (isASTNodeAnAtom(n)) {
        //just print its value
        fprintAtom(f, n);
    } else if (n->carType == AST_NODE_CAR_TYPE_AST_NODE) {
        //print car
        if (!isASTNodeAnAtom(n->car))
            fprintf(f, "(");
        fprintAST(f, n->car);
        if (!isASTNodeAnAtom(n->car))
            fprintf(f, ")");
        //print cdr
        if (n->cdr != NULL && !isNil(n->cdr)) { //nil is the empty list, so it ends a list
            fprintf(f, " "); /** delimit by spaces, just like the source code of the language,
							 *  so that quoted trees end up on one line. */
            fprintAST(f, n->cdr);
        }
    } else {
        fprintf(stderr, "%s\n",
                "Encountered AST_node_t non-atom of unexpected type. Printing <Unexpected non-atom type>");
        fprintf(f, "%s\n", "<Unexpected non-atom type>");
    }
}


void printAST(AST_node_t *n) {
    fprintAST(stdout, n);
}