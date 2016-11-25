#pragma once
#include "syntax.h"

#define DO_PRINT_RESULT_SYNTAX_TREE 0

int generateSyntaxTree(struct parsenode * parseTree, syntaxnode * syntaxTree){
	if(parseTree == NULL){
		fprintf(stderr, "Encountered a null parsenode while generating syntax tree.");
		return -4;
	} else if(!parseTree->isValid){
		fprintf(stderr, "Encountered an invalid parsenode while generating syntax tree.");
		return -3;
	} else if(parseTree->type != TYPE_PROGRAM){
		fprintf(stderr, "Expected an PROGRAM but encountered %s.", getNameOfParseNodeType(parseTree->type));
		return -2;
	} else if(parseTree->numChildren != 1){
		fprintf(stderr, "Program node doesn't have exactly 1 child.");
		return -1;
	}
	struct parsenode * e = &(parseTree->children[0]);
	syntaxnode * root = makeSyntaxnodeFromE(e);
	*syntaxTree = *root; //copy the data, not memory addresses
	free(root); //because we copied the data into syntaxTree, we can free this memory space
	return 0;
}


/**
 * Tries to make a subtree of a syntax tree from the given parse tree subtree, by the rules
 * of the context-free grammar defined for the language.
 *
 * This function returns a pointer to a heap-allocated syntaxnode, or NULL if that fails.
 */
syntaxnode * makeSyntaxnodeFromEs(struct parsenode * node){
	if(node == NULL){
		fprintf(stderr, "Encountered a null parsenode while generating syntax tree.");
		return NULL;
	} else if(!node->isValid){
		fprintf(stderr, "Encountered an invalid parsenode while generating syntax tree.");
		return NULL;
	} else if(node->type != TYPE_ES){
		fprintf(stderr, "Expected an ES but encountered %s.", getNameOfParseNodeType(node->type));
		return NULL;
	}
	switch(node->numChildren){
		case 0:
			//this ES is the end of a list. Return an empty syntax node.
			return emptySyntaxnodeAlloc();
		case 2:
			//This ES is the middle of a list. Return a syntax node with ES->E as car and ES->ES as cdr
		{ //limits lexical scope of enclosed variables
			struct parsenode e = node->children[0];
			struct parsenode es = node->children[1];
			if(e.type != TYPE_E){
				fprintf(stderr, "Expected an E but encountered %s.", getNameOfParseNodeType(node->type));
				return NULL;
			}
			if(es.type != TYPE_ES){
				fprintf(stderr, "Expected an E but encountered %s.", getNameOfParseNodeType(node->type));
				return NULL;
			}
			syntaxnode* out = emptySyntaxnodeAlloc();
			syntaxnode* car = makeSyntaxnodeFromE(&e);
			syntaxnode* cdr = makeSyntaxnodeFromEs(&es); //overwrites what was there?
			if(out == NULL){
				fprintf(stderr,"Error generating first child of an ES");
			}
			out->carType = SYNTAX_CAR_TYPE_SYNTAX_NODE;
			out->car = car;
			out->cdr = cdr;
			return out;
		}
		default:
			fprintf(stderr, "Encountered an ES with an unexpected number of children (%d).",node->numChildren);
			return NULL;
	}
}

syntaxnode * makeSyntaxnodeFromE(struct parsenode * node){
	if(node == NULL){
		fprintf(stderr, "Encountered a null parsenode while generating syntax tree.");
		return NULL;
	} else if(!node->isValid){
		fprintf(stderr, "Encountered an invalid parsenode while generating syntax tree.");
		return NULL;
	} else if(node->type != TYPE_E){
		fprintf(stderr, "Expected an E but encountered %s.", getNameOfParseNodeType(node->type));
		return NULL;
	}
	switch(node->numChildren){
		case 1:
			//this E is an atom. Return a syntax node made from this E's child.
		{
			struct parsenode atom = node->children[0];
			if(atom.type != TYPE_ATOM){
				fprintf(stderr, "Expected an atom but encountered %s.", getNameOfParseNodeType(atom.type));
				return NULL;
			}
			return makeSyntaxnodeFromAtom(&atom);
		}
		case 2:
			//This E is a list. Return a syntax node with ES->E as car and ES->ES as cdr
		{ //limits lexical scope of enclosed variables
			struct parsenode e = node->children[0];
			struct parsenode es = node->children[1];
			if(e.type != TYPE_E){
				fprintf(stderr, "Expected an E but encountered %s.", getNameOfParseNodeType(node->type));
				return NULL;
			}
			if(es.type != TYPE_ES){
				fprintf(stderr, "Expected an ES but encountered %s.", getNameOfParseNodeType(node->type));
				return NULL;
			}
			syntaxnode* out = emptySyntaxnodeAlloc();
			syntaxnode* car = makeSyntaxnodeFromE(&e);
			syntaxnode* cdr = makeSyntaxnodeFromEs(&es);
			out->carType = SYNTAX_CAR_TYPE_SYNTAX_NODE;
			out->cdr = cdr;
			out->car = car;
			return out;
		}
		default:
			fprintf(stderr, "Encountered an ES with an unexpected number of children (%d).",node->numChildren);
			return NULL;
	}
}

syntaxnode * makeSyntaxnodeFromAtom(struct parsenode * node){
	if(node == NULL){
		fprintf(stderr, "Encountered a null parsenode while generating syntax tree.");
		return NULL;
	} else if(!node->isValid){
		fprintf(stderr, "Encountered an invalid parsenode while generating syntax tree.");
		return NULL;
	} else if(node->type != TYPE_ATOM){
		fprintf(stderr, "Expected an ATOM but encountered %s.", getNameOfParseNodeType(node->type));
		return NULL;
	} else if(node->numChildren != 0){
		fprintf(stderr, "Expected a parsenode with just a token, got one with %d children.",node->numChildren);
		return NULL;
	} else if(node->tokenPtr == NULL){
		fprintf(stderr, "Expected a parsenode with a token when creating an atom, got none.");
	}
	syntaxnode * out = emptySyntaxnodeAlloc();
	out->atom = node->tokenPtr;
	out->carType = SYNTAX_CAR_TYPE_TOKEN;
	return out;
}