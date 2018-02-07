#pragma once

#include <malloc.h>
#include "../types/token.h"
#include "../util/annotations.h"

typedef enum {
    PARASENODE_TYPE_ATOM,
    PARSENODE_TYPE_E,
    PARSENODE_TYPE_ES,
    PARSENODE_TYPE_PROGRAM
} parsenode_type_t;

//If numChildren is 0, this is assumed to have children NULL and be a terminator if tokenPtr is non-NULL, or represent an empty string if tokenPtr is NULL.
//Otherwise, it is assumed that tokenPtr is NULL and children is not.
typedef struct parsenode_t {
    parsenode_type_t type;
    int numChildren;
    struct parsenode_t *children; //an array of parsenode_t structs
    token_t *tokenPtr;
    bool isValid;
} parsenode_t;

parsenode_t *parsenodeAlloc(int num) {
    return (parsenode_t *) malloc(num * sizeof(parsenode_t));
}

char *getNameOfParseNodeType(int type) {
    switch (type) {
        case PARASENODE_TYPE_ATOM:
            return "ATOM";
        case PARSENODE_TYPE_E:
            return "E";
        case PARSENODE_TYPE_ES:
            return "ES";
        case PARSENODE_TYPE_PROGRAM:
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

void printParseNode(parsenode_t node) {
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
parsenode_t makeAtomParseNodeFromToken(token_t *tokenPtr) {
    parsenode_t out = {PARASENODE_TYPE_ATOM, 0, NULL, tokenPtr, 1};
    return out;
}

/**
 * Makes a parse node that represents the end of a list, an Es with no children.
 * This Es is allocated on the stack. //TODO: why?
 */
parsenode_t makeEmptyEs() {
    parsenode_t out = {PARSENODE_TYPE_ES, 0, NULL, NULL, 1};
    return out;
}

/**
 * Returns true if the given atom can be transformed into an E.
 * This is the case when the atom is well-formed, and is a number or identifier.
 */
bool canMakeEFromAtom(parsenode_t atom) {
    return atom.isValid && (atom.tokenPtr->type == TYPE_TOKEN_NUMBER || atom.tokenPtr->type == TYPE_TOKEN_ID);
}

/**
 * Creates an E parse node form an atom.
 * This E is allocated on the stack.
 */
parsenode_t makeEFromAtom(parsenode_t atom) {
    int isValid = (atom.type == PARASENODE_TYPE_ATOM) && atom.isValid;
    struct parsenode_t *children = parsenodeAlloc(1);
    children[0] = atom;
    struct parsenode_t out = {PARSENODE_TYPE_E, 1, children, NULL, isValid};
    return out;
}

/**
 * Checks if an E representing a list can be made from the four arguments.
 * This is true when they match the form ( E Es )
 */
bool canMakeEFromList(parsenode_t lparen, parsenode_t e, parsenode_t es, parsenode_t rparen) {
    return (lparen.type == PARASENODE_TYPE_ATOM && lparen.tokenPtr->type == TYPE_TOKEN_LPAREN)
           && (e.type == PARSENODE_TYPE_E)
           && (es.type == PARSENODE_TYPE_ES)
           && (rparen.type == PARASENODE_TYPE_ATOM && rparen.tokenPtr->type == TYPE_TOKEN_RPAREN)
           && lparen.isValid && e.isValid && es.isValid && rparen.isValid;
}

/**
 * Makes an E from four parse nodes representing a list.
 * This E is stack-allocated
 * 
 * See: canMakeEFromList()
 */
parsenode_t makeEFromList(parsenode_t lparen, parsenode_t e, parsenode_t es, parsenode_t rparen) {
    bool isValid = canMakeEFromList(lparen, e, es, rparen);
    /*struct parsenode_t* children = parsenodeAlloc(4);
    children[0] = lparen;
    children[1] = e;
    children[2] = es;
    children[3] = rparen;
    struct parsenode_t out = {PARSENODE_TYPE_E, 4, children, NULL, isValid};
    */
    parsenode_t *children = parsenodeAlloc(2);
    children[0] = e;
    children[1] = es;
    parsenode_t out = {PARSENODE_TYPE_E, 2, children, NULL, isValid};
    return out;
}

/**
 * Makes an Es from an E and an Es. This is the parsing equivalent to creating a cons cell.
 * This Es is stack-allocated.
 */
parsenode_t makeEsFromList(parsenode_t e, parsenode_t es) {
    bool isValid = ((e.type == PARSENODE_TYPE_E) && e.isValid)
                  && ((es.type == PARSENODE_TYPE_ES) && e.isValid);
    parsenode_t *children = parsenodeAlloc(2);
    children[0] = e;
    children[1] = es;
    parsenode_t out = {PARSENODE_TYPE_ES, 2, children, NULL, isValid};
    return out;
}

struct parsenode_t makeProgram(struct parsenode_t e) {
    struct parsenode_t *children = parsenodeAlloc(1);
    children[0] = e;
    struct parsenode_t out = {PARSENODE_TYPE_PROGRAM, 1, children, NULL, e.isValid};
    return out;
}