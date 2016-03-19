#pragma once
#include "parsenode.h"
#define DEFAULT_STACK_SIZE 10

struct parseStackNode{
	struct parsenode data;
	struct parseStackNode* next;
};

struct parseStackNode * parseStackNodeAlloc(){
	return (struct parseStackNode *) malloc(sizeof(struct parseStackNode));
}

struct parseStack{
	struct parseStackNode* top;
};

struct parseStack makeParseStack(){
	struct parseStack out = {NULL};
	return out; 
}

int isEmpty(struct parseStack stack){
	return stack.top == NULL;
}

void push(struct parseStack* stack, struct parsenode node){
	struct parseStackNode* next = stack->top;
	stack->top = parseStackNodeAlloc();
	stack->top->data = node;
	stack->top->next = next;
}

struct parsenode pop(struct parseStack* stack){
	struct parsenode out = stack->top->data;
	struct parseStackNode* next = stack->top->next;
	free(stack->top);
	stack->top=next;
	return out;
}

struct parsenode peek(struct parseStack* stack){
	return stack->top->data;
}

int stackSize(struct parseStackNode* node){
	(node == NULL)?0:(1+stackSize(node->next));
}

int size(struct parseStack stack){
	return stackSize(stack.top);
}