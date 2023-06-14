#include "util/tree.h"

int int_cmp(void* a,void* b){
	return *((int*)a) - *((int*)b);
}

int main(){
	btree* tree = btree_new(sizeof(int));

	btree_set_cmp(tree,int_cmp);
	
	int i;
	
	for(i = 0; i < 10; i++){
		btree_add(tree,&i);
	}

	i = 4;

	int* b2 = btree_search(tree,&i);

	printf("%d\n",*b2);

	printf("%d , %d\n",node_left_height(tree->root,0),node_right_height(tree->root,0));

	return 0;
}
