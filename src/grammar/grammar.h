#ifndef GRAMMAR_DEFINED_H
#define GRAMMAR_DEFINED_H

#include <stdlib.h>
#include "../util/vector.h"
#include "../util/stack.h"

#define GRAMMAR_STATUS_OK 0
#define GRAMMAR_STATUS_EMPTY -1
#define GRAMMAR_STATUS_UNEXPECT -2
#define GRAMMAR_STATUS_WRONG -3
#define GRAMMAR_STATUS_COMPLETE 1

typedef enum{
	nodefine,
	noterminal,
	terminal,
} simbol_type;		

typedef vector vector_simbolptr;
typedef vector vector_simbol;
typedef vector vector_func;
typedef vector vector_int;

typedef struct{
	int value;
	simbol_type type;
}language_simbol;

typedef struct{
	int epsilon;
	//vector de vector simbol
	vector* vec;
	//vector de vector int
	vector* first;
}derivation;

typedef struct{
	int epsilon;
	//vector de int
	vector* vec;
}func;

typedef struct{
	int value;
	vector_simbol* production;
}predictive;

typedef struct{
	int simbol;
	void* gramm;
	derivation production;

	func first;
	func follow;
	//vector predictive
	vector* LL;
}rule;

typedef struct{
	vector_int* noterminals;
	vector_int* terminals;
	int initial;
	vector* rules;
	stack* state;
}grammar;

int vecint_exist(vector_int* vec,int element);
void vecint_push_unique(vector_int* vec,int element);
void vecint_push_unique_vecint(vector_int* vec_a,vector_int* vec_b);

rule* rule_new(int simbol);
void rule_free(rule* rl);
rule* grammar_find_rule(grammar* gramm,int noterminal);
int rule_isfirst(rule* rl);
int rule_isfollow(rule* rl);
int rule_ispredictive(rule* rl);

void production_add_follow(vector_simbol* simbols,rule* rl,rule* rl_current,language_simbol* next, int noterminal);
void production_add_first(rule* prod,int index,grammar* gramm);

void derivation_set_first_vector(derivation* production);
vector_int* derivation_get_first(derivation* production, int index);
vector_simbol* rule_get_predictive(rule* rl,int value,int* find);

void grammar_start_input(grammar* gramm);
int grammar_input(grammar* gramm,int value,int* status);
int grammar_end_input(grammar* gramm,int* status);
void grammar_print_state(grammar* gramm);

grammar* grammar_new();
void grammar_free(grammar** gramm);

void grammar_add_production(grammar* gramm,int noterminal,language_simbol* simbols,int n_simbol);
void rule_add_predictive(rule* prod);
void rule_check_LL(rule* rl);
rule* grammar_add_rule(grammar* gramm,int simbol);
rule* grammar_get_first(grammar* gramm,int noterminal);
rule* grammar_get_follow(grammar* gramm,int noterminal);

void grammar_generate_first(grammar* gramm,int noterminal);
void grammar_generate_follow(grammar* gramm,int noterminal);
void grammar_generate_predictive(grammar* gramm,int noterminal);

void grammar_compute_first(grammar* gramm);
void grammar_compute_follow(grammar* gramm);
void grammar_compute_predictive(grammar* gramm);

void grammar_check_LL(grammar* gramm);

void grammar_debug_first(grammar* gramm,int noterminal);
void grammar_debug_follow(grammar* gramm,int noterminal);

#endif
