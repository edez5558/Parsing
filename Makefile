C := gcc
C_FLAGS := -g

CC = $(C) $(C_FLAGS)

PRE_FOLDER 	:= precompiler/
BIN_FOLDER	:= bin/

PRE_TOKEN 	= $(PRE_FOLDER)token.o 
PRE_LEXICAL	= $(PRE_FOLDER)lexical.o
PRE_GRAMMAR	= $(PRE_FOLDER)grammar.o
PRE_VECTOR 	= $(PRE_FOLDER)vector.o
PRE_STACK	= $(PRE_FOLDER)stack.o
PRE_UTIL	= $(PRE_FOLDER)util.o
PRE_PARSER	= $(PRE_FOLDER)parser.o
PRE_GRAMMFILE	= $(PRE_FOLDER)grammar_file.o

TEST_DEPD	= $(PRE_GRAMMAR) $(PRE_VECTOR) $(PRE_STACK)
GRAMMAR_DEPD 	= $(PRE_VECTOR) $(PRE_STACK)
LEXICAL_DEPD 	= $(PRE_TOKEN) $(PRE_UTIL)
GRAMMFILE_DEPD	= $(PRE_VECTOR) $(PRE_UTIL) $(PRE_GRAMMAR)
PARSER_DEPD	= $(PRE_LEXICAL) $(PRE_TOKEN) $(PRE_GRAMMAR) $(PRE_GRAMMFILE)
COMPILER_DEPD 	= $(PRE_TOKEN) $(PRE_VECTOR) $(PRE_STACK) $(PRE_UTIL) $(PRE_LEXICAL) $(PRE_GRAMMAR) $(PRE_GRAMMFILE) $(PRE_PARSER)

compiler: src/main.c $(COMPILER_DEPD)
	$(CC) src/main.c $(COMPILER_DEPD) -o $(BIN_FOLDER)compiler

$(PRE_TOKEN): src/token.c src/token.h
	$(CC) -c src/token.c -o $(PRE_TOKEN)

$(PRE_LEXICAL): src/lexical.c src/lexical.h $(LEXICAL_DEPD)
	$(CC) -c src/lexical.c -o $(PRE_LEXICAL)

$(PRE_GRAMMAR): src/grammar/grammar.c src/grammar/grammar.h $(GRAMMAR_DEPD)
	$(CC) -c src/grammar/grammar.c -o $(PRE_GRAMMAR)

$(PRE_VECTOR): src/util/vector.c src/util/vector.h
	$(CC) -c src/util/vector.c -o $(PRE_VECTOR)

$(PRE_STACK): src/util/stack.c src/util/stack.h
	$(CC) -c src/util/stack.c -o $(PRE_STACK)

$(PRE_UTIL): src/util/util.c src/util/util.h
	$(CC) -c src/util/util.c -o $(PRE_UTIL)

$(PRE_PARSER): src/parser.c src/parser.h $(PARSER_DEPD)
	$(CC) -c src/parser.c -o $(PRE_PARSER)

$(PRE_GRAMMFILE): src/grammar/grammar_file.c src/grammar/grammar_file.h $(GRAMMFILE_DEPD)
	$(CC) -c src/grammar/grammar_file.c -o $(PRE_GRAMMFILE)

clean:
	rm -r $(PRE_FOLDER)*.o

test_grammar: src/test_gramm.c $(TEST_DEPD)
	$(CC) src/test_gramm.c $(TEST_DEPD) -o $(BIN_FOLDER)generate_grammar

tokens: src/abc.c $(LEXICAL_DEPD) $(PRE_LEXICAL)
	$(CC) src/abc.c $(LEXICAL_DEPD) $(PRE_LEXICAL) -o $(BIN_FOLDER)tokens_grammar

