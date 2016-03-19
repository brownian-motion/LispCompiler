#pragma once
#include "parser.h"

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
	struct parsenode program = buildProgram(stdin);
	printParseNode(program);
	return 0;
}

struct parsenode buildProgram(FILE * file){
	struct parseStack stack = makeParseStack();
	token lookAhead = getToken();

	while(isEmpty(stack) || peek(&stack).type != TYPE_PROGRAM){
		if(shouldReduce(stack, lookAhead)){
			//then reduce
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
				case TYPE_PROGRAM:
				default:
					fputs("FATAL: Tried to reduce a program or an unkown parsenode.",stderr);
					assert(0); //fatal error!
			}
		} else {
			//then shift
			push(&stack,makeAtom(lookAhead));
			lookAhead = getToken();
		}
	}

	return pop(&stack);
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
			return lookAhead.type == TYPE_TOKEN_RPAREN;
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