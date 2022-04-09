#pragma once
#include "astDef.h"
void insertAST(astNode *curr, astNode *right);
void addChildAST(astNode *parent, astNode *child);
astNode * mknode(node elem, grammar G);
void constructAst(astNode *node, treeN *root, grammar G, int *insertPrev, astNode *mainRoot);
astNode *findChild(astNode *node, int position, bool insertUsed, int insertCount);
void performAction(int ruleNumber, astNode *node, treeN *root, grammar G, bool *insertUsed, int *insertCount);
void printAST(astNode *node, grammar G);
// astNode * mkleaf(node *elem);
// In implementation mknode can be used for mkleaf as well