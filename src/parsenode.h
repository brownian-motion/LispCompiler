#pragma once
#include "token.h"

#define TYPE_ATOM 1
#define TYPE_E 2
#define TYPE_ES 3
#define TYPE_PROGRAM 4

//If numChildren is 0, this is assumed to have children NULL and be a terminator if tokenPtr is non-NULL, or represent an empty string if tokenPtr is NULL.
//Otherwise, it is assumed that tokenPtr is NULL and children is not.
struct parsenode{
	int type;
	int numChildren;
	struct parsenode* children; //an array of parsenode structs
	token* tokenPtr;
	int isValid;
};

struct parsenode* parsenodeAlloc(int num){
	return (struct parsenode*) malloc(num * sizeof(struct parsenode));
}

char* getNameOfType(int type){
	switch(type){
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

void fprintParseNode(FILE * file, struct parsenode node){
	fprintf(file, "{type:%s(%d), ",getNameOfType(node.type),node.type);
	if(!node.isValid){
		fprintf(file, "INVALID}");
		return;
	}
	if(node.numChildren == 0){
		fprintf(file, "token:\"");
		if(node.tokenPtr == NULL)
			fprintf(file, "NULL_TOKEN");
		else
			fprintTokenText(file,*(node.tokenPtr));
		fprintf(file, "\"}");
	} else {
		fprintf(file, "children: (");
		for(int i = 0 ; i < node.numChildren; i++){
			if(i != 0){
				fprintf(file,", ");
			}
			fprintParseNode(file,node.children[i]);
		}
		fprintf(file,")}");
	}
}

void printParseNode(struct parsenode node){
	fprintParseNode(stdout, node);
}

struct parsenode makeAtom(token t){
	token * ptr = tokenAlloc(1);
	*ptr = t;
	struct parsenode out = {TYPE_ATOM, 0, NULL, ptr, 1};
	return out;
}

struct parsenode makeEmptyEs(){
	struct parsenode out = {TYPE_ES, 0, NULL, NULL, 1};
	return out;
}

int canMakeEFromAtom(struct parsenode atom){
	return atom.isValid && (atom.tokenPtr->type == TYPE_TOKEN_NUMBER || atom.tokenPtr->type == TYPE_TOKEN_ID);
}

struct parsenode makeEFromAtom(struct parsenode atom){
	int isValid = (atom.type == TYPE_ATOM) && atom.isValid;
	struct parsenode* children = parsenodeAlloc(1);
	children[0] = atom;
	struct parsenode out = {TYPE_E, 1, children, NULL, isValid};
	return out;
}

int canMakeEFromList(struct parsenode lparen, struct parsenode e, struct parsenode es, struct parsenode rparen){
	return (lparen.type == TYPE_ATOM && lparen.tokenPtr->type == TYPE_TOKEN_LPAREN)
		&& (e.type == TYPE_E)
		&& (es.type == TYPE_ES)
		&& (rparen.type == TYPE_ATOM && rparen.tokenPtr->type == TYPE_TOKEN_RPAREN)
		&& lparen.isValid && e.isValid && es.isValid && rparen.isValid;
}

struct parsenode makeEFromList(struct parsenode lparen, struct parsenode e, struct parsenode es, struct parsenode rparen){
	int isValid = canMakeEFromList(lparen,e,es,rparen);
	struct parsenode* children = parsenodeAlloc(4);
	children[0] = lparen;
	children[1] = e;
	children[2] = es;
	children[3] = rparen;
	struct parsenode out = {TYPE_E, 4, children, NULL, isValid};
	return out;
}

struct parsenode makeEsFromList(struct parsenode e, struct parsenode es){
	int isValid = ((e.type == TYPE_E) && e.isValid)
		&& ((es.type == TYPE_ES) && e.isValid);
	struct parsenode * children = parsenodeAlloc(2);
	children[0] = e;
	children[1] = es;
	struct parsenode out = {TYPE_ES, 2, children, NULL, isValid};
	return out;
}

struct parsenode makeProgram(struct parsenode e){
	struct parsenode* children = parsenodeAlloc(1);
	children[0] = e;
	struct parsenode out = {TYPE_PROGRAM, 1, children, NULL, e.isValid};
	return out;
}