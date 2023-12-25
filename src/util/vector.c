#include "vector.h"

#include <stdio.h>

vector* vector_new(size_t element_size,size_t init_capacity){

	vector* new_vector = (vector*)malloc(sizeof(vector));;

	new_vector->element_size = element_size;
	new_vector->alloc_size = init_capacity;
	new_vector->size = 0;
	new_vector->data = malloc(init_capacity * element_size);
	new_vector->callback = NULL;

	return new_vector;
}

void vector_resize(vector* v,size_t resize_capacity){
	v->data = realloc(v->data,resize_capacity * v->element_size);

	v->alloc_size = resize_capacity;
	if(v->size > v->alloc_size)
		v->size = v->alloc_size;
}

void* vector_at(vector* v,size_t index){
	if(v == NULL){
		printf("Vector is null\n");
	}
	if(index >= v->size) return NULL;

	return (char*)v->data + (index * v->element_size);
}

void* vector_at_ptr(vector* v,size_t index){
	void** ptr = (void**)vector_at(v,index);

	if(ptr == NULL) return NULL;

	return *ptr;
}

void vector_set(vector* v,size_t index,void* value){
	void* pointer = vector_at(v,index);
	if(pointer == NULL) return;

	memcpy(pointer,value,v->element_size);
}

void vector_set_n(vector* v,size_t start_index,void* value,size_t n){
	void* pointer = vector_at(v,start_index);
	if(pointer == NULL) return;

	memcpy(pointer,value,v->element_size * n);
}

void* vector_push(vector* v,void* value){
	if(v->size == v->alloc_size){
		vector_resize(v,v->alloc_size * 2);	
	}
	
	v->size++;

	vector_set(v,v->size - 1,value);

	return vector_at(v,v->size - 1);
}

void vector_push_n(vector* v,void* value,size_t n){
	if(v->size + n + 1 >= v->alloc_size)
		vector_resize(v,v->size + n);

	v->size += n;

	vector_set_n(v,v->size - n,value,n);	
}

void vector_foreach(vector* v, void (*callback)(void*)){
	for(size_t i = 0; i < v->size; i++){
		(*callback)((char*)v->data + (i * v->element_size));
	}
}

void vector_free(vector* v){
	if(v->callback)
		vector_foreach(v,v->callback);

	free(v->data);
	free(v);
}

void vector_set_free(vector* v, void (*callback)(void*)){
	v->callback = callback;
}
