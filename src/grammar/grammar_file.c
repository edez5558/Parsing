#include "grammar_file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

reader_grammar* reader_grammar_new(){
	reader_grammar* new_reader = (reader_grammar*)malloc(sizeof(reader_grammar));
	
	new_reader->simbols = vector_new(sizeof(reader_simbol),6);; 
	new_reader->state = reader_search;
	new_reader->next = NULL;
	new_reader->gramm = NULL;
	new_reader->search_value = 0;
	new_reader->rl_current = NULL;
	new_reader->ls_current = NULL;
	new_reader->line = 0;
	new_reader->src_line = NULL;

	return new_reader;
}

void reader_grammar_error(reader_grammar* reader){
	fprintf(stderr,"(%d,%d)",reader->line,(int)(reader->next - reader->src_line));
}

reader_simbol* reader_grammar_search_simbol(reader_grammar* reader){
	reader_simbol* current = NULL;
	for(int i = 0; i < reader->simbols->size; i++){
		current = (reader_simbol*)vector_at(reader->simbols,i);	
			
		if(strcmp(current->word,reader->word) == 0)
			return current;
	}

	return NULL;
}

int reader_grammar_isespecial(char character){
	return character == ':' || character == '#' ||
		   character == '|' || character == ';' ||
		   character == '~';
}
int reader_grammar_isword(char character){
	return !isspace(character) && !reader_grammar_isespecial(character);
}

void reader_grammar_read_number(reader_grammar* reader,int offset){
	char* last = reader->next + offset;

	while(isdigit(*reader->next))
		reader->next++;

	size_t n_read = reader->next - last;

	if(n_read > sizeof(reader->word)){
		n_read = sizeof(reader->word);
	}

	strncpy(reader->word,last,n_read);
	reader->word[n_read] = '\0';

	return;
}

void reader_grammar_read_word(reader_grammar* reader,int offset){
	char* last = reader->next + offset;

	while(reader_grammar_isword(*reader->next))
		reader->next++;		

	size_t n_read = reader->next - last;

	if(n_read > sizeof(reader->word)){
		n_read = sizeof(reader->word);
	}

	strncpy(reader->word,last,n_read);
	reader->word[n_read] = '\0';
	
	return;
}

char* reader_grammar_get_word(reader_grammar* reader,int offset){
	char* last = reader->next + offset;

	while(reader_grammar_isword(*reader->next))
		reader->next++;

	size_t n_read = reader->next - last - 1;

	char* new_string = sub_string(last,0,n_read);	

	return new_string;
}

int reader_grammar_isfirstword(char current){
	return !isdigit(current) && reader_grammar_isword(current);
}

void reader_grammar_process_dictionary(reader_grammar* reader){
	char current;
	int value;

	while((current = *reader->next)){
		reader->next++;

		if(isspace(current))
			continue;
		else if(current == '#'){
			reader->next--;
			reader->state = reader_search;
			return;
		}else if(!reader->search_value &&
				reader_grammar_isfirstword(current)){
			reader->current.word = reader_grammar_get_word(reader,-1);
		}else if(!reader->search_value && current == ':'){
			reader->search_value = 1;
		}else if(reader->search_value && isdigit(current)){
			reader_grammar_read_number(reader,-1);

			sscanf(reader->word,"%d",&reader->current.simbol.value);

			reader->current.simbol.type = nodefine;

			vector_push(reader->simbols,&reader->current);

			reader->search_value = 0;
		}else{
			reader_grammar_error(reader);
			printf("Unexpected %c\n",current);
		}
	}
}

void reader_grammar_insert_rule(reader_grammar* reader){
	reader_simbol* simbol = reader_grammar_search_simbol(reader);
	rule* to_rule = NULL;


	if(simbol == NULL){
		reader_grammar_error(reader);
		printf("'%s' is no defined\n",reader->word);
		return;
	}

	switch(simbol->simbol.type){
		case terminal:
			reader_grammar_error(reader);
			printf("Try to create an rule with '%s' terminal",reader->word);
			return;
		case nodefine:
			reader_grammar_error(reader);
			printf("'%s' is no define, cannot create rule\n",reader->word);
			return;
		default:
			break;
	}

	to_rule = grammar_find_rule(reader->gramm,simbol->simbol.value);
	
	if(to_rule == NULL){
		to_rule = grammar_add_rule(reader->gramm,simbol->simbol.value);
	}

	reader->rl_current = to_rule;

}

void reader_grammar_insert_derivation(reader_grammar* reader){
	reader_simbol* simbol = reader_grammar_search_simbol(reader);

	if(simbol == NULL){
		reader_grammar_error(reader);
		printf("'%s' is no defined in dictionary\n",reader->word);	
		return;
	}

	if(simbol->simbol.type == nodefine){
		reader_grammar_error(reader);
		printf("Warning: '%s' insert on production, but is no define\n",reader->word);
	}

	if(reader->rl_current == NULL){
		return;
	}

	if(reader->ls_current == NULL){
		reader->ls_current = vector_new(sizeof(language_simbol),5);

		vector_push(
			reader->rl_current->production.vec, 
			&reader->ls_current);
	}

	vector_push(reader->ls_current,&simbol->simbol);
}

void reader_grammar_set_init(reader_grammar* reader){
	reader_simbol* simbol = reader_grammar_search_simbol(reader);

	if(simbol == NULL){
		reader_grammar_error(reader);
		printf("'%s' is no defined\n",reader->word);
		return;
	}

	if(simbol->simbol.type != noterminal){
		reader_grammar_error(reader);
		printf("'%s' no correct type for initial simbol\n",reader->word);
		return;
	}

	reader->gramm->initial = simbol->simbol.value;
}

void reader_grammar_insert_epsilon(reader_grammar* reader){
	reader->rl_current->production.epsilon = 1;
}

void reader_grammar_process_rule(reader_grammar* reader){
	char current;
	
	while((current = *reader->next)){
		reader->next++;

		if(isspace(current))
			continue;
		else if(current == '#'){
			reader->next--;
			reader->state = reader_search;
			return;
		}else if(reader_grammar_isfirstword(current)){
			reader_grammar_read_word(reader,-1);

			if(!reader->search_value){
				reader_grammar_insert_rule(reader);
				reader->ls_current = NULL;
			}
			else
				reader_grammar_insert_derivation(reader);
		}else if(!reader->search_value && current == ':'){
			reader->search_value = 1;
		}else if(reader->search_value && current == '~'){
			reader_grammar_insert_epsilon(reader);
		}else if(reader->search_value && current == '|'){
			reader->ls_current = NULL;
		}else if(reader->search_value && current == ';'){
			reader->search_value = 0;
			reader->rl_current = NULL;
			reader->ls_current = NULL;
		}
	}

}

void reader_grammar_set_type(reader_grammar* reader,simbol_type type){
	reader_simbol* simbol = reader_grammar_search_simbol(reader);

	if(simbol == NULL){
		reader_grammar_error(reader);
		printf("'%s' has not been specified in dictionary\n",reader->word);
		return;
	}

	int value = simbol->simbol.value;

	simbol->simbol.type = type;

	switch(simbol->simbol.type){
		case terminal:
			vector_push(reader->gramm->terminals,&value);
			break;
		case noterminal:
			vector_push(reader->gramm->noterminals,&value);
			break;
		default:
			break;
	}
}

void reader_grammar_process_term(reader_grammar* reader,simbol_type type){
	char current;

	while((current = *reader->next)){
		reader->next++;	

		if(isspace(current))
			continue;
		else if(current == '#'){
			reader->next--;
			reader->state = reader_search;
			return;
		}
		else if(reader_grammar_isfirstword(current)){
			reader_grammar_read_word(reader,-1);
			reader_grammar_set_type(reader,type);
		}else{
			reader_grammar_error(reader);
			printf("unexpected character '%c'\n",current);
			return;
		}
	}

	return;
}

void reader_grammar_process_init(reader_grammar* reader){
	char current;

	while((current = *reader->next)){
		reader->next++;

		if(isspace(current))
			continue;
		else if(current == '#'){
			reader->next--;
			reader->state = reader_search;
			return;
		}else if(reader_grammar_isfirstword(current)){
			reader_grammar_read_word(reader,-1);

			reader_grammar_set_init(reader);
		}
	}
}

void reader_grammar_redirect(reader_grammar* reader){
	switch(reader->state){
		case reader_noterminal:
			reader_grammar_process_term(reader,noterminal);
			break;
		case reader_terminal:
			reader_grammar_process_term(reader,terminal);
			break;
		case reader_dictionary:
			reader_grammar_process_dictionary(reader);
			break;
		case reader_rule:
			reader_grammar_process_rule(reader);
			break;
		case reader_init:
			reader_grammar_process_init(reader);
			break;
		default:
			break;
	}
}

void reader_grammar_state(reader_grammar* reader){
	if(strstr(reader->word,RD_GR_HEADNAME_NOTERMINAL)){
		reader->state = reader_noterminal;
	}else if(strstr(reader->word,RD_GR_HEADNAME_TERMINAL)){
		reader->state = reader_terminal;
	}else if(strstr(reader->word,RD_GR_HEADNAME_DICTIONARY)){
		reader->state = reader_dictionary;
	}else if(strstr(reader->word,RD_GR_HEADNAME_RULES)){
		reader->state = reader_rule;
	}else if(strstr(reader->word,RD_GR_HEADNAME_INIT)){
		reader->state = reader_init;
	}else{
		reader_grammar_error(reader);
		printf("Unexpected '%s' state\n",reader->word);
		reader->state = reader_search;
	}
}

void reader_grammar_process_line(reader_grammar* reader,char* line){
	char current;
	char* last = NULL;

	reader->next = line;
	reader_grammar_redirect(reader);

	while((current = *reader->next)){
		reader->next++;
		
		if(isspace(current))
			continue;
		else if(current == '#'){
			last = reader->next;	

			while(isalpha(*reader->next))
				reader->next++;
			
			strncpy(reader->word,last,reader->next - last);
			reader->word[reader->next - last] = '\0';

			reader_grammar_state(reader);
			reader_grammar_redirect(reader);
		}else{
			reader_grammar_error(reader);
			printf("Unexpected '%c' character in %s\n",current,line);
		}
	}

}

void reader_grammar_check(reader_grammar* reader){
	grammar* gramm = reader->gramm;
	rule* rl = NULL;
	int* value = NULL;
	int find = 0;

	for(int i = 0; i < gramm->noterminals->size; i++){
		value = (int*)vector_at(gramm->noterminals,i);			
		find = 0;
		for(int j = 0; j < gramm->rules->size; j++){
			rl = (rule*)vector_at_ptr(gramm->rules,j);
			if(*value == rl->simbol){
				find = 1;
				break;
			}
		}

		if(!find){
			printf("%d has not rules\n",*value);
		}
	}
}

void reader_grammar_by_file(reader_grammar* reader,const char* name){
	FILE* file = fopen(name,"r");
	char buffer[2048];

	if(file == NULL){
		printf("Cannot open file: %s\n",name);
		return;
	}

	if(reader->gramm != NULL){
		//free previous grammar
	}

	reader->gramm = grammar_new();

	while(fgets(buffer,sizeof(buffer),file)){
		reader->line++;
		reader->src_line = buffer;
		reader_grammar_process_line(reader,buffer);
	}

	reader_grammar_check(reader);
}
