#ifndef GRAMMAR_FILE_DEFINED_H
#define GRAMMAR_FILE_DEFINED_H

#include "grammar.h"
#include "../util/vector.h"
#include "../util/util.h"


#define RD_GR_HEADNAME_DICTIONARY "DICTIONARY"
#define RD_GR_HEADNAME_NOTERMINAL "NOTERMINAL"
#define RD_GR_HEADNAME_TERMINAL   "TERMINAL"
#define RD_GR_HEADNAME_INIT       "INITIAL"
#define RD_GR_HEADNAME_RULES 	  "RULE"

typedef enum {
	reader_search,
	reader_dictionary,
	reader_noterminal,
	reader_terminal,
	reader_rule,
	reader_init
}reader_state;

typedef struct{
	char* word;
	language_simbol simbol;
}reader_simbol;

typedef struct{
	char word[100];
	char* src_line;
	char* next;
	int search_value;

	int line;

	reader_state state;
	reader_simbol current;
	rule* rl_current;
	vector* ls_current;
	vector* simbols; 
	grammar* gramm;
}reader_grammar;

reader_grammar* reader_grammar_new();
void reader_grammar_free(reader_grammar* rg);

void reader_grammar_error(reader_grammar* reader);
reader_simbol* reader_grammar_search_simbol(reader_grammar* reader);
int reader_grammar_isespecial(char character);
int reader_grammar_isword(char character);
void reader_grammar_read_number(reader_grammar* reader,int offset);
void reader_grammar_read_word(reader_grammar* reader,int offset);
char* reader_grammar_get_word(reader_grammar* reader,int offset);
int reader_grammar_isfirstword(char current);
void reader_grammar_insert_rule(reader_grammar* reader);
void reader_grammar_insert_derivation(reader_grammar* reader);
void reader_grammar_set_init(reader_grammar* reader);
void reader_grammar_insert_epsilon(reader_grammar* reader);
void reader_grammar_process_rule(reader_grammar* reader);
void reader_grammar_set_type(reader_grammar* reader,simbol_type type);
void reader_grammar_process_term(reader_grammar* reader,simbol_type type);
void reader_grammar_process_init(reader_grammar* reader);
void reader_grammar_redirect(reader_grammar* reader);
void reader_grammar_state(reader_grammar* reader);
void reader_grammar_process_line(reader_grammar* reader,char* line);
void reader_grammar_check(reader_grammar* reader);
void reader_grammar_by_file(reader_grammar* reader,const char* name);

#endif
