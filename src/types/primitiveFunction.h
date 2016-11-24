#pragma once

struct _syntaxnode;
typedef struct _syntaxnode syntaxnode;

struct _environmentNode;
typedef struct _environmentNode environmentNode;

typedef syntaxnode* (PRIMITIVE_FUNCTION)(environmentNode*, syntaxnode*);