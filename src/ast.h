#pragma once
#include "astDef.h"
void insertAST(astNode *curr, astNode *right);
void addChildAST(astNode *parent, astNode *child);
astNode * mknode(node elem);
// astNode * mkleaf(node *elem);
// In implementation mknode can be used for mkleaf as well