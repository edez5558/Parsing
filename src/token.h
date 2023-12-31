#ifndef TOKEN_H_DEFINED
#define TOKEN_H_DEFINED

typedef enum {
	TOKEN_KEYWORD,
	TOKEN_CONSTANT,
	TOKEN_SINGLE,
	TOKEN_COMBINED,
	TOKEN_IDENTIFIER,
	TOKEN_NO
} token_type;

typedef enum {
	TOKEN_KEYWORD_INT,
	TOKEN_KEYWORD_FLOAT,
	TOKEN_KEYWORD_BYTE,
	TOKEN_KEYWORD_CHAR,
	TOKEN_KEYWORD_BOOL,
	TOKEN_KEYWORD_STRING,
	TOKEN_KEYWORD_VOID,
	TOKEN_KEYWORD_OBJECT,
	TOKEN_KEYWORD_NEW,
	TOKEN_KEYWORD_DELETE,
	TOKEN_KEYWORD_IF,
	TOKEN_KEYWORD_ELSE,
	TOKEN_KEYWORD_WHILE,
	TOKEN_KEYWORD_DO,
	TOKEN_KEYWORD_FOR,
	TOKEN_KEYWORD_FOREACH,
	TOKEN_KEYWORD_MAIN,
	TOKEN_KEYWORD_CASE,
	TOKEN_KEYWORD_SWITCH,
	TOKEN_KEYWORD_INCLUDE,
	TOKEN_KEYWORD_ENDIF,
	TOKEN_KEYWORD_RETURN,
	TOKEN_KEYWORD_ELIF,
	TOKEN_KEYWORD_FUNC,
	TOKEN_KEYWORD_CALL,
	TOKEN_CO_NULL,
	TOKEN_CO_TRUE,
	TOKEN_CO_FALSE,
	TOKEN_CO_OUT,
	TOKEN_CO_IN,
	//NO KEYWORD
	TOKEN_CO_VALUE,
	TOKEN_CO_LITERAL,
	TOKEN_CO_NUMBER,
	TOKEN_ID_VALUE,
	//PUNCTUATORS 
	TOKEN_PU_LBRACE = 0,
	TOKEN_PU_RBRACE,
	TOKEN_PU_LBRACKET,
	TOKEN_PU_RBRACKET,
	TOKEN_PU_LPARENTHESIS,
	TOKEN_PU_RPARENTHESIS,
	TOKEN_PU_COMMA,
	TOKEN_PU_DOT,
	TOKEN_PU_SEMICOLON,
	
	//SINGLE OPERATOR
	TOKEN_OP_ASSIGN,
	TOKEN_OP_PLUS,
	TOKEN_OP_MINUS,
	TOKEN_OP_MULT,
	TOKEN_OP_DIV,
	TOKEN_OP_NEG,
	TOKEN_OP_LESS,
	TOKEN_OP_GREATER,
	
	TOKEN_PU_DOUBLEDOT,
	//COMBINED OPERATOR
	TOKEN_OP_ASSIGN_DIST = 0,
	TOKEN_OP_EQUALS,
	TOKEN_OP_EQUAL_GREATER,
	TOKEN_OP_EQUAL_LESS,
	TOKEN_OP_ASSIGN_PLUS,
	TOKEN_OP_ASSIGN_MINUS,
	TOKEN_OP_ASSIGN_MULT,
	TOKEN_OP_ASSIGN_DIV,
	TOKEN_OP_AND,
	TOKEN_OP_OR
} token_value;

typedef struct{
	char* line;
	int column;
	int row;
} token_info;

typedef struct{
	token_type type;
	token_value value;

	token_info info;

	int type_hash;

	void* value_reference;
	void* next_token;
	void* previous_token;
} token;

token* token_init(token_type type,token_value value,void* value_reference,token_info info);

void token_free(token** token_to_delete);

token* token_chain(token* current,token* add);

int token_thash(int type,int value);

void thasht_token(int thash, int* type,int* value);

void token_error_info(token* tkn);

#endif
