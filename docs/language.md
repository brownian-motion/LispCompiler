# Language Overview

The language will be very simple to make it easy to implement. Currently it will implement the following functions:
 - Addition (+ a b c ...) - adds all variables a, b, c ...
 - Subtraction (- a b c ...) - subtracts the value of all variables after the first from the first variable. Equivalent to (- a (+ b c ...))

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
 - .jtoken - annotated list of tokens output by the tokenizer, each on their own line

# Intermediate File Formats

# .jtoken

.jtoken files are produced by the tokenizer and read by the parser.
Each token is on a separate line, which is broken up into three parts:
 - An integer reflecting the type of the token (number, id, etc.)
 - An integer reflecting which line of the source code this token is from
 - A string which is the actual text of the token