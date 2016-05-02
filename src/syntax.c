#pragma once
#include "syntax.h"

#define DO_PRINT_RESULT_SYNTAX_TREE 1

int main(int argc, char* argv[]){
	int errorcode = 0;
	//First, parse tokens from stdin
	struct parsenode program;
	if((errorcode = buildParseTree(stdin, &program)) != 0){
		puts("Error while parsing.");
		return errorcode;
	}
	puts("Successful parsing!");
	#ifdef DO_PRINT_RESULT_PARSE_TREE
		printParseNode(program);
	#endif

	//Second, generate a syntax tree from the given parse tree
	syntaxnode syntaxtree;
	if((errorcode = generateSyntaxTree(&program, &syntaxtree)) != 0){
		puts("Error while generating syntax tree.");
		return errorcode;
	}
	puts("\nSuccessful syntax tree generation!");
	#ifdef DO_PRINT_RESULT_SYNTAX_TREE 
		printSyntaxnode(&syntaxtree); 
	#endif

	return errorcode;
}

int generateSyntaxTree(struct parsenode * parseTree, syntaxnode * syntaxTree){
	if(parseTree == NULL){
		fprintf(stderr, "Encountered a null parsenode while generating syntax tree.");
		return -4;
	} else if(!parseTree->isValid){
		fprintf(stderr, "Encountered an invalid parsenode while generating syntax tree.");
		return -3;
	} else if(parseTree->type != TYPE_PROGRAM){
		fprintf(stderr, "Expected an PROGRAM but encountered %s.", getNameOfType(parseTree->type));
		return -2;
	} else if(parseTree->numChildren != 1){
		fprintf(stderr, "Program node doesn't have exactly 1 child.");
		return -1;
	}
	struct parsenode e = parseTree->children[0];
	syntaxnode * root = makeSyntaxnodeFromE(&e);
	*syntaxTree = *root;
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
		fprintf(stderr, "Expected an ES but encountered %s.", getNameOfType(node->type));
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
				fprintf(stderr, "Expected an E but encountered %s.", getNameOfType(node->type));
				return NULL;
			}
			if(es.type != TYPE_ES){
				fprintf(stderr, "Expected an E but encountered %s.", getNameOfType(node->type));
				return NULL;
			}
			syntaxnode* out = makeSyntaxnodeFromE(&e);
			syntaxnode* cdr = makeSyntaxnodeFromEs(&es);
			if(out == NULL){
				fprintf(stderr,"Error generating first child of an ES");
			}
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
		fprintf(stderr, "Expected an E but encountered %s.", getNameOfType(node->type));
		return NULL;
	}
	switch(node->numChildren){
		case 1:
			//this E is an atom. Return a syntax node made from this E's child.
		{
			struct parsenode atom = node->children[0];
			if(atom.type != TYPE_ATOM){
				fprintf(stderr, "Expected an atom but encountered %s.", getNameOfType(atom.type));
				return NULL;
			}
			return makeSyntaxnodeFromAtom(&atom);
		}
		case 2:
			//This E is a list. Return a syntax node with ES->E as car and ES->ES as ES->E->cdr, and cdr as null
		{ //limits lexical scope of enclosed variables
			struct parsenode e = node->children[0];
			struct parsenode es = node->children[1];
			if(e.type != TYPE_E){
				fprintf(stderr, "Expected an E but encountered %s.", getNameOfType(node->type));
				return NULL;
			}
			if(es.type != TYPE_ES){
				fprintf(stderr, "Expected an ES but encountered %s.", getNameOfType(node->type));
				return NULL;
			}
			syntaxnode* out = emptySyntaxnodeAlloc();
			syntaxnode* car = makeSyntaxnodeFromE(&e);
			syntaxnode* cdr = makeSyntaxnodeFromEs(&es);
			car->cdr = cdr;
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
		fprintf(stderr, "Expected an ATOM but encountered %s.", getNameOfType(node->type));
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