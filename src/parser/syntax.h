#pragma once
#include "parser.c"
#include "../types/ASTnode.h"
#include "parsenode.h"

/**
 * This module is intended to generate a syntax tree, given the root of a valid parse tree.
 */

int  generateAST(struct parsenode_t *, AST_node_t *);
AST_node_t * makeSyntaxnodeFromE(struct parsenode_t *);
AST_node_t * makeSyntaxnodeFromEs(struct parsenode_t *);
AST_node_t * makeSyntaxnodeFromAtom(struct parsenode_t *);
