/*
ID  2019A7PS0064P					Name Siddharth Sharma
ID  2019A7PS0062P					Name Atharva Chandak
ID  2019A7PS0133P					Name Archit Bhatnagar 
ID  2019A7PS0554P					Name Suchismita Tripathy
ID  2019A7PS1139P 					Name Srujan Deolasee
*/

#pragma once
#include <stdio.h> 
#include <stdbool.h> 
#include "symbolTable.h" 

bool isLogOp(astNode *root, grammar G); 
bool isRelOp(astNode *root, grammar G); 
int findIDtype(astNode* root, grammar G, symbolTable* sTable, int index); 
int findNestedType(astNode *root, grammar G, symbolTable *sTable, int index); 
int matchReturnParams(astNode *temp, grammar G, symbolTable *sTable, int index); 
int fIndex(char* fName, symbolTable* sTable); 
int typeCheck(astNode* root, grammar G, symbolTable* sTable, int index); 