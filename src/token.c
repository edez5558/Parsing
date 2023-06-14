
#include "token.h"

#include <stdlib.h>
#include <stdio.h>

token* token_init(token_type type,token_value value,void* value_reference,token_info info){
	token* token_new = (token*)malloc(sizeof(token));

	if(token_new == NULL) return NULL;

	token_new->type = type;
	token_new->value = value;
	token_new->value_reference = value_reference;
	token_new->next_token = NULL;
	token_new->previous_token = NULL;
	token_new->type_hash = token_thash(type,value);
	token_new->info = info;

	return token_new;
}

void token_free(token** token_to_delete){
	free(*token_to_delete);
}

token* token_chain(token* current,token* add){
	if(add == NULL) return NULL;

	if(current != NULL)
		current->next_token = add;

	add->previous_token = current;

	return add;
}

int token_thash(int type,int value){
	return (int)value + ((int)type << (sizeof(int)/2 * 8));
}

void token_error_info(token* tkn){
	printf("(%d,%d) %s\n ",tkn->info.column,tkn->info.row,tkn->info.line);
}

void thasht_token(int thash, int* type,int* value){
	*type = thash >> (sizeof(int)/2 * 8);
	*value = thash << (sizeof(int)/2 * 8);
	*value >>= sizeof(int)/2 * 8;
}
