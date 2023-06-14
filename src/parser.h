#ifndef PARSER_DEFINED_H
#define PARSER_DEFINED_H

#include "lexical.h"
#include "token.h"
#include "grammar/grammar.h"
#include "grammar/grammar_file.h"
#include <stdlib.h>

typedef struct{
	lexical* lexer;
	grammar* gramm;

	int status;
	int expected;
	token* current;

	reader_grammar* reader;
} parser;

parser* parser_new(lexical* lexer){
	parser* new_parser = (parser*)malloc(sizeof(parser));
	
	new_parser->lexer = lexer;
	new_parser->reader = reader_grammar_new();

	reader_grammar_by_file(new_parser->reader,"compiler.grammar");

	new_parser->gramm = new_parser->reader->gramm;

	grammar_compute_first(new_parser->gramm);
	grammar_compute_follow(new_parser->gramm);
	grammar_compute_predictive(new_parser->gramm);

	grammar_check_LL(new_parser->gramm);

	return new_parser;
}

void parser_check_lexical(parser* pr){
	char* token_text;

	grammar_start_input(pr->gramm);

	int exit = 0;
	while(1){
		pr->current = lexical_get_token(pr->lexer);

		if(pr->current == NULL){
			pr->expected = grammar_end_input(pr->gramm,&pr->status);
			break;
		}

		pr->expected = grammar_input(pr->gramm,
							pr->current->type_hash,
							&pr->status);

		if(pr->status != GRAMMAR_STATUS_OK){
			token_error_info(pr->current);
			token_text = get_string_of_thash(pr->current->type_hash);
		}


		switch(pr->status){
			case GRAMMAR_STATUS_EMPTY: case GRAMMAR_STATUS_WRONG:
				printf("Token '%s' inesperado\n",token_text);	
				exit = 1;
				break;
			case GRAMMAR_STATUS_UNEXPECT:
				printf("Token '%s' inesperado buscando '%s'\n",token_text,
										get_string_of_thash(pr->expected));
				exit = 1;
				break;

		}
		

		if(exit)
			break;
	}

	if(pr->status == GRAMMAR_STATUS_COMPLETE){
		printf("Gramatica correcta\n");
	}else if(!exit){
		switch(pr->status){
			case GRAMMAR_STATUS_UNEXPECT: case GRAMMAR_STATUS_EMPTY: case GRAMMAR_STATUS_WRONG:
				printf("Ultimo token inesperado\n");
				break;
		}
	}


}

#endif
