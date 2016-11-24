#pragma once
//Intended to be the one thing you have to include to get access to everything
#include "environment.h"
#include "eval.h"
#include "primitives.h"

#include "environment.c"
#include "eval.c"
#include "primitives.c"

environmentNode* getDefaultEnvironment(){
	environmentNode* initialEnvironment = NULL;
	initialEnvironment = definePrimitive(initialEnvironment, "plus", &plus);
	initialEnvironment = definePrimitive(initialEnvironment, "+", &plus);
	initialEnvironment = definePrimitive(initialEnvironment, "minus", &minus);
	initialEnvironment = definePrimitive(initialEnvironment, "-", &minus);
	initialEnvironment = definePrimitive(initialEnvironment, "eval", &eval);
	initialEnvironment = definePrimitive(initialEnvironment, "car", &car);
	initialEnvironment = definePrimitive(initialEnvironment, "cdr", &cdr);
	initialEnvironment = definePrimitive(initialEnvironment, "quote", &quote);
	initialEnvironment = definePrimitive(initialEnvironment, "cons", &cons);
	initialEnvironment = definePrimitive(initialEnvironment, "print_list", &printListToStdout);

	return initialEnvironment;
}