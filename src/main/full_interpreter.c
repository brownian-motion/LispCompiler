#pragma once

#include "../types/types.h"	
#include "../parser/syntax.c"
#include "../runtime/runtime.h"

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
		putc('\n', stdout);
		printSyntaxnode(&syntaxtree); 
		putc('\n', stdout);
	#endif

	//Finally, evaluate the syntax tree and print it out
	//set up the environment
	environmentNode* initialEnvironment = getDefaultEnvironment();

	syntaxnode* evaluatedProgram = eval(initialEnvironment, &syntaxtree);
	puts("\nSuccessful program evaluation!\n\tResult: ");
	printSyntaxnode(evaluatedProgram);

	return errorcode;
}