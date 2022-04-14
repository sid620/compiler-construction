#pragma once
#include "ast.h"
#include "symbolTable.h"
#include "typechecker.h"
void checkMultipleDeclarations(astNode *root, symbolTable *sTable, int index, grammar G);
void checkFuncCallParameters(astNode *root, symbolTable *sTable, grammar G);
void checkFunctionCall(astNode *root, symbolTable *sTable,int index, grammar G);
void checkIterativeStmt(astNode *root, symbolTable *sTable, grammar G);
void checkOverloading (astNode *root, symbolTable *sTable, grammar G);