#include "lexical.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "util/util.h"

const char* lexical_reserved_words[] = {
	[TOKEN_KEYWORD_INT] = "int",
	[TOKEN_KEYWORD_FLOAT] = "float",
	[TOKEN_KEYWORD_BYTE] = "byte",
	[TOKEN_KEYWORD_CHAR] = "char",
	[TOKEN_KEYWORD_BOOL] = "bool",
	[TOKEN_KEYWORD_STRING] = "string",
	[TOKEN_KEYWORD_VOID] = "void",
	[TOKEN_CO_NULL] = "null",
	[TOKEN_KEYWORD_OBJECT] = "object",
	[TOKEN_KEYWORD_NEW] = "new",
	[TOKEN_KEYWORD_IF] = "if",
	[TOKEN_KEYWORD_ELSE] = "else",
	[TOKEN_KEYWORD_DELETE] = "delete",
	[TOKEN_KEYWORD_WHILE] = "while",
	[TOKEN_KEYWORD_DO] = "do",
	[TOKEN_KEYWORD_FOR] = "for",
	[TOKEN_KEYWORD_FOREACH] = "foreach",
	[TOKEN_KEYWORD_MAIN] = "main",
	[TOKEN_KEYWORD_CASE] = "case",
	[TOKEN_KEYWORD_SWITCH] = "switch",
	[TOKEN_KEYWORD_INCLUDE] = "include",
	[TOKEN_KEYWORD_ENDIF] = "endif",
	[TOKEN_KEYWORD_RETURN] = "return",
	[TOKEN_KEYWORD_ELIF] = "elif",
	[TOKEN_KEYWORD_FUNC] = "func",
	[TOKEN_KEYWORD_CALL] = "call",
	[TOKEN_CO_TRUE] = "true",
	[TOKEN_CO_FALSE] = "false",
	[TOKEN_CO_OUT] = "out",
	[TOKEN_CO_IN] = "in"
};

const char* lexical_combined_characters[] = {
	[TOKEN_OP_ASSIGN_DIST] = "!=",
	[TOKEN_OP_EQUALS] = "==",
	[TOKEN_OP_EQUAL_GREATER] = ">=",
	[TOKEN_OP_EQUAL_LESS] = "<=",
	[TOKEN_OP_ASSIGN_PLUS] = "+=",
	[TOKEN_OP_ASSIGN_MINUS] = "-=",
	[TOKEN_OP_ASSIGN_MULT] = "*=",
	[TOKEN_OP_ASSIGN_DIV] = "/=",
	[TOKEN_OP_AND] = "&&",
	[TOKEN_OP_OR] = "||",
};

const char lexical_character_especial[] = {
	[TOKEN_PU_LBRACE] = '{',
	[TOKEN_PU_RBRACE] = '}',
	[TOKEN_PU_LBRACKET] = '[',
	[TOKEN_PU_RBRACKET] = ']',
	[TOKEN_PU_LPARENTHESIS] = '(',
	[TOKEN_PU_RPARENTHESIS] = ')',
	[TOKEN_PU_COMMA] = ',',
	[TOKEN_PU_DOT] = '.',
	[TOKEN_PU_SEMICOLON] = ';',
	[TOKEN_PU_DOUBLEDOT] = ':',
	[TOKEN_OP_ASSIGN] = '=',
	[TOKEN_OP_PLUS] = '+',
	[TOKEN_OP_MINUS] = '-',
	[TOKEN_OP_MULT] = '*',
	[TOKEN_OP_DIV] = '/',
	[TOKEN_OP_NEG] = '!',
	[TOKEN_OP_LESS] = '<',
	[TOKEN_OP_GREATER] = '>'
};

int lexical_n_reserved_words = sizeof(lexical_reserved_words)/sizeof(char*);
int lexical_n_combined_characters = sizeof(lexical_combined_characters)/sizeof(char*);
int lexical_n_charater_especial = sizeof(lexical_character_especial)/sizeof(char);

BOOL on_comment = FALSE;

char character_buffer[3];

lexical* lexical_new(){
	lexical* tmp_lexical = (lexical*)malloc(sizeof(lexical));

	tmp_lexical->count_token = 0;
	tmp_lexical->first_token = NULL;
	tmp_lexical->current_token = NULL;
	tmp_lexical->last_token = NULL;

	return tmp_lexical;
}



void lexical_reset_curren(lexical* lexer){
	lexer->current_token = lexer->first_token;	
}

token* lexical_get_token(lexical* lexer){
	if(lexer == NULL || lexer->current_token == NULL) return NULL;

	token* get_token = lexer->current_token;	
	lexer->current_token = (token*)lexer->current_token->next_token;

	return get_token;
}

void lexical_insert_new_token(lexical* lexer, token_type type,token_value value, void* value_reference){
	if(lexer == NULL) return;

	token* new_token = token_init(type,value,value_reference,lexer->info);

	token_chain(lexer->last_token,new_token);

	if(lexer->first_token == NULL){
		lexer->first_token = new_token;
		lexer->last_token = lexer->first_token;
		lexer->current_token = lexer->first_token;
	}
	else{
		lexer->last_token = new_token;
	}

	lexer->count_token++;
}

void lexical_free(lexical** lexer){
	if(lexer == NULL) return;

	token* delete_token = (*lexer)->first_token;
	token* next_token = NULL;
	while(delete_token){
		next_token = (token*)delete_token->next_token;

		token_free(&delete_token);

		delete_token = next_token;
	}

	free(*lexer);

	*lexer = NULL;
}

int is_reserved_word(char* word){
    for(int i = 0; i < lexical_n_reserved_words; i++){
        if(strcmp(lexical_reserved_words[i],word) == 0)
            return i;
    }

    return -1;
}

int is_character_especial(char character){
    for(int i = 0; i < lexical_n_charater_especial; i++){
        if(character == lexical_character_especial[i])
            return i;
    }
    
    return -1;
}

int is_combined_character(char* first_character){
	char* combined_character = NULL;
	for(int i = 0; i < lexical_n_combined_characters; i++){
		combined_character = (char*)lexical_combined_characters[i];
		if(combined_character[0] == first_character[0] &&
		   combined_character[1] == first_character[1])
			return i;
	}

	return -1;
}



char* lexical_get_read(char* first,char* current){
    return sub_string(first,0,current - first - 1);
}

char* skip_characters(char* current){
    while(*current == ' ' || *current == '\t')
        current++;

    return current;
}

char* read_number(char* current){
    while(isdigit(*current) || *current == '.'){
        current++; 
    }

    return current;
}

char* read_id(char* current){
    while(isalnum(*current) || *current == '_')
        current++;

    return current;
}

char* read_literal(char* current){
    while(*current != '"' && *current != '\0')
        current++;

    if(*current == '\0') return current;

    return ++current;
}

void lexical_print_debugger(lexical* lexer){
	if(lexer == NULL) return;

	token* current_token = lexer->first_token;

	while(current_token){
		switch(current_token->type){
			case TOKEN_IDENTIFIER:
				printf("Identificador: %s\n",(char*)current_token->value_reference);
				break;
			case TOKEN_CONSTANT:
				printf("Constante: %s\n",(char*)current_token->value_reference);
				break;
			case TOKEN_SINGLE:
				printf("Caracter: %c\n",lexical_character_especial[current_token->value]);
				break;
			case TOKEN_KEYWORD:
				printf("Palabra reservada: %s\n",lexical_reserved_words[current_token->value]);
				break;
			case TOKEN_COMBINED:
				printf("Combinacion caracteres: %s\n",lexical_combined_characters[current_token->value]);
				break;
			default:
				break;
		}	
		current_token = (token*)current_token->next_token;
	}
}

char* get_string_of_thash(int thash){
	int type;
	int value;

	thasht_token(thash,&type,&value);

	switch((token_type)type){
		case TOKEN_IDENTIFIER:
			return "identificador";
		case TOKEN_KEYWORD:
			return (char*)lexical_reserved_words[value];
		case TOKEN_SINGLE:
			character_buffer[0] = lexical_character_especial[value];
			character_buffer[1] = '\0';
			return character_buffer;
		case TOKEN_COMBINED:
			return (char*)lexical_combined_characters[value];
		case TOKEN_CONSTANT:
			switch((token_value)value){
				case TOKEN_CO_NUMBER: 
					return "numero";
				case TOKEN_CO_LITERAL:
					return "string";
				default:
					return "none";

			}
			break;

		default:
			return "none";
	}
};

void lexical_to_file(const char* name){
	char buffer[3];
	FILE* file = fopen(name,"w");
	const char* text;

	if(file == NULL) return;

	for(int i = 0; i < lexical_n_reserved_words; i++){
		text = lexical_reserved_words[i];

		fprintf(file,"%s: %d\n",text,token_thash(TOKEN_KEYWORD,(token_value)i));
	}

	for(int i = 0; i < lexical_n_charater_especial; i++){
		buffer[0] = lexical_character_especial[i];
		buffer[1] = '\0';

		fprintf(file,"%s: %d\n",buffer,
				token_thash(TOKEN_SINGLE,(token_value)i));
	}

	for(int i = 0; i < lexical_n_combined_characters; i++){
		text = lexical_combined_characters[i];

		fprintf(file,"%s: %d\n",text,
				token_thash(TOKEN_COMBINED,(token_value)i));
	}


	fprintf(file,"NUMBER: %d\n",token_thash(TOKEN_CONSTANT,TOKEN_CO_NUMBER));
	fprintf(file,"STRING: %d\n",token_thash(TOKEN_CONSTANT,TOKEN_CO_LITERAL));
	fprintf(file,"ID: %d\n",token_thash(TOKEN_IDENTIFIER,TOKEN_ID_VALUE));
	fprintf(file,"NOTERMINAL0: %d\n",token_thash(TOKEN_NO,0));
	fprintf(file,"NOTERMINAL1: %d\n",token_thash(TOKEN_NO,1));
	fprintf(file,"NOTERMINAL2: %d\n",token_thash(TOKEN_NO,2));
	
	fprintf(file,"#TERMINAL ");

	for(int i = 0; i < lexical_n_reserved_words; i++){
		text = lexical_reserved_words[i];

		fprintf(file,"%s ",text);
	}

	for(int i = 0; i < lexical_n_charater_especial; i++){
		buffer[0] = lexical_character_especial[i];
		buffer[1] = '\0';

		fprintf(file,"%s ",buffer);
	}

	for(int i = 0; i < lexical_n_combined_characters; i++){
		text = lexical_combined_characters[i];

		fprintf(file,"%s ",text);
	}

	fclose(file);
}

void lexical_process_line(lexical* lexer,char* line, size_t len,int column){
    char* next = line;
    char current;

    char* last_start = line;
    BOOL isDecimal = FALSE;

	lexer->info.line = (char*)malloc(len + 1);
	memcpy(lexer->info.line,line,len);
	lexer->info.line[len] = '\0';
	lexer->info.column = column;

	int result = -1;
    while((current = *next)){
        next++;

        if(on_comment){
            if(current == '*' && *next == '/'){
                on_comment = FALSE;
                next++;
            }
            continue;
        }
    
        if(current == ' ' || current == '\t'){
            next = skip_characters(next);
        }else if(current == '/' && *next == '*'){
            on_comment = TRUE;
            next++;
        }else if(current == '/' && *next == '/'){
            while(*next != '\0' && *next != '\n'){
                next++;
            }
        }else if(current == '"'){
            //Read an literal string
            last_start = next - 1;

            next = read_literal(next);

            char* word = lexical_get_read(last_start,next);

			lexer->info.row = last_start - line;
			lexical_insert_new_token(lexer,TOKEN_CONSTANT,TOKEN_CO_LITERAL,word);
        }else if(isdigit(current) || (isdigit(*next) && (current == '+' || current == '-')) ){
            //Read an number
            last_start = next - 1;

            next = read_number(next);

            char* word = lexical_get_read(last_start,next);

			lexer->info.row = last_start - line;
			lexical_insert_new_token(lexer,TOKEN_CONSTANT,TOKEN_CO_NUMBER,word);
        }else if(isalpha(current) || current == '_'){
            //Read an identifer
            last_start = next - 1;

            next = read_id(next);

            char* word = lexical_get_read(last_start,next);

			lexer->info.row = last_start - line;
			if((result = is_reserved_word(word)) != -1){
				lexical_insert_new_token(lexer,TOKEN_KEYWORD,(token_value)result,word);
			}else{
				lexical_insert_new_token(lexer,TOKEN_IDENTIFIER,TOKEN_ID_VALUE,word);
			}
        }else if(!isalnum(*next) && (result = is_combined_character(next - 1)) != -1){
			lexer->info.row = (next - 1) - line;

			next++;
			lexical_insert_new_token(lexer,TOKEN_COMBINED,(token_value)result,NULL);
		}else if((result = is_character_especial(current)) != -1){
			lexer->info.row = (next - 1) - line;
			lexical_insert_new_token(lexer,TOKEN_SINGLE,(token_value)result,NULL);
		}else{
			printf("LEXER: %c caracter no esperado\n",current);
		}
	}
}
