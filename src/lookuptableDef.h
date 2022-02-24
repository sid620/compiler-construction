#ifndef SYMBOLTABLE_DEF
#define SYMBOLTABLE_DEF
#include "lexerDef.h"
#include <stdbool.h>
#define HASH_SIZE 71 
typedef struct{
    char *lexeme;
    token tkn;
    bool present;
    int hashvalue;
}element;

typedef element lookupTable[HASH_SIZE];

#endif