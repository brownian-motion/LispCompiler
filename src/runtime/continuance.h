//
// Created by JJ on 2/6/2018.
//

#ifndef LISPCOMPILER_CONTINUANCE_H
#define LISPCOMPILER_CONTINUANCE_H

#include "../types/ASTnode.h"
#include "../types/environmentNode.h"
#include "../util/annotations.h"
#include <stdlib.h>

//forward-declared so that these names resolve in the function pointers
struct continuance_stack_t;
struct program_state_t;

typedef struct program_state_t (*SIMPLE_UNARY_PRIMITIVE)(struct continuance_stack_t *, AST_node_t *);
typedef struct program_state_t (*UNARY_PRIMITIVE_WITH_ENV)(struct continuance_stack_t *, AST_node_t *, environment_t*);
typedef struct program_state_t (*BINARY_PRIMITIVE)(struct continuance_stack_t *, AST_node_t *, AST_node_t*);

typedef enum continuance_type_t {
    CONTINUANCE_TYPE_SIMPLE, CONTINUANCE_TYPE_ENV_EXTRA, CONTINUANCE_TYPE_AST_EXTRA
}continuance_type_t;

typedef struct continuance_t {
    union {
        SIMPLE_UNARY_PRIMITIVE unaryPrimitive;

        UNARY_PRIMITIVE_WITH_ENV unaryPrimitiveWithEnv;

        BINARY_PRIMITIVE binaryPrimitive;
    };

    AST_node_t *astNodeExtra;
    environment_t *environmentExtra;

    continuance_type_t type;
} continuance_t;

continuance_t *malloc_continuance() {
    return (continuance_t *) malloc(sizeof(continuance_t));
}

void initializeSimpleContinuance(NOT_NULL continuance_t* continuance, NOT_NULL SIMPLE_UNARY_PRIMITIVE simpleUnaryPrimitive){
    continuance->unaryPrimitive = simpleUnaryPrimitive;
    continuance->type = CONTINUANCE_TYPE_SIMPLE;
}

continuance_t* allocSimpleContinuance(NOT_NULL SIMPLE_UNARY_PRIMITIVE simpleUnaryPrimitive){
    continuance_t* out = malloc_continuance();
    initializeSimpleContinuance(out, simpleUnaryPrimitive);
}

void initializeContinuanceWithEnvironment(continuance_t* continuance, UNARY_PRIMITIVE_WITH_ENV unaryPrimitiveWithEnv, environment_t* environment){
    continuance->unaryPrimitiveWithEnv = unaryPrimitiveWithEnv;
    continuance->environmentExtra = environment;
    continuance->type = CONTINUANCE_TYPE_ENV_EXTRA;
}

void initializeContinuanceWithExtraASTNode(continuance_t* continuance, BINARY_PRIMITIVE binaryPrimitive, AST_node_t* extraASTNode){
    continuance->binaryPrimitive = binaryPrimitive;
    continuance->astNodeExtra = extraASTNode;
    continuance->type = CONTINUANCE_TYPE_AST_EXTRA;
}

typedef struct continuance_stack_t {
    struct continuance_stack_t *next;
    continuance_t *continuance;
} continuance_stack_t;

continuance_stack_t *malloc_continuance_stack() {
    return (continuance_stack_t *) malloc(sizeof(continuance_stack_t));
}

continuance_stack_t *push_continuance(continuance_stack_t *curr_stack, continuance_t *next_continuance) {
    continuance_stack_t *newStack = malloc_continuance_stack();
    newStack->next = curr_stack;
    newStack->continuance = next_continuance;
    return newStack;
}

typedef struct program_state_t {
    continuance_stack_t *nextContinuance;
    const AST_node_t *nextArg;
} program_state_t;

#define makeProgramState(nextContinuance, nextArg) ((program_state_t) {nextContinuance, nextArg})

//TODO: DELETE the continuance stack as continuances are consumed!

//forward-declare some continuances that are used later
continuance_t *make_CONS_continuance(AST_node_t *cdr);

continuance_t *CAR_continuance;
continuance_t *CDR_continuance;

continuance_t *make_INT_EQ_continuance(AST_node_t *intAtom);

continuance_t *make_STR_EQ_continuance(AST_node_t *strAtom);

continuance_t *make_CHAR_EQ_continuance(AST_node_t *charAtom);

continuance_t *make_LIST_EQ_continuance(AST_node_t *otherList);

continuance_t *IS_NIL_continuance;

continuance_t *make_EVAL_continuance(environment_t *env);

continuance_t *make_TOKEN_LOOKUP_continuance(environment_t *env);

continuance_t *make_EVAL_LIST_continuance(environment_t *env);

continuance_t *make_ADD_INT_continuance(AST_node_t *intAtomOperand);

continuance_t *PRINT_TO_STDOUT_continuance;


#endif //LISPCOMPILER_CONTINUANCE_H
