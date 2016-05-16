# Simple Lisp Compiler
This is a personal project, written in C, to practice skills by writing an interpreter for a simplified dialect of Lisp.

# Language Specification
(Read more at [language specification](docs/language.md))

The language will be very simple to make it easy to implement. Currently it will implement the following functions:
 - Addition (+ a b c ...) - adds all variables a, b, c ...
 - Subtraction (- a b c ...) - subtracts the value of all variables after the first from the first variable. Equivalent to (- a (+ b c ...))
 - 
# Examples


# Compiling source files
I prefer to use [Microsoft Visual Studio](https://www.visualstudio.com/en-us/visual-studio-homepage-vs.aspx)'s [command line C compiler, cl](https://msdn.microsoft.com/en-us/library/f35ctcxw.aspx). Any C compiler that builds to the C runtime environment should do the trick.

- To compile the tokenizer, compile [tokenizer.c](src/tokenizer.c)
- To compile the standalone parser, compile [parser.c](src/main/parser.c)
- To compile the syntax tree generator (which includes the parser), compile [syntax.c](src/syntax.c)

For example, `cl src/tokenizer.c` would compile the tokenizer and create `tokenizer.exe` in the current directory.

# Using the executable interpreter components
(Read more at [examples](examples/)
Several examples are included in this repository under the [examples](examples/) directory. To use them, you must specify input and output files on the command line to the desired component of the interpreter. For example, on Windows 10, using `cmd`:

    tokenizer.exe < "examples/parser_test.jlisp" > "examples/parser_test.jtoken"
    
Will instruct the tokenizer to parse [examples/parser_test.jlisp](examples/parser_test.jlisp) and print the result tokens in [examples/parser_test.jtoken](examples/parser_test.jtoken).

# Status
The compiler is currently capable of tokenizing text input, generating a parse tree from annotated tokens (stored in a .jtoken file), and generating a syntax tree.
Current progress is under way to establish a runtime environment.
For more details, see the git logs of this project.
