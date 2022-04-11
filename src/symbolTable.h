#pragma once
#include <stdio.h> 
#include <stdbool.h> 
#include "ast.h"
#include "symbolTableDef.h" 

int hashS(char* lexeme, int i); 
int searchS(char* lexeme, entry** hashT); 
int insertS(entry* new, entry** hashT); 
entry** initialiseS(); 
entry* makeEntry(char* lexeme, int type, int offset, int lineNo); 
void insertType(typeInfo* new, symbolTable* sTable); 
int searchTypes(char* name, symbolTable* sTable); 
void handleNT(astNode* root, grammar G, symbolTable* sTable, int index, int recInd); 
void handleF(astNode* root, grammar G, symbolTable* sTable, int index); 
void handleM(astNode* root, grammar G, symbolTable* sTable, int index); 
symbolTable* constructST(astNode* root, grammar G); 