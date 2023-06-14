
#include <stdio.h>
#include <string.h>

#include "lexical.h"
#include "parser.h"

#define LIMIT_LINE 1024

int main(int argc, char* argv[]){
    FILE* source = NULL;
    char buffer[LIMIT_LINE];
    lexical* lexer = lexical_new();
    parser* prs = NULL;

    if(argc < 2){
        printf("Parsing of programming language \"Kok\"\nInsert '\\' at the end to finish write\n");

        int column = 1;

        char* search_end_ptr;
	    do{
            printf(">>");
            fgets(buffer,LIMIT_LINE,stdin);

            search_end_ptr = strrchr(buffer,'\\');

            if(search_end_ptr != NULL){
                *search_end_ptr = '\0';
            }else{
                buffer[strlen(buffer) - 1] = '\0';
            }

            lexical_process_line(lexer,buffer,strlen(buffer),column);

            column++;

	    }while(search_end_ptr == NULL);

    }else{
        source = fopen(argv[1],"r");

        if(source == NULL){
            puts("Error: load source code");
            return -1;
        }

        int column = 1;
        while(fgets(buffer,LIMIT_LINE,source)){
            buffer[strcspn(buffer,"\n")] = 0;
            lexical_process_line(lexer,buffer,strlen(buffer),column);

            column++;
        }
    }

    prs = parser_new(lexer);

    parser_check_lexical(prs);
    lexical_free(&lexer);
    return 0;
}
