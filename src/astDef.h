/*
ID  2019A7PS0064P					Name Siddharth Sharma
ID  2019A7PS0062P					Name Atharva Chandak
ID  2019A7PS0133P					Name Archit Bhatnagar 
ID  2019A7PS0554P					Name Suchismita Tripathy
ID  2019A7PS1139P 					Name Srujan Deolasee
*/

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
