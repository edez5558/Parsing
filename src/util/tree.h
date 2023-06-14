#ifndef TREE_INCLUDED_DEFINE
#define TREE_INCLUDED_DEFINE

#define lambda(lambda$_ret, lambda$_args, lambda$_body)\
	({\
		lambda$_ret lambda$__anon$ lambda$_args\
			lambda$_body\
		$lambda$__anon$;\
	 })

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct node_struct{
	struct node_struct* left;	
	struct node_struct* right;
	void* value;
}node;

typedef struct{
	size_t size;
	
	node* root;

	int (*cmp)(void*,void*);
	void (*free)(void*);
}btree;

btree* btree_new(size_t element_size){
	btree* new_btree = (btree*)malloc(sizeof(btree));
	
	new_btree->root = NULL;
	new_btree->cmp = NULL;
	new_btree->free = NULL;
	new_btree->size = element_size;

	return new_btree;
}

void btree_set_cmp(btree* tr, int (*cmp)(void*,void*)){
	tr->cmp = cmp;
}

void btree_set_free(btree* tr, void (*free)(void*)){
	tr->free = free;
}

node* node_new(){
	node* new_node = (node*)malloc(sizeof(node));

	new_node->value = NULL;
	new_node->left = NULL;
	new_node->right = NULL;

	return new_node;
}

int node_left_height(node* nd,int height){
	if(nd == NULL)
		return height;

	return node_left_height(nd->left,height + 1);
}

int node_right_height(node* nd,int height){
	if(nd == NULL)
		return height;

	return node_right_height(nd->right,height + 1);
}

void node_set_value(node* nd,void* value,size_t size){
	if(nd->value != NULL)
		printf("NodeWarning: value is no empty\n");

	nd->value = malloc(size);

	memcpy(nd->value,value,size);
}

void* btree_node_search(btree* tree,node* nd,void* value){
	if(nd == NULL)
		return NULL;

	int cmp = tree->cmp(nd->value,value);

	if(cmp == 0){
		return nd->value;
	}else if(cmp > 0){
		return btree_node_search(tree,nd->right,value);
	}else{
		return btree_node_search(tree,nd->left,value);
	}
}

void* btree_search(btree* tr,void* value){
	if(tr->cmp == NULL){
		printf("TreeWarning: no compare function\n");
		return NULL;
	}

	return btree_node_search(tr,tr->root,value);
}

void btree_node_insert(btree* tr,node** nd,void* value){
	if(*nd == NULL){
		printf("Add value\n");
		*nd = node_new();
		node_set_value(*nd,value,tr->size);	
		return;
	}

	if(tr->cmp((*nd)->value,value) > 0)
		btree_node_insert(tr,&(*nd)->right,value);	
	else
		btree_node_insert(tr,&(*nd)->left,value);	
}

void btree_add(btree* tr,void* value){
	if(tr->cmp == NULL){
		printf("TreeWarning: no compare function\n");
		return;
	}
	
	btree_node_insert(tr,&tr->root,value);
}

#endif
