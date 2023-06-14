#ifndef VECTOR_DEFINED_H
#define VECTOR_DEFINED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	void* data;
	size_t size;
	size_t alloc_size;
	size_t element_size;
}vector;

vector* vector_new(size_t element_size,size_t init_capacity);

void vector_resize(vector* v,size_t resize_capacity);

void* vector_at(vector* v,size_t index);

void* vector_at_ptr(vector* v,size_t index);

void vector_set(vector* v,size_t index,void* value);

void vector_set_n(vector* v,size_t start_index,void* value,size_t n);

void* vector_push(vector* v,void* value);

void vector_push_n(vector* v,void* value,size_t n);

void vector_free(vector* v);

#endif
