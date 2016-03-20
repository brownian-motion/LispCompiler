#pragma once
#include "parser.h"

int main(int argc, char* argv []){
	struct parsenode lparen = makeAtom(getToken());
	struct parsenode one = makeEFromAtom(makeAtom(getToken()));
	struct parsenode two = makeEFromAtom(makeAtom(getToken()));
	struct parsenode three = makeEFromAtom(makeAtom(getToken()));
	struct parsenode rparen = makeAtom(getToken());
	struct parsenode emptyEs = makeEmptyEs();
	struct parsenode lastOne = makeEsFromList(three, emptyEs);
	struct parsenode lastTwo = makeEsFromList(two, lastOne);
	struct parsenode list = makeEFromList(lparen, one, lastTwo, rparen);
	struct parsenode program = makeProgram(list);
	printParseNode(program);
}