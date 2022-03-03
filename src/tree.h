#pragma once
#include "treeADTDef.h"
#include "parserDef.h"
node createEl(int lineNo, int parentSymbolID, int symbolID, int isLeafNode); 
treeN createNode(node el, grammar G);
void addChild(treeN* parent, treeN* new); 
