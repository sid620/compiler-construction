#ifndef LEXER_DEF
#define LEXER_DEF
#define BUFFER_SIZE 4096
#include "lookuptable.h"
#include<stdio.h>
typedef struct twinBuffer twinBuffer;
element lookup_table[HASH_SIZE];


struct twinBuffer{
    char *buffer[2];
    char *forward;
    char *lexemeBegin;
};

typedef enum token_name{
    // tokens list
    TK_ASSIGNOP
}token_name;

typedef struct TOKEN{
    int line;
    token_name tkn_name;
    union value{
        int num;        // numeric value of int
        struct rnum{
            char *rep;  // string form of reals
            float v;    // numeric value of int
        };
        char * str; // lexeme, identifiers
    };
}tokenInfo;

#endif

/*
To Do:

1. Hash Table/ Symbol Table
2. getnextToken



*/