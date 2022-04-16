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
void updateWidth(int ind, symbolTable* sTable, int width); 
void handleNT(astNode* root, grammar G, symbolTable* sTable, int index, int recInd); 
void handleF(astNode* root, grammar G, symbolTable* sTable, int index); 
void handleM(astNode* root, grammar G, symbolTable* sTable, int index); 
symbolTable* constructST(astNode* root, grammar G); 
int fIndex(char* fName, symbolTable* sTable);
char* getGlobalFName(int lineNo, symbolTable* sTable);
void printGlobals(symbolTable* sTable);
void printAllVars(symbolTable* sTable); 
void printARSizes(symbolTable* sTable); 
void printRUInfo(symbolTable* sTable); 
void printSymbolTable(symbolTable* sTable);
void printGlobalVars(symbolTable* sTable);