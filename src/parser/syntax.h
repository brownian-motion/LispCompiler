#pragma once
#include "parser.c"
#include "syntaxnode.h"
#include "parsenode.h"

/**
 * This module is intended to generate a syntax tree, given the root of a valid parse tree.
 */

int  generateSyntaxTree(struct parsenode * , syntaxnode *);
syntaxnode * makeSyntaxnodeFromE(struct parsenode *);
syntaxnode * makeSyntaxnodeFromEs(struct parsenode *);
syntaxnode * makeSyntaxnodeFromAtom(struct parsenode *);
