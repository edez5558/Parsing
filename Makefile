PRE_FOLDER 	= precompiler/
PRE_TOKEN 	= $(PRE_FOLDER)token.o 
PRE_LEXICAL	= $(PRE_FOLDER)lexical.o
PRE_GRAMMAR	= $(PRE_FOLDER)grammar.o
PRE_VECTOR 	= $(PRE_FOLDER)vector.o
PRE_STACK	= $(PRE_FOLDER)stack.o
PRE_UTIL	= $(PRE_FOLDER)util.o

TEST_DEPD	= $(PRE_GRAMMAR) $(PRE_VECTOR) $(PRE_STACK)
GRAMMAR_DEPD 	= $(PRE_VECTOR) $(PRE_STACK)
LEXICAL_DEPD 	= $(PRE_TOKEN) $(PRE_UTIL)
COMPILER_DEPD 	= $(PRE_GRAMMAR) $(PRE_LEXICAL) $(LEXICAL_DEPD) $(GRAMMAR_DEPD)

$(PRE_TOKEN): src/token.c src/token.h
	gcc -g -c src/token.c -o $(PRE_TOKEN)

$(PRE_LEXICAL): src/lexical.c src/lexical.h precompiler/token.o
	gcc -g -c src/lexical.c precompiler/token.o -o $(PRE_LEXICAL)

$(PRE_GRAMMAR): src/grammar/grammar.c $(GRAMMAR_DEPD)
	gcc -g -c src/grammar/grammar.c $(GRAMMAR_DEPD) -o $(PRE_GRAMMAR)

$(PRE_VECTOR): src/util/vector.c
	gcc -g -c src/util/vector.c -o $(PRE_VECTOR)

$(PRE_STACK): src/util/stack.c
	gcc -g -c src/util/stack.c -o $(PRE_STACK)
$(PRE_UTIL): src/util/util.c
	gcc -g -c src/util/util.c -o $(PRE_UTIL)

clean:
	rm -r $(PRE_FOLDER)*.o

compiler: src/main.c $(COMPILER_DEPD)
	gcc src/main.c $(COMPILER_DEPD) -o bin/compiler

test_grammar: src/test_gramm.c $(TEST_DEPD)
	gcc -g src/test_gramm.c $(TEST_DEPD) -o bin/generate_grammar

tokens: src/abc.c $(LEXICAL_DEPD) $(PRE_LEXICAL)
	gcc -g src/abc.c $(LEXICAL_DEPD) $(PRE_LEXICAL) -o bin/tokens_grammar

