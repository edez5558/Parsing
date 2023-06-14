#include "util.h"

char* sub_string(char* string,size_t left,size_t right){
    if(string == NULL) return NULL;

    size_t len = right - left + 1;

    char* sub = (char*)malloc(sizeof(char) * (len + 1));

    if(sub == NULL){
        return NULL;
    }

    memcpy(sub,string + left,len);
    sub[len] = '\0';
    
    return sub;
}
