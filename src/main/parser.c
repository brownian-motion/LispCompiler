#pragma once

#include "../parser.c"

int main(int argc, char* argv []){
	struct parsenode program;
	if(buildParseTree(stdin, &program) == 0){
		puts("Successful parsing!");
		#ifdef DO_PRINT_RESULT_PARSE_TREE
			printParseNode(program);
		#endif
		return 0;
	}
	return -1;
}