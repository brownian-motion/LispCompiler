#pragma once

#include <malloc.h>
#include "../types/token.h"

#define TYPE_ATOM 1
#define TYPE_E 2
#define TYPE_ES 3
#define TYPE_PROGRAM 4

//If numChildren is 0, this is assumed to have children NULL and be a terminator if tokenPtr is non-NULL, or represent an empty string if tokenPtr is NULL.
//Otherwise, it is assumed that tokenPtr is NULL and children is not.
struct parsenode {
    int type;
    int numChildren;
    struct parsenode *children; //an array of parsenode structs
    token_t *tokenPtr;
    int isValid;
};

typedef struct parsenode parsenode_t;

parsenode_t *parsenodeAlloc(int num) {
    return (parsenode_t *) malloc(num * sizeof(parsenode_t));
}

char *getNameOfParseNodeType(int type) {
    switch (type) {
        case TYPE_ATOM:
            return "ATOM";
        case TYPE_E:
            return "E";
        case TYPE_ES:
            return "ES";
        case TYPE_PROGRAM:
            return "PROGRAM";
        default:
            return "UNKNOWN_TYPE";
    }
}

void fprintParseNode(FILE *file, parsenode_t node) {
    fprintf(file, "{type:%s(%d), ", getNameOfParseNodeType(node.type), node.type);
    if (!node.isValid) {
        fprintf(file, "INVALID}");
        return;
    }
    if (node.numChildren == 0) {
        fprintf(file, "token_t:\"");
        if (node.tokenPtr == NULL)
            fprintf(file, "NULL_TOKEN");
        else
            fprintTokenText(file, *(node.tokenPtr));
        fprintf(file, "\"}");
    } else {
        fprintf(file, "children: (");
        for (int i = 0; i < node.numChildren; i++) {
            if (i != 0) {
                fprintf(file, ", ");
            }
            fprintParseNode(file, node.children[i]);
        }
        fprintf(file, ")}");
    }
}

void printParseNode(struct parsenode node) {
    fprintParseNode(stdout, node);
}

/**
 * Makes an atom parse node from the given pointer to a token, which is assumed to be heap-allocated.
 * This atom is stack-allocated.
 *
 * It is expected that by passing a token to this function, that the caller is 
 * relinquishing control of the token to the atom that will be returned.
 * This is important to ensure that there are no memory leaks, or reads from
 * deleted memory.
 * tokenPtr is copied directly (shallowly) into the new parse node.
 */
struct parsenode makeAtom(token_t *tokenPtr) {
    struct parsenode out = {TYPE_ATOM, 0, NULL, tokenPtr, 1};
    return out;
}

/**
 * Makes a parse node that represents the end of a list, an Es with no children.
 * This Es is allocated on the stack. //TODO: why?
 */
struct parsenode makeEmptyEs() {
    struct parsenode out = {TYPE_ES, 0, NULL, NULL, 1};
    return out;
}

/**
 * Returns true if the given atom can be transformed into an E.
 * This is the case when the atom is well-formed, and is a number or identifier.
 */
int canMakeEFromAtom(struct parsenode atom) {
    return atom.isValid && (atom.tokenPtr->type == TYPE_TOKEN_NUMBER || atom.tokenPtr->type == TYPE_TOKEN_ID);
}

/**
 * Creates an E parse node form an atom.
 * This E is allocated on the stack.
 */
struct parsenode makeEFromAtom(struct parsenode atom) {
    int isValid = (atom.type == TYPE_ATOM) && atom.isValid;
    struct parsenode *children = parsenodeAlloc(1);
    children[0] = atom;
    struct parsenode out = {TYPE_E, 1, children, NULL, isValid};
    return out;
}

/**
 * Checks if an E representing a list can be made from the four arguments.
 * This is true when they match the form ( E Es )
 */
int canMakeEFromList(struct parsenode lparen, struct parsenode e, struct parsenode es, struct parsenode rparen) {
    return (lparen.type == TYPE_ATOM && lparen.tokenPtr->type == TYPE_TOKEN_LPAREN)
           && (e.type == TYPE_E)
           && (es.type == TYPE_ES)
           && (rparen.type == TYPE_ATOM && rparen.tokenPtr->type == TYPE_TOKEN_RPAREN)
           && lparen.isValid && e.isValid && es.isValid && rparen.isValid;
}

/**
 * Makes an E from four parse nodes representing a list.
 * This E is stack-allocated
 * 
 * See: canMakeEFromList()
 */
struct parsenode
makeEFromList(struct parsenode lparen, struct parsenode e, struct parsenode es, struct parsenode rparen) {
    int isValid = canMakeEFromList(lparen, e, es, rparen);
    /*struct parsenode* children = parsenodeAlloc(4);
    children[0] = lparen;
    children[1] = e;
    children[2] = es;
    children[3] = rparen;
    struct parsenode out = {TYPE_E, 4, children, NULL, isValid};
    */
    struct parsenode *children = parsenodeAlloc(2);
    children[0] = e;
    children[1] = es;
    struct parsenode out = {TYPE_E, 2, children, NULL, isValid};
    return out;
}

/**
 * Makes an Es from an E and an Es. This is the parsing equivalent to creating a cons cell.
 * This Es is stack-allocated.
 */
struct parsenode makeEsFromList(struct parsenode e, struct parsenode es) {
    int isValid = ((e.type == TYPE_E) && e.isValid)
                  && ((es.type == TYPE_ES) && e.isValid);
    struct parsenode *children = parsenodeAlloc(2);
    children[0] = e;
    children[1] = es;
    struct parsenode out = {TYPE_ES, 2, children, NULL, isValid};
    return out;
}

struct parsenode makeProgram(struct parsenode e) {
    struct parsenode *children = parsenodeAlloc(1);
    children[0] = e;
    struct parsenode out = {TYPE_PROGRAM, 1, children, NULL, e.isValid};
    return out;
}