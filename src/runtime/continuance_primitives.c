//
// Created by JJ on 2/6/2018.
//

#include <assert.h>
#include "continuance_primitives.h"
#include "../types/types.h"

program_state_t primitive_cons(continuance_stack_t *next_c, AST_node_t *unevaluatedArgList) {

}

program_state_t
primitive_cons_intermediate(continuance_stack_t *next_c, AST_node_t *evaluatedCarNode, AST_node_t *unevaluatedCdrNode) {

}

program_state_t
primitive_cons_direct(continuance_stack_t *next_c, AST_node_t *evaluatedCarNode, AST_node_t *evaluatedCdrNode) {

}

program_state_t primitive_car(continuance_stack_t *, AST_node_t *);

NOT_NULL AST_node_t* _car_of_node(NOT_NULL LIST AST_node_t* node){
    assert(node != NULL);
    assert(node->carType == AST_NODE_CAR_TYPE_AST_NODE);
    assert(node->car != NULL);
    return node->car;
}

NOT_NULL AST_node_t* _cdr_of_node(NOT_NULL AST_node_t* node){
    assert(node != NULL);
    assert(node->cdr != NULL);
    return node->cdr;
}

program_state_t primitive_cdr_direct(continuance_stack_t* next_c, AST_node_t* evaluatedArg) {
    return makeProgramState(next_c, _cdr_of_node(evaluatedArg));
}

program_state_t primitive_cdr(continuance_stack_t * next_c, AST_node_t * unevaluatedArgList, environment_t* env) {
    next_c = push_continuance(next_c, allocSimpleContinuance(&primitive_cdr_direct));
    //TODO: finish writing this
}

program_state_t primitive_int_eq(continuance_stack_t *, AST_node_t *, AST_node_t *);

program_state_t primitive_str_eq(continuance_stack_t *, AST_node_t *, AST_node_t *);

program_state_t primitive_char_eq(continuance_stack_t *, AST_node_t *, AST_node_t *);

program_state_t primitive_list_eq(continuance_stack_t *, AST_node_t *, AST_node_t *);

program_state_t primitive_is_nil_direct(continuance_stack_t *next_c, AST_node_t *evaluatedArg) {
    return (isNil(evaluatedArg)) ? makeProgramState(next_c, &AST_NODE_TRUE) : makeProgramState(next_c, &AST_NODE_FALSE);
}

program_state_t primitive_is_nil(continuance_stack_t *next_c, AST_node_t *unevaluatedArgList, environment_t* environment) {
    continuance_t* isNilDirectContinuance = malloc_continuance();
    initializeSimpleContinuance(isNilDirectContinuance, &primitive_is_nil_direct);
    next_c = push_continuance(next_c, isNilDirectContinuance);
    next_c = push_continuance(next_c, make_EVAL_continuance(environment));
    return makeProgramState(next_c, _car_of_node(unevaluatedArgList));
}

program_state_t primitive_eval(continuance_stack_t *, AST_node_t *, environment_t *);

program_state_t primitive_symbol_lookup(continuance_stack_t *, AST_node_t *, environment_t *);

program_state_t primitive_eval_list(continuance_stack_t *, AST_node_t *, environment_t *);

program_state_t primitive_add_binary_list(continuance_stack_t *, AST_node_t *, environment_t *);

program_state_t primitive_print_first_arg(continuance_stack_t *, AST_node_t *); // prints only the first element
