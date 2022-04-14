#pragma once
#include "astDef.h"

// List of operations supported

char* operations[] = {
    "ADD", "SUB", "MUL", "DIV", // arithmetic
    "GT", "GTE","LT", "LTE", "EQ", "NE", // relational 
    "AND", "OR", // boolean
    // "GOTO","LABEL", "ASSIGN", "CALL", "RETURN"
};

// Three address code struct
typedef struct ThreeAddrCode ThreeAddrCode;
struct ThreeAddrCode {
    char* arg1; //first argument 
    char* op;   //operation type 
    char* arg2;  //second argument
    char* res;  // result 
    int label;
    // astNode* node;
};

// TODO:temporaries