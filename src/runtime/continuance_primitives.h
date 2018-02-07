//
// Created by JJ on 2/6/2018.
//

#ifndef LISPCOMPILER_CONTINUANCES_H
#define LISPCOMPILER_CONTINUANCES_H

#include "continuance.h"


program_state_t primitive_cons(continuance_stack_t *, AST_node_t *, environment_t*);
program_state_t primitive_car(continuance_stack_t *, AST_node_t *, environment_t*);
program_state_t primitive_cdr(continuance_stack_t *, AST_node_t *, environment_t*);
program_state_t primitive_int_eq(continuance_stack_t *, AST_node_t *, AST_node_t*);
program_state_t primitive_str_eq(continuance_stack_t *, AST_node_t *, AST_node_t*);
program_state_t primitive_char_eq(continuance_stack_t *, AST_node_t *, AST_node_t*);
program_state_t primitive_list_eq(continuance_stack_t*, AST_node_t*, AST_node_t*);
program_state_t primitive_is_nil(continuance_stack_t*, AST_node_t*, environment_t*);
program_state_t primitive_eval(continuance_stack_t*, AST_node_t*, environment_t*);
program_state_t primitive_symbol_lookup(continuance_stack_t*, AST_node_t*, environment_t*);
program_state_t primitive_eval_list(continuance_stack_t*, AST_node_t*, environment_t*);
program_state_t primitive_add_binary_list(continuance_stack_t*, AST_node_t*, environment_t*);
program_state_t primitive_print_first_arg(continuance_stack_t*, AST_node_t*); // prints only the first element


#endif //LISPCOMPILER_CONTINUANCES_H
