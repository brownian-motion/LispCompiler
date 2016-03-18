# Simple Lisp Compiler
This is a personal project, written in C, to practice skills by writing an interpreter or compiler for a simplified dialect of LISP.

# Language Specification
(Read more at [language specification](docs/language.md))

The language will be very simple to make it easy to implement. Currently it will implement the following functions:
 - Addition (+ a b c ...) - adds all variables a, b, c ...
 - Subtraction (- a b c ...) - subtracts the value of all variables after the first from the first variable. Equivalent to (+ a (- b c ...))