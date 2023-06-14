#ifndef STACK_DEFINED_H
#define STACK_DEFINED_H

#include <stdlib.h>
#include <string.h>

typedef struct{
	char* data;
	char* top;
	size_t alloc_size;
	size_t element_size;
}stack;

stack* stack_new(size_t element_size,size_t init_capacity);

int stack_is_empty(stack* stk);

void stack_clean(stack* stk);

void* stack_pop(stack* stk);

void stack_resize(stack* stk,size_t resize_capacity);

void stack_push(stack* stk,void* value);

#endif
