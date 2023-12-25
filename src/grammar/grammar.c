#include "grammar.h"

void grammar_generate_predictive(grammar* gramm,int noterminal){
	rule* rl = grammar_find_rule(gramm,noterminal);

	if(rule_ispredictive(rl))
		return;

	rule_add_predictive(rl);	
}

vector_simbol* rule_get_predictive(rule* rl,int value,int* find){
	predictive* pred = NULL;
	for(int i = 0; i < rl->LL->size; i++){
		pred = (predictive*)vector_at(rl->LL,i);

		if(pred->value == value){
			*find = 1;
			return pred->production;
		}
	}

	*find = 0;
	return NULL;
}

int rule_ispredictive(rule* rl){
	return rl->LL->size != 0;
}

void rule_add_predictive(rule* rl){
	vector_simbol* simbols = NULL;
	language_simbol* simbol = NULL;
	predictive pre;

	for(int i = 0; i < rl->production.vec->size; i++){
		vector_int* first_production = derivation_get_first(&rl->production,i);
		simbols = (vector_simbol*)vector_at_ptr(rl->production.vec,i);
		int* terminal_value = NULL;

		pre.production = simbols;
		for(int j = 0; j < first_production->size; j++){
			terminal_value = (int*)vector_at(first_production,j);

			pre.value = *terminal_value;	
			
			vector_push(rl->LL,&pre);
		}

		if(!rl->first.epsilon)
			continue;

		for(int j = 0; j < rl->follow.vec->size; j++){
			terminal_value = (int*)vector_at(rl->follow.vec,j);

			pre.value = *terminal_value;
			
			pre.production = NULL;
			vector_push(rl->LL,&pre);
		}

		if(rl->follow.epsilon){
			pre.value = -1;
			pre.production = NULL;
			vector_push(rl->LL,&pre);
		}
	}
}

vector_int* derivation_get_first(derivation* production, int index){
	return (vector_int*)vector_at_ptr(production->first,index);
}

int vecint_exist(vector_int* vec,int element){
	int* value = NULL;
	for(int i = 0; i < vec->size; i++){
		value = (int*)vector_at(vec,i);	
		
		if(*value == element)
			return 1;
	}

	return 0;
}

void vecint_push_unique(vector_int* vec,int element){
	if(vecint_exist(vec,element)) return;

	vector_push(vec,&element);
}

void vecint_push_unique_vecint(vector_int* vec_a,vector_int* vec_b){
	int* value = NULL;
	for(int i = 0; i < vec_b->size; i++){
		value = (int*)vector_at(vec_b,i);

		vecint_push_unique(vec_a,*value);
	}
}

void free_vector_of_vector(void* v){
	puts("Free Vector Of Vector");
	vector_free(*((void**)v));
}

rule* rule_new(int simbol){
	rule* rl = (rule*)malloc(sizeof(rule)); 

	rl->simbol = simbol;

	rl->production.vec = vector_new(sizeof(vector_simbolptr*),1);
	rl->production.first = vector_new(sizeof(vector*),1);
	rl->production.epsilon = 0;

	rl->first.vec = vector_new(sizeof(vector_int*),4);
	rl->first.epsilon = 0;

	rl->follow.vec = vector_new(sizeof(vector_int*),4);
	rl->follow.epsilon = 0;

	rl->LL = vector_new(sizeof(predictive),4);

	vector_set_free(rl->production.vec,&free_vector_of_vector);
	vector_set_free(rl->production.first,&free_vector_of_vector);

	return rl;
}

void rule_free(rule* rl){
	vector_free(rl->production.vec);
	vector_free(rl->production.first);

	vector_free(rl->first.vec);

	vector_free(rl->follow.vec);
}

rule* grammar_find_rule(grammar* gramm,int noterminal){
	vector* rules = gramm->rules;
	rule* current_rule = NULL;

	for(int i = 0; i < rules->size; i++){
		current_rule = (rule*)vector_at_ptr(rules,i);

		if(current_rule->simbol == noterminal)
			return current_rule;
	}

	return NULL;
}

rule* grammar_add_rule(grammar* gramm,int simbol){
	rule* new_rule = rule_new(simbol);

	new_rule->gramm = gramm;

	vector_push(gramm->rules,&new_rule);

	return new_rule;
}

void grammar_add_production(grammar* gramm,int noterminal,language_simbol* simbols,int n_simbol){
	rule* to_rule = grammar_find_rule(gramm, noterminal);
	
	if(to_rule == NULL) return;

	vector* new_productions = vector_new(sizeof(language_simbol),n_simbol);
	
	vector_push_n(new_productions,simbols,n_simbol);	
	vector_push(to_rule->production.vec,&new_productions);
}

void derivation_set_first_vector(derivation* production){
	int size_vec 	= production->vec->size;
	int size_first	= production->first->size;
	if(size_vec == size_first)
		return;
	
	vector_int* vec_int = NULL;
	for(int i = 0; i < size_vec - size_first; i++){
		vec_int = vector_new(sizeof(int),4);	
		vector_push(production->first,&vec_int);
	}
}

void rule_check_LL(rule* rl){
	predictive* pre_left = NULL;
	predictive* pre_right = NULL;

	for(int i = 0; i < rl->LL->size; i++){
		pre_left = (predictive*)vector_at(rl->LL,i);

		for(int j = 0; j < rl->LL->size; j++){
			if(i == j) continue;
			pre_right = (predictive*)vector_at(rl->LL,j);

			if(pre_left->value == pre_right->value){
				printf("Warning: Grammar no LL(1) on rule: %d\n",rl->simbol);
			}
		}
	}
}

void grammar_check_LL(grammar* gramm){
	rule* rl = NULL;
	for(int i = 0; i < gramm->rules->size; i++){
		rl = (rule*)vector_at_ptr(gramm->rules,i);

		rule_check_LL(rl);
	}
}

int rule_isfirst(rule* rl){
	return rl->first.vec->size != 0;
}

int rule_isfollow(rule* rl){
	return rl->follow.vec->size != 0 || rl->follow.epsilon;
}

rule* grammar_get_first(grammar* gramm,int noterminal){
	rule* prod = grammar_find_rule(gramm,noterminal);

	if(prod == NULL){
		printf("%d no has rule production\n",noterminal);
	}

	if(rule_isfirst(prod)){
		return prod;
	}

	grammar_generate_first(gramm,noterminal);

	return prod;
}

rule* grammar_get_follow(grammar* gramm,int noterminal){
	rule* prod = grammar_find_rule(gramm,noterminal);

	if(rule_isfollow(prod)){
		return prod;
	}

	grammar_generate_follow(gramm,noterminal);

	return prod;
}

void production_add_first(rule* rl,int index,grammar* gramm){
	vector_simbolptr* simbols = (vector_simbolptr*)vector_at_ptr(rl->production.vec,index);
	vector_int* local_first = (vector_int*)vector_at_ptr(rl->production.first,index);

	language_simbol* simbol = NULL;
	rule* another_first = NULL;

	for(int i = 0; i < simbols->size; i++){
		simbol = (language_simbol*)vector_at(simbols,i);

		if(simbol->value == rl->simbol){
			if(i == 0){
				printf("Warning: Left recursive detect on rule %d production %d\n",rl->simbol,index);
			}
			continue;
		}

		if(simbol->type == terminal){
			vecint_push_unique(rl->first.vec,simbol->value);
			vecint_push_unique(local_first,simbol->value);
			return;
		}

		another_first = grammar_get_first(gramm,simbol->value);


		vecint_push_unique_vecint(rl->first.vec, another_first->first.vec);
		vecint_push_unique_vecint(local_first,another_first->first.vec);

		if(another_first->first.epsilon == 0)
			return;
	}

	rl->first.epsilon = 1;
}

void grammar_generate_first(grammar* gramm,int noterminal){
	rule* rl = grammar_find_rule(gramm,noterminal);

	if(rl == NULL){
		printf("Try to create an first with NULL\n");	
		return;
	}

	derivation_set_first_vector(&rl->production);

	if(rl->production.epsilon)
		rl->first.epsilon = 1;
	
	derivation_set_first_vector(&rl->production);
	
	//For each production, will search the terminal simbol
	for(int i = 0; i < rl->production.vec->size; i++){
		production_add_first(rl,i,gramm);
	}
}

void production_add_follow(vector_simbol* simbols,rule* rl,rule* rl_current,language_simbol* next, int noterminal){
	language_simbol* simbol = NULL;
	rule* first = NULL;
	grammar* gramm = (grammar*)rl->gramm;

	int i = 0;
	if(next != NULL){
		i = next - (language_simbol*)simbols->data;
		i++;
		return;
	}

	int beta = 0;

	if(noterminal == gramm->initial){
		rl->follow.epsilon = 1;
	}

	for(; i < simbols->size; i++){
		simbol = (language_simbol*)vector_at(simbols,i);

		if(!beta && noterminal == simbol->value){
			beta = 1;
			continue;
		}
		
		if(beta == 0) continue;

		if(simbol->type == terminal){
			vecint_push_unique(rl->follow.vec,simbol->value);	
			return production_add_follow(simbols,rl,rl_current,simbol,noterminal);
		}

		first = grammar_get_first(gramm,simbol->value);
		
		vecint_push_unique_vecint(rl->follow.vec,first->first.vec);
		
		if(!first->first.epsilon)
			return production_add_follow(simbols,rl,rl_current,simbol,noterminal);
	}

	if(beta){
		rule* follow = grammar_get_follow((grammar*)rl->gramm,rl_current->simbol);

		if(follow->simbol == rl->simbol)
			return;

		vecint_push_unique_vecint(rl->follow.vec,follow->follow.vec);

		if(follow->follow.epsilon){
			rl->follow.epsilon = 1;
		}
	}

}

void grammar_generate_follow(grammar* gramm,int noterminal){
	rule* rl = grammar_find_rule(gramm,noterminal);
	rule* current_rule = NULL;

	vector_simbol* production = NULL;
	
	if(rl->simbol == gramm->initial){
		rl->follow.epsilon = 1;
	}

	for(int i = 0; i < gramm->rules->size; i++){
		current_rule = (rule*)vector_at_ptr(gramm->rules,i);
		for(int j = 0; j < current_rule->production.vec->size; j++){
			production = (vector_simbol*)vector_at_ptr(
											current_rule->production.vec,
											j);
			production_add_follow(production,rl,current_rule,NULL,noterminal);
		}
	}
}

void grammar_debug_first(grammar* gramm,int noterminal){
	rule* prod = grammar_find_rule(gramm,noterminal);
	int* value = NULL;

	printf("FIRST(%d) = {",prod->simbol);
	for(int i = 0; i < prod->first.vec->size; i++){
		value = (int*)vector_at(prod->first.vec,i);	
		printf("%d ",*value);
	}

	if(prod->first.epsilon)
		printf("~ ");

	printf("}\n");
}

void grammar_debug_follow(grammar* gramm,int noterminal){
	rule* prod = grammar_find_rule(gramm,noterminal);
	int* value = NULL;

	printf("FOLLOW(%d) = {",prod->simbol);
	for(int i = 0; i < prod->follow.vec->size; i++){
		value = (int*)vector_at(prod->follow.vec,i);	
		printf("%d ",*value);
	}

	if(prod->follow.epsilon)
		printf("~ ");

	printf("}\n");
}

void vector_rule_free(void* v){
	rule_free((*((rule**)v)));
}

grammar* grammar_new(){
	grammar* new_grammar = (grammar*)malloc(sizeof(grammar));

	new_grammar->rules = vector_new(sizeof(rule*),4);
	new_grammar->terminals = vector_new(sizeof(int),10);
	new_grammar->noterminals = vector_new(sizeof(int),10);
	new_grammar->state = stack_new(sizeof(language_simbol),5);


	vector_set_free(new_grammar->rules,&vector_rule_free);

	return new_grammar;
}

void grammar_free(grammar** gramm){
	vector_free((*gramm)->rules);	
	vector_free((*gramm)->terminals);
	vector_free((*gramm)->noterminals);

	free(*gramm);

	*gramm = NULL;
}


void grammar_start_input(grammar* gramm){
	language_simbol empty;
	language_simbol initial;

	initial.type = noterminal;
	initial.value = gramm->initial;

	empty.type = noterminal;
	empty.value = -1;

	stack_push(gramm->state,&empty);
	stack_push(gramm->state,&initial);
}

void grammar_compute_first(grammar* gramm){
	int* noterminal = NULL;
	for(int i = 0; i < gramm->noterminals->size; i++){
		noterminal = (int*)vector_at(gramm->noterminals,i);

		grammar_get_first(gramm,*noterminal);
	}
}

void grammar_compute_follow(grammar* gramm){
	int* noterminal = NULL;
	for(int i = 0; i < gramm->noterminals->size; i++){
		noterminal = (int*)vector_at(gramm->noterminals,i);

		grammar_get_follow(gramm,*noterminal);
	}
}

void grammar_compute_predictive(grammar* gramm){
	int* noterminal = NULL;
	for(int i = 0; i < gramm->noterminals->size; i++){
		noterminal = (int*)vector_at(gramm->noterminals,i);

		grammar_generate_predictive(gramm,*noterminal);
	}
}

void grammar_push_reversed_stack(grammar* gramm,vector_simbol* simbols){
	if(simbols == NULL) return;

	for(int i = simbols->size - 1; i >= 0; i--){
		stack_push(gramm->state,vector_at(simbols,i));	
	}
}

int grammar_input(grammar* gramm,int value,int* status){
	language_simbol simbol;
	language_simbol* aux = NULL;
	vector_simbol* simbols = NULL;
	rule* rl = NULL;
	int find = 0;

	do{
		aux = (language_simbol*)stack_pop(gramm->state);

		if(aux == NULL){
			*status = GRAMMAR_STATUS_EMPTY;
			return 0;
		}

		simbol = *aux;

		if(simbol.value == -1){
			*status = GRAMMAR_STATUS_COMPLETE;
			return 0;
		}

		if(simbol.type == terminal){
			if(simbol.value == value){
				*status = GRAMMAR_STATUS_OK;
				return 0;
			}

			*status = GRAMMAR_STATUS_UNEXPECT;
			return simbol.value;
		}
		
		rl = grammar_find_rule(gramm,simbol.value);

		simbols = rule_get_predictive(rl,value,&find);

		if(find){
			grammar_push_reversed_stack(gramm,simbols);	
		}else{
			*status = GRAMMAR_STATUS_WRONG;
			return 0;
		}
	}while(1);
}

int grammar_end_input(grammar* gramm,int* status){
	return grammar_input(gramm,-1,status);
}

void grammar_print_state(grammar* gramm){
	stack* stk = gramm->state;
	char* current = stk->data;
	language_simbol* simbol;

	putchar('[');
	while(current <= stk->top){
		simbol = (language_simbol*)current;
		current += stk->element_size;
		printf("%d ",simbol->value);
	}
	printf("]\n");
}
