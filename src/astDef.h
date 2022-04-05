#pragma once
#include<stdbool.h>
#include "treeADTDef.h"
typedef struct astNode{
    astNode *next;
    astNode *child;
    // astNode **children;
    int numChildren;
    node *elem;
}astNode;