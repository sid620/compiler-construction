#ifndef LEXER_DEF
#define LEXER_DEF
#define BUFFER_SIZE 4096

typedef struct twinBuffer twinBuffer;
typedef struct TOKEN tokenInfo;


struct twinBuffer{
    char *buffer[2];
    char *forward;
    char *lexemeBegin;
};

typedef enum {
    // tokens list
    TK_ASSIGNOP,
    TK_COMMENT,
    TK_FIELDID,
    TK_ID,
    TK_NUM,
    TK_RNUM,
    TK_FUNID,
    TK_RUID,
    TK_WITH,
    TK_PARAMETERS,
    TK_END,
    TK_WHILE,
    TK_UNION,
    TK_ENDUNION,
    TK_DEFINETYPE,
    TK_AS,
    TK_TYPE,
    TK_MAIN,
    TK_GLOBAL,
    TK_PARAMETER,
    TK_LIST,
    TK_SQL,
    TK_SQR,
    TK_INPUT,
    TK_OUTPUT,
    TK_INT,
    TK_REAL,
    TK_COMMA,
    TK_SEM,
    TK_COLON,
    TK_DOT,
    TK_ENDWHILE,
    TK_OP,
    TK_CL,
    TK_IF,
    TK_THEN,
    TK_ENDIF,
    TK_READ,
    TK_WRITE,
    TK_RETURN,
    TK_PLUS,
    TK_MINUS,
    TK_MUL,
    TK_DIV,
    TK_CALL,
    TK_RECORD,
    TK_ENDRECORD,
    TK_ELSE,
    TK_AND,
    TK_OR,
    TK_NOT,
    TK_LT,
    TK_LE,
    TK_EQ,
    TK_GT,
    TK_GE,
    TK_NE
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
}token;

#endif

/*
To Do:

1. Hash Table/ Symbol Table
2. getnextToken



*/