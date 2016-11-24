#pragma once

#include "../parser/tokenizer.c"

int main(int argc, char* argv[]){
	if(argc == 2 && (strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"/?") == 0) ){
		puts("Usage: tokenizer < inputfile.jlisp");
		return 0;
	}
	//assume that the stdin now refers to a valid input file

	tokenizer stdinTokenizer = makeTokenizerFromFile(stdin);

	//print these tokens to stdout as though it were a .jtoken file
	while(hasNextToken(&stdinTokenizer)){
		token * newToken = getNextToken(&stdinTokenizer);
		printToken(*newToken);
	}
	return 0;
}