#pragma once
#include<stdbool.h>
#include "parser.h"
typedef struct astNode astNode;
struct astNode{
    astNode *next;
    astNode *child;
    // astNode **children;
    // int numChildren;
    node *elem;
};
