#pragma once

#include <assert.h>
#include "../types/syntaxnode.h"
#include "../types/primitiveFunction.h"
#include "environment.h"
#include "eval.h"

/**
 * Defines a collection of primitive macros written in C instead of JLisp,
 * accessible in the global scope of a JLisp program.
 */

/**
 * The primary part of the runtime environment.
 * Simplifies and evaluates an abstract syntax tree into a single value, stored in a syntaxnode*
 * Syntax: (eval <body>)
 * Usage: (eval `(+ 3 4)) -> 7
 */


PRIMITIVE_FUNCTION printASTToStdout;


/**
 * Accepts a list, and returns the front of that list.
 * Syntax: (car <list>)
 * Example usage: (car `(3 4 5)) -> 3
 */
PRIMITIVE_FUNCTION car;

/**
 * Accepts a list, and returns the end of that list.
 * If this is the end of that list, returns nil.
 * Syntax: (cdr <list>)
 * Example usage: (cdr `(3 4 5)) -> `(4 5)
 */
PRIMITIVE_FUNCTION cdr;

/**
 * Accepts a value and a list, then appends the value to the front of the list
 * Syntax: (cons <value> <list>)
 * Example usage: (cons 3 `(4)) -> (3 4)
 */
PRIMITIVE_FUNCTION cons;

/**
 * Returns exactly its argument, and nothing more.
 * Syntax: (quote <arg>)
 * Example usage: (quote (4)) -> (4)
 */
PRIMITIVE_FUNCTION quote;

/**
 * Returns the sum of all of the numbers following it.
 * Syntax: (plus <numbers> ...)
 * Example usage: (plus 1 2 3) -> 6
 */
PRIMITIVE_FUNCTION plus;

/**
 * Defines one or more variables within scope of its body, and returns the result of evaluating that body
 * in the new scope.
 * Syntax: (let ((<key> <val>) ...) <body>)
 * Example usage: (let ((x 2) (y 3)) (+ x y)) -> 5
 */
//PRIMITIVE_FUNCTION let;

/**
 * Creates a lambda expression that can be evaluated with a constant number of arguments.
 * Syntax: (lambda (<args> ...) <body>)
 * Example: ((lambda (x y) (+ x y)) 4 5) -> 9
 */
//PRIMITIVE_FUNCTION lambda;

/**
 * Modifies the given syntaxnode atom, whose car is a token, so that is represents its actual value.
 * Also frees the token ??? //TODO: Decide this
 *
 * Floats get a floatValue and carType _NUMBER.
 * Strings get a stringValue and carType _STRING
 * Identifiers get a carType _IDENTIFIER
 */
void evaluateTokenType(syntaxnode* atomWithUnresolvedToken){
	token t = *atomWithUnresolvedToken->atom;
	//free(atomWithUnresolvedToken->atom);
	if(is_float(t.text)){
		atomWithUnresolvedToken->carType = SYNTAX_CAR_TYPE_NUMBER;
		atomWithUnresolvedToken->floatValue = strtod(t.text,NULL);
	} else if(is_string(t.text)){
		atomWithUnresolvedToken->carType = SYNTAX_CAR_TYPE_STRING;
		atomWithUnresolvedToken->stringValue = t.text;
	} else {
		atomWithUnresolvedToken->carType = SYNTAX_CAR_TYPE_IDENTIFIER;
		atomWithUnresolvedToken->identifier = t.text;
	}
}