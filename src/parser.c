#pragma once
#include "parser.h"

#define DO_DEBUG_STACK 0
#define DO_PRINT_PARSE_ERRORS 1
#define DO_PRINT_RESULT_PARSE_TREE 1

/**
 * Generates a parse tree of the given source code file, assumed to represent a valid .jlisp file.
 * On success, returns 0 and places the generated PROGRAM parsenode into *output.
 * On failure, returns an error code and does not modify *output.
 *
 * Internally, this creates a lot of token objects. It does not free the resources of these,
 * because every token is unique (that is, each bit of text it represents is not stored twice).
 * When freeing the resources of the parse tree generated, these tokens must individually be freed.
 */
int buildParseTree(FILE * file, struct parsenode * output){
	struct parseStack stack = makeParseStack();
	tokenizer fileTokenizer = makeTokenizerFromFile(file); //stack allocated
	token * lookAhead = getNextToken(&fileTokenizer); //never null, might represent EOF
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
		if(lookAhead->type == TYPE_TOKEN_ERROR){
			#ifdef DO_PRINT_PARSE_ERRORS
				printf("Parse error #%3d: Line #%3d, Col %3d: Invalid token encountered: \"%s\".\n", PARSE_ERROR_INVALID_TOKEN, lookAhead->lineNumber, lookAhead->colNumber, lookAhead->text);
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
						if(atom.tokenPtr->type == TYPE_TOKEN_RPAREN && size(stack) >= 3){
							struct parsenode es = pop(&stack);
							struct parsenode e = pop(&stack);
							struct parsenode lparen = pop(&stack);
							if(canMakeEFromList(lparen,e,es,atom))
								push(&stack, makeEFromList(lparen, e, es, atom));
							else{
								#ifdef DO_PRINT_PARSE_ERRORS
									fprintf(stderr, "Parse error #%3d: Line %3d, Col %3d: Syntax error. Cannot reduce series into list: ",PARSE_ERROR_SYNTAX_ERROR,atom.tokenPtr->lineNumber, atom.tokenPtr->colNumber);
									fprintParseNode(stderr, lparen);
									fprintParseNode(stderr, e);
									fprintParseNode(stderr, es);
									fprintParseNode(stderr, atom);
									fprintf(stderr, "\n");
								#endif
								return PARSE_ERROR_SYNTAX_ERROR;
							}
						} else if(canMakeEFromAtom(atom)){
							push(&stack, makeEFromAtom(atom));
						} else {
							#ifdef DO_PRINT_PARSE_ERRORS
								fprintf(stderr, "Parse error #%3d: Line %3d, Col %3d: Unexpected token %s.\n", PARSE_ERROR_SYNTAX_ERROR, atom.tokenPtr->lineNumber, atom.tokenPtr->colNumber, atom.tokenPtr->text);
							#endif
							return PARSE_ERROR_SYNTAX_ERROR;
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
						if(lookAhead == NULL || lookAhead->type == TYPE_TOKEN_EOF)
							push(&stack, makeProgram(pop(&stack)));
						else
							push(&stack, makeEmptyEs());
						break;
						#ifdef DO_PRINT_PARSE_ERRORS
							fprintf(stderr,"Parse error #%3d: Line %3d, Col %3d: Tried to reduce a program parse node: ",PARSE_ERROR_PROGRAM_NODE_REDUCED, lookAhead->lineNumber, lookAhead->colNumber);
							fprintParseNode(stderr,peek(&stack));
							fprintf(stderr,"\n");
						#endif
						return PARSE_ERROR_PROGRAM_NODE_REDUCED;
					default:
						#ifdef DO_PRINT_PARSE_ERRORS
							fprintf(stderr,"Parse error #%3d: Line %3d, Col %3d: Tried to reduce a parse node of unknown type: ",PARSE_ERROR_UNKNOWN_NODE_REDUCED, lookAhead->lineNumber, lookAhead->colNumber);
							fprintParseNode(stderr, peek(&stack));
							fprintf(stderr,"\n");
						#endif
						return PARSE_ERROR_UNKNOWN_NODE_REDUCED;
				}
				break;
			case 0:
				//then shift
				#ifdef DO_DEBUG_STACK
					printf(" (shifting)\n");
				#endif
				if(lookAhead == NULL || lookAhead->type == TYPE_TOKEN_EOF){
					#ifdef DO_PRINT_PARSE_ERRORS
						fprintf(stderr, "Parse error #%3d: Unexpected end of file.",PARSE_ERROR_EARLY_EOF);
					#endif
					return PARSE_ERROR_EARLY_EOF;
				}
				push(&stack,makeAtom(lookAhead)); //relinquises control of the token to the atom
				lookAhead = getNextToken(&fileTokenizer);
				break;
			default:
				#ifdef DO_PRINT_PARSE_ERRORS
					fprintf(stderr, "Parse error #%3d: Line %3d, Col %3d: Syntax error :\"%s\"\n.",PARSE_ERROR_SYNTAX_ERROR, lookAhead->lineNumber, lookAhead->colNumber, lookAhead->text);
				#endif
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
int shouldReduce(struct parseStack stack, token * lookAhead){
	if(isEmpty(stack))
		return 0;
	switch(peek(&stack).type){
		case TYPE_PROGRAM:
			fputs("Somehow tried to check if the parser should reduce an entire program.",stderr);
			return 0;
		case TYPE_E:
			//If the upcoming token is a right parentheses, we need to reduce by putting an empty Es onto the stack and doing nothing else
			//Otherwise, we need more tokens
			return lookAhead->type == TYPE_TOKEN_RPAREN || lookAhead->type == TYPE_TOKEN_EOF;
		case TYPE_ES:
			assert(lookAhead->type == TYPE_TOKEN_RPAREN);
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