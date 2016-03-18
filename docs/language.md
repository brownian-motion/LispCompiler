# Language Overview

The language will be very simple to make it easy to implement. Currently it will implement the following functions:
 - Addition (+ a b c ...) - adds all variables a, b, c ...
 - Subtraction (- a b c ...) - subtracts the value of all variables after the first from the first variable. Equivalent to (+ a (- b c ...))

# Formal Specification

The following products define the language:
 - *Program* -> *E*
 - *E* -> ( *E* *Es* )
 - *E* -> *Atom*
 - *Atom* -> Number
 - *Es* -> *E* *Es*
 - *Es* -> ϵ

Planned additions to the language:
 - *E* -> ( *Function* *Es* )
 - *Function* -> +
 - *Function* -> -

# File Types

 - .jlisp - source code
 - .jtokens - annotated list of tokens output by the tokenizer, each on their own line