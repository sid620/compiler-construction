#include "treeADTDef.h"

node createEl(int lineNo, int parentSymbolID, int symbolID, int isLeafNode); 
treeN createNode(node el);
void addChild(treeN* parent, treeN* new); 
