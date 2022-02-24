#ifndef LOOKUPTABLE_DEF
#define LOOKUPTABLE_DEF

#include <stdbool.h>
#include "string.h"
#define HASH_SIZE 101 
#define PRIME 31
typedef struct{
    char *lexeme;
    bool present;
}element;

typedef element lookupTable[HASH_SIZE];

#endif