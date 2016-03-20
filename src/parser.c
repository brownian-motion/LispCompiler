#pragma once
#include "parser.h"

//#define DO_DEBUG_PARSE 1
#define DO_PRINT_PARSE_ERRORS 1
#define DO_PRINT_RESULT_PARSE_TREE 1

int main(int argc, char* argv []){
	// struct parsenode lparen = makeAtom(getToken());
	// struct parsenode one = makeEFromAtom(makeAtom(getToken()));
	// struct parsenode two = makeEFromAtom(makeAtom(getToken()));
	// struct parsenode three = makeEFromAtom(makeAtom(getToken()));
	// struct parsenode rparen = makeAtom(getToken());
	// struct parsenode emptyEs = makeEmptyEs();
	// struct parsenode lastOne = makeEsFromList(three, emptyEs);
	// struct parsenode lastTwo = makeEsFromList(two, lastOne);
	// struct parsenode list = makeEFromList(lparen, one, lastTwo, rparen);
	// struct parsenode program = makeProgram(list);
	struct parsenode program;
	if(buildParseTree(stdin, &program) == 0){
		puts("Successful parsing!");
		#ifdef DO_PRINT_RESULT_PARSE_TREE
			printParseNode(program);
		#endif
		return 0;
	}
}

/**
 * Generates a parse tree of the given file, assumed to represent a valid .jtoken file.
 * On success, returns 0 and places the generated PROGRAM parsenode into *output.
 * On failure, returns an error code and does not modify *output.
 */
int buildParseTree(FILE * file, struct parsenode * output){
	struct parseStack stack = makeParseStack();
	token lookAhead = fgetToken(file);
	int numIterations = 0;

	while(isEmpty(stack) || peek(&stack).type != TYPE_PROGRAM){
		#ifdef DO_DEBUG_STACK
			printf("#%3d: ",numIterations++);
			if(isEmpty(stack)){
				printf("<EMPTY STACK>");
			} else {
				//print the top of the stack
				printf("Stack size %3d. Top of stack: ",size(stack));
				printParseNode(peek(&stack));
			}
			printf(" Look ahead: ");
			printTokenDebug(lookAhead);
		#endif
		int shouldReduceResult;
		if(lookAhead.type == TYPE_TOKEN_ERROR){
			#ifdef DO_PRINT_PARSE_ERRORS
				printf("Parse error #%3d: Invalid token encountered.", PARSE_ERROR_INVALID_TOKEN);
			#endif
			return PARSE_ERROR_INVALID_TOKEN;
		}
		switch(shouldReduceResult = shouldReduce(stack, lookAhead)){
			case 1:
				//then reduce
				#ifdef DO_DEBUG_STACK
					printf(" (reducing)\n");
				#endif
				switch(peek(&stack).type){
					case TYPE_ATOM:
						struct parsenode atom = pop(&stack);
						if(atom.tokenPtr->type == TYPE_TOKEN_RPAREN){
							struct parsenode es = pop(&stack);
							struct parsenode e = pop(&stack);
							struct parsenode lparen = pop(&stack);
							push(&stack, makeEFromList(lparen, e, es, atom));
						} else {
							push(&stack, makeEFromAtom(atom));
						}
						break;
					case TYPE_ES:
						//hit an rparen. Should make an ES from an E and ES
						struct parsenode es = pop(&stack);
						struct parsenode e = pop(&stack);
						push(&stack, makeEsFromList(e, es));
						break;
					case TYPE_E:
						//hit an rparen, and should make an empty ES,
						//or hit end of file and should make program
						if(lookAhead.type == TYPE_TOKEN_EOF)
							push(&stack, makeProgram(pop(&stack)));
						else
							push(&stack, makeEmptyEs());
						break;
						#ifdef DO_PRINT_PARSE_ERRORS
							fprintf(stderr,"Parse error #%3d: Tried to reduce a program parse node.",PARSE_ERROR_PROGRAM_NODE_REDUCED);
						#endif
						return PARSE_ERROR_PROGRAM_NODE_REDUCED;
					default:
						#ifdef DO_PRINT_PARSE_ERRORS
							fprintf(stderr,"Parse error#%3d: Tried to reduce a parse node of unknown type.",PARSE_ERROR_UNKNOWN_NODE_REDUCED);
						#endif
						return PARSE_ERROR_UNKNOWN_NODE_REDUCED;
				}
				break;
			case 0:
				//then shift
				#ifdef DO_DEBUG_STACK
					printf(" (shifting)\n");
				#endif
				if(lookAhead.type == TYPE_TOKEN_EOF){
					return PARSE_ERROR_EARLY_EOF;
				}
				push(&stack,makeAtom(lookAhead));
				lookAhead = fgetToken(file);
				break;
			default:
				return PARSE_ERROR_SYNTAX_ERROR;
		}
	}
	if(size(stack) != 1){
		#ifdef DO_PRINT_PARSE_ERRORS
			fprintf(stderr,"Parse error#%3d: Source could not be reduced to a complete program.",PARSE_ERROR_NON_EMPTY_STACK);
		#endif
		return PARSE_ERROR_NON_EMPTY_STACK;
	}
	*output = pop(&stack);
	return PARSE_SUCCESS;
}

/**
 * This takes in a stack of parsenodes, and determines
 * (without modifying the stack) if the parser should reduce the stack
 * without consuming more tokens.
 */
int shouldReduce(struct parseStack stack, token lookAhead){
	if(isEmpty(stack))
		return 0;
	switch(peek(&stack).type){
		case TYPE_PROGRAM:
			fputs("Somehow tried to check if the parser should reduce an entire program.",stderr);
			return 0;
		case TYPE_E:
			//If the upcoming token is a right parentheses, we need to reduce by putting an empty Es onto the stack and doing nothing else
			//Otherwise, we need more tokens
			return lookAhead.type == TYPE_TOKEN_RPAREN || lookAhead.type == TYPE_TOKEN_EOF;
		case TYPE_ES:
			assert(lookAhead.type == TYPE_TOKEN_RPAREN);
			//If the stack currently is ... atom(LPAREN) E Es, then we need to shift in order to get the right paren we need to reduce
			//Otherwise, we don't need to reduce
			return size(stack)>= 3 && (stack.top->next->next->data.type != TYPE_ATOM || stack.top->next->next->data.tokenPtr->type != TYPE_TOKEN_LPAREN);
		case TYPE_ATOM:
			//If this atom is a left parentheses, then we need more tokens
			//If this atom is a right parentheses, then we need to reduce by the rule E -> ( E ES )
			//If this atom is a number, then we need to reduce by the rule E->ATOM
			//If this atom is an id, then we need to reduce by the rule E->ATOM
			return peek(&stack).tokenPtr->type != TYPE_TOKEN_LPAREN;
		default:
			fputs("Somehow tried to check if the parser should reduce an unknown type of parse node.",stderr);
			return 0;
	}
}

/*
int isTokenInPredictSet(struct parsenode node, token t){
	switch(node.type){
		case TYPE_PROGRAM:
			fputs(stderr,"Somehow checked the predict set for a program");
			return 0; //HOW WOULD THIS EVEN HAPPEN?
		case TYPE_E:
			switch(t.type){
				case TYPE_TOKEN_NUMBER:
					return 1;
				case TYPE_TOKEN_ID:
					return 1;
				case TYPE_TOKEN_LPAREN:
					return 1;
				case TYPE_TOKEN_RPAREN:
					return 1;
			}
		case TYPE_ES:
			switch(t.type){
				case TYPE_TOKEN_NUMBER:

				case TYPE_TOKEN_ID:

				case TYPE_TOKEN_LPAREN:
					return 0;
				case TYPE_TOKEN_RPAREN:
			}
		case TYPE_ATOM:
			switch(t.type){

			}
		case default:
			fprintf(stderr,"Somehow checked the predict set for unidentified state %d\n",node.type);
			return 0;
	}
}*/