#ifndef LEXER_DEF
#define LEXER_DEF
#define BUFFER_SIZE 4096
#define MAX_LEXEME 40
// #include "lookuptable.h"
#include<stdio.h>
#include<stdlib.h>
typedef struct twinBuffer twinBuffer;



struct twinBuffer{
    char *buffer;
    int forward;
    int lexemeBegin;
    int pos;    // position at which character should be added to lexeme
    char *lexeme;
};
int lexError, lineNo;
/*
0- no error
1- unknown symbol
2- unknown pattern
3- length exceeded  
*/
typedef enum token_name{
    // tokens list
    eps,
    TK_MAIN,
    TK_END,
    TK_FUNID,
    TK_SEM,
    TK_INPUT,
    TK_PARAMETER,
    TK_LIST,
    TK_SQL,
    TK_SQR,
    TK_OUTPUT,
    TK_ID,
    TK_INT,
    TK_REAL,
    TK_RECORD,
    TK_RUID,
    TK_UNION,
    TK_COMMA,
    TK_ENDRECORD,
    TK_ENDUNION,
    TK_TYPE,
    TK_COLON,
    TK_FIELDID,
    TK_GLOBAL,
    TK_ASSIGNOP,
    TK_DOT,
    TK_CALL,
    TK_WITH,
    TK_WHILE,
    TK_OP,
    TK_CL,
    TK_ENDWHILE,
    TK_IF,
    TK_THEN,
    TK_ENDIF,
    TK_ELSE,
    TK_COMMENT,
    TK_READ,
    TK_WRITE,
    TK_MUL,
    TK_DIV,
    TK_PLUS,
    TK_MINUS,
    TK_NUM,
    TK_RNUM,
    TK_AND,
    TK_OR,
    TK_LT,
    TK_GT,
    TK_LE,
    TK_GE,
    TK_NE,
    TK_EQ,
    TK_RETURN,
    TK_DEFINETYPE,
    TK_AS,
    TK_EOF,
    TK_PARAMETERS,
    TK_NOT,
    ERROR,   // special token when error is encountered
    
}token_name;

static const char * enumToString[]={"eps", "TK_MAIN", "TK_END", "TK_FUNID", "TK_SEM", "TK_INPUT", "TK_PARAMETER", "TK_LIST", "TK_SQL", "TK_SQR", "TK_OUTPUT", "TK_ID", 
    "TK_INT", "TK_REAL", "TK_RECORD", "TK_RUID", "TK_UNION", "TK_COMMA", "TK_ENDRECORD", "TK_ENDUNION", "TK_TYPE", "TK_COLON", 
    "TK_FIELDID", "TK_GLOBAL", "TK_ASSIGNOP", "TK_DOT", "TK_CALL", "TK_WITH", "TK_WHILE", "TK_OP", "TK_CL", "TK_ENDWHILE", 
    "TK_IF", "TK_THEN", "TK_ENDIF", "TK_ELSE", "TK_READ", "TK_WRITE", "TK_MUL", "TK_DIV", "TK_PLUS", "TK_MINUS", "TK_NUM", 
    "TK_RNUM", "TK_AND", "TK_OR", "TK_LT", "TK_GT", "TK_LE", "TK_GE", "TK_NE", "TK_EQ", "TK_RETURN", "TK_DEFINETYPE", "TK_AS", "TK_EOF","TK_PARAMETERS","TK_NOT","ERROR"};
typedef struct TOKEN{
    int line;
    token_name tkn_name;
    union value{
        int num;        // numeric value of int
        struct rnum{
            char *rep;  // string form of reals
            float v;    // numeric value of int
        }rnum;
        char * str; // lexeme, identifiers
    }value;
}tokenInfo;
twinBuffer *twin_buffer;    // buffer the lexer
#endif

/*
To Do:

1. Hash Table/ Symbol Table
2. getnextToken



*/