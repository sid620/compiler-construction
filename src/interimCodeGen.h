#pragma once
#include "interimCodeGenDef.h"

ThreeAddrCode** generateInterimCode(astNode* root);
void generateCodeForStmt(astNode*root, int stmtType);
void makeThreeAddrCode(char*op, char*arg1, char*arg2, int label, astNode* root);