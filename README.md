# Simple Lisp Compiler
This is a personal project, written in C, to practice skills by writing an interpreter or compiler for a simplified dialect of LISP.

# Language Specification
(Read more at [language specification](docs/language.md))

The language will be very simple to make it easy to implement. Currently it will implement the following functions:
 - Addition (+ a b c ...) - adds all variables a, b, c ...
 - Subtraction (- a b c ...) - subtracts the value of all variables after the first from the first variable. Equivalent to (- a (+ b c ...))

# Status
The compiler is currently capable of tokenizing text input, and generating a parse tree from annotated tokens.
Current progress is under way of generating a syntax tree from a parse tree.
For more details, see the git logs of this project.