#pragma once
#include <stdio.h> 

// typedef enum nonTerminalNames { 
//     program, mainFunction, otherFunctions, function, inputPar, outputPar, parameter_list, dataType, 
//     primitiveDatatype, constructedDatatype, remaining_list, stmts, typeDefinitions, actualOrRedefined, 
//     typeDefinition, fieldDefinitions, fieldDefinition, fieldType, moreFields, declarations, declaration, 
//     global_or_not, otherStmts, stmt, assignmentStmt, singleOrRecId, constructedvariable, oneExpansion, 
//     moreExpansions, funCallStmt, outputParameters, inputParameters, iterativeStmt, conditionalStmt, elsePart, 
//     ioStmt, arithmeticExpression, expPrime, term, termPrime, factor, highPrecedenceOperator, 
//     lowPrecedenceOperator, booleanExpression, var, logicalOp, relationalOp, returnStmt, optionalreturn, 
//     idList, more_ids, definetypestmt, A
// } nonTerminalNames; 

// typedef enum terminalNames { 
//     eps, TK_MAIN, TK_END, TK_FUNID, TK_SEM, TK_INPUT, TK_PARAMETER, TK_LIST, TK_SQL, TK_SQR, TK_OUTPUT, TK_ID, 
//     TK_INT, TK_REAL, TK_RECORD, TK_RUID, TK_UNION, TK_COMMA, TK_ENDRECORD, TK_ENDUNION, TK_TYPE, TK_COLON, 
//     TK_FIELDID, TK_GLOBAL, TK_ASSIGNOP, TK_DOT, TK_CALL, TK_WITH, TK_WHILE, TK_OP, TK_CL, TK_ENDWHILE, 
//     TK_IF, TK_THEN, TK_ENDIF, TK_ELSE, TK_READ, TK_WRITE, TK_MUL, TK_DIV, TK_PLUS, TK_MINUS, TK_OP, TK_NUM, 
//     TK_RNUM, TK_AND, TK_OR, TK_LT, TK_GT, TK_LE, TK_GE, TK_NE, TK_EQ, TK_RETURN, TK_DEFINTEYPE, TK_AS
// } terminalNames; 

// typedef union symbol { 
//     terminalNames terminal; 
//     nonTerminalNames nonTerminal; 
// } symbol; 

typedef struct varSymbol { 
    int type; // Type = 0 -> Non Terminal, Type = 1 -> Terminal 
    // symbol symbol; 
    int symbol; 
} varSymbol; 

typedef struct rhsSingular { 
    varSymbol* symbols; 
    int numSyms; 
} rhsSingular; 

typedef struct rule{ 
    int LHS; 
    rhsSingular* RHS; 
    int numOrs; 
    int ruleNumber; 
    int epsilon; // Flag, 1 if epsilon in RHS, else 0 
} rule; 

typedef struct FirstAndFollow{ 
    int* numFirst; 
    int numFollow; 
    int** first; // Not using varSymbol* as they will be only terminals 
    int* follow; // Not using varSymbol* as they will be only terminals 
} FirstAndFollow; 

typedef struct grammar{ 
    rule* allRules; 
    int totalNumRules; 
    char** nonTerminals; 
    char** terminals; 
    int numNonTerminals; 
    int numTerminals; 
    FirstAndFollow* ff; 
} grammar; 

// typedef struct parseTable{ 

// } parseTable; 

// typedef struct parseTree{ 

// } parseTree; 

