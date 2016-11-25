#pragma once

#define DO_PRINT_RUNTIME_TRACE 1

#include "../util/string_util.h"
#include "environment.h"

void convertTokenToFloat(syntaxnode* atomNode);

void convertTokenToPrimitive(syntaxnode* primitiveNode, PRIMITIVE_FUNCTION* primitive);

syntaxnode* lookupIdentifier(environmentNode* scope, syntaxnode* identifierNode);

syntaxnode* eval(environmentNode * enviroment, syntaxnode* listOfArguments);