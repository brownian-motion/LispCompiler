#pragma once
#include "token.h"
#ifndef NULL
#define NULL 0
#endif

/**
 * This file describes a struct type representing a node in a syntax tree.
 * It also includes functiosn to generate and manipulate these nodes.
 */

#define SYNTAX_CAR_TYPE_EMPTY -1
#define SYNTAX_CAR_TYPE_TOKEN 1
#define SYNTAX_CAR_TYPE_SYNTAX_NODE 3

#define EMPTY_SYNTAX_NODE_CDR NULL

struct _syntaxnode {
	union {
		struct _syntaxnode * car;
		token * atom;
	};
	struct _syntaxnode * cdr;
	int carType;
} EMPTY_SYNTAX_NODE= {NULL,EMPTY_SYNTAX_NODE_CDR, SYNTAX_CAR_TYPE_EMPTY};

typedef struct _syntaxnode syntaxnode;

syntaxnode * syntaxnodeAlloc(int num){
	return (syntaxnode *) malloc( num * sizeof(syntaxnode));
}

int isEmptySyntaxNode(syntaxnode * node){
	return node->carType == SYNTAX_CAR_TYPE_EMPTY;
}

void initializeToEmptySyntaxNode(syntaxnode * node){
	node->cdr = EMPTY_SYNTAX_NODE_CDR;
	node->carType = SYNTAX_CAR_TYPE_EMPTY;
}

syntaxnode * emptySyntaxnodeAlloc(){
	syntaxnode * node = syntaxnodeAlloc(1);
	initializeToEmptySyntaxNode(node);
	return node;
}

syntaxnode * makeSyntaxTreeLeafFromToken(token * t){
	syntaxnode * out = emptySyntaxnodeAlloc();
	out->atom = t;
	out->carType = SYNTAX_CAR_TYPE_TOKEN;
	return out;
}

/**
 * Prints out a textual representation of the syntax subtree n to the FILE f
 */
void fprintSyntaxnode(FILE * f, syntaxnode * n){
	if(n == NULL){
		fprintf(f,"nil");
		return;
	}
	// fprintf(f,"%d:",n);

	switch(n->carType){
		case SYNTAX_CAR_TYPE_EMPTY:
			//do nothing, assume end of list
			fprintf(f,"nil");
			return;
		case SYNTAX_CAR_TYPE_TOKEN:
			fprintTokenText(f, *(n->atom));
			fprintf(f,"\n");
			break;
		case SYNTAX_CAR_TYPE_SYNTAX_NODE:
			//print car
			fprintf(f,"(");
			fprintSyntaxnode(f, n->car);
			//print cdr
			if(n->cdr != NULL){
				fprintSyntaxnode(f, n->cdr);
			}
			fprintf(f,")");
			break;
	}
}

void printSyntaxnode(syntaxnode * n){
	fprintSyntaxnode(stdout, n);
}