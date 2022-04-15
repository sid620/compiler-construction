#pragma once
#include "interimCodeGenDef.h"

ThreeAddrCode** generateInterimCode(astNode* root);
ThreeAddrCode** getInterimCode(astNode* root);
ThreeAddrCode* getCodeForStmt(astNode*root);
ThreeAddrCode* makeThreeAddrCode(char*op, char*arg1, char*arg2, int label, astNode* root);