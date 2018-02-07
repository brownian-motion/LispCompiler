#pragma once

#define DO_PRINT_RUNTIME_TRACE 1

#include "../util/string_util.h"
#include "environment.h"

void convertTokenToFloat(AST_node_t* atomNode);

void convertTokenToPrimitive(AST_node_t* primitiveNode, PRIMITIVE_FUNCTION* primitive);

AST_node_t* lookupIdentifier(environment_t* scope, AST_node_t* identifierNode);

AST_node_t* eval(environment_t * enviroment, AST_node_t* listOfArguments);