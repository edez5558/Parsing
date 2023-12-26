#ifndef LEXICAL_H_DEFINE
#define LEXICAL_H_DEFINE

#include "token.h"
#include <stdio.h>

typedef char BOOL;
#define TRUE 1
#define FALSE 0

extern const char* lexical_reserved_words[];
extern const char* lexical_combined_characters[];
extern const char lexical_character_especial[];
extern int lexical_n_reserved_words;
extern int lexical_n_combined_characters;
extern int lexical_n_charater_especial;

extern BOOL on_comment;
extern char character_buffer[3];

typedef struct{
	int count_token;
	token* first_token;
	token* current_token;
	token* last_token;
	token_info info;
}lexical;

lexical* lexical_new();

void lexical_reset_curren(lexical* lexer);

token* lexical_get_token(lexical* lexer);

void lexical_insert_new_token(lexical* lexer, token_type type,token_value value, void* value_reference);

void lexical_free(lexical** lexer);

int is_reserved_word(char* word);

int is_character_especial(char character);

int is_combined_character(char* first_character);

char* lexical_get_read(char* first,char* current);

char* get_string_of_thash(int thash);

char* skip_characters(char* current);

char* read_number(char* current);

char* read_id(char* current);

char* read_literal(char* current);

void lexical_print_debugger(lexical* lexer);

void lexical_to_file(const char* name);

void lexical_process_line(lexical* lexer,char* line, size_t len,int column);

#endif
