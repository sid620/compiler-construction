#pragma once
#include <stdio.h> 
#include <stdbool.h> 
#include "ast.h"

// char* types[] = {"int", "real", "boolean", "rec", "union"}; 

typedef struct entry { 
    char* varName; 
    int present; 
    int lineNo; 
    int offset; 
    int type; // Index of array allTypes 
    
} entry; 

typedef struct typeInfo { 
    char* name; 
    int isParam; 
    int isPrim; 
    int typeId; 
    int width; 
    entry** fields; 
    int numFields; 
    char** aliases; 
    int numAl; 
    int ref; 
} typeInfo;

typedef struct fInfo { 
    char* fId; 
    int numIn; 
    int numOut; 
    int inId; 
    int outId; 
    int* inOrder; 
    int* outOrder; 
} fInfo; 

typedef struct table { 
    entry** entries; // Array of pointers to entries 
    int numEntries; 
    fInfo* function; 
    int currWidth; 
} table; 

typedef struct symbolTable { 
    table** tables; // Array of table* : Array of pointers to arrays of entries i.e even single 
    // entries like 1 variable will have their own entry array with length 1 
    entry** entries; // Global variables 
    int numEntries; 
    int numF; 
    int numTypes; 
    int mainOffset; 
    typeInfo** allTypes; // Pointer to array of typeInfo structs 
} symbolTable; 