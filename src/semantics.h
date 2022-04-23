/*
ID  2019A7PS0064P					Name Siddharth Sharma
ID  2019A7PS0062P					Name Atharva Chandak
ID  2019A7PS0133P					Name Archit Bhatnagar 
ID  2019A7PS0554P					Name Suchismita Tripathy
ID  2019A7PS1139P 					Name Srujan Deolasee
*/

#pragma once
#include "ast.h"
#include "symbolTable.h"
#include "typechecker.h"
#include "parser.h"
void checkMultipleDeclarations(astNode *root, symbolTable *sTable, int index, grammar G);/*OK*/
void checkFunctionCall(astNode *root, symbolTable *sTable,int index, grammar G, int **isOutAssigned);
void checkIterativeStmt(astNode *root, symbolTable *sTable, int index, grammar G,int **isOutAssigned);
void checkOverloading(astNode *root, symbolTable *sTable, grammar G, int index);
void checkFunction(astNode *root, symbolTable *sTable, grammar G, int index);
bool isOutputParams(char *str,symbolTable *sTable,int index, grammar G);
void loadBooleanExpressionVariables(astNode *root,grammar G,char ***boolExpVariables,int *numberOfBoolExpVariables);
void getVarName(char **name, astNode *root, grammar G, symbolTable *sTable, int index);
void checkConditionalInIterative(astNode *root,symbolTable *sTable,int index,grammar G,int **isOutAssigned,char ***boolExpVariables,int *numberOfBoolExpVariables, bool **isChanged);
void semanticCheck(astNode *root, symbolTable *sTable, grammar G);
void checkConditionalStmt(astNode *root, symbolTable *sTable,int index,grammar G, int **isOutAssigned);
bool isOutputParams(char *str, symbolTable *sTable, int index, grammar G);  /*OK*/
void checkIdentifierScope(astNode *root,grammar G,symbolTable *sTable, int index);
void checkBoolExprScope(astNode *root, symbolTable *sTable,int index,grammar G);
void checkArithExprScope(astNode *root,symbolTable *sTable, int index, grammar G);
void checkIdListScope(astNode *root,symbolTable *sTable, int index, grammar G);
void checkNestedIterative(astNode *root,symbolTable *sTable,int index,grammar G,int **isOutAssigned,char ***boolExpVariablesComplete,int *numberOfBoolExpVariablesTotal,bool **isChanged);