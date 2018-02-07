#pragma once

struct AST_node_t;
typedef struct AST_node_t AST_node_t;

struct _environmentNode;
typedef struct _environmentNode environment_t;

typedef AST_node_t* (PRIMITIVE_FUNCTION)(environment_t*, AST_node_t*);