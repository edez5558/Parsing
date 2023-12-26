#ifndef PARSER_DEFINED_H
#define PARSER_DEFINED_H

#include "lexical.h"
#include "token.h"
#include "grammar/grammar.h"
#include "grammar/grammar_file.h"

#define GRAMMAR_FILE "compiler.grammar"

typedef struct{
	lexical* lexer;
	grammar* gramm;

	int status;
	int expected;
	token* current;

	reader_grammar* reader;
} parser;

parser* parser_new(lexical* lexer);

void parser_free(parser** psr);

void parser_check_lexical(parser* pr);

#endif
