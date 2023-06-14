#include "stack.h"

stack* stack_new(size_t element_size,size_t init_capacity){
	stack* new_stack = (stack*)malloc(sizeof(stack));

	new_stack->element_size = element_size;
	new_stack->alloc_size = init_capacity;
	new_stack->data = (char*)malloc(init_capacity * element_size);
	new_stack->top = new_stack->data - element_size;
	return new_stack;
}

int stack_is_empty(stack* stk){
	return stk->top < stk->data;
}

void stack_clean(stack* stk){
	stk->top = stk->data - stk->element_size;
}

void* stack_pop(stack* stk){
	if(stack_is_empty(stk))
		return NULL;

	void* prev_top = stk->top;

	stk->top -= stk->element_size;

	return prev_top;
}

void stack_resize(stack* stk,size_t resize_capacity){
	int offset = stk->top - stk->data;
	stk->data = (char*)realloc(stk->data,resize_capacity * stk->element_size);

	stk->alloc_size = resize_capacity;
	stk->top = stk->data + offset;
}

void stack_push(stack* stk,void* value){
	if((stk->top - stk->data) == stk->alloc_size){
		stack_resize(stk,stk->alloc_size * 2);
	}

	stk->top += stk->element_size;

	memcpy(stk->top,value,stk->element_size);
}

