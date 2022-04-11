#include "tree.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

node createEl(int lineNo, int parentSymbolID, int symbolID, int isLeafNode, int expansionRule) { 
    node new; 
    new.lineNo = lineNo; 
    new.parentNodeSymbolID = parentSymbolID; 
    new.curr = symbolID; 
    new.isLeaf = isLeafNode; 
    new.ruleNumber = expansionRule;
    if (isLeafNode == 0) { 
        new.lex.lexemeStr = "____"; 
    } 
    return new; 
}

treeN createNode(node el, grammar G) {
    treeN new; 

    new.children = NULL; 
    new.numChild = 0; 
    new.elem.curr = el.curr; 
    new.elem.isLeaf = el.isLeaf; 
    if(strcmp(G.terminals[el.curr],"TK_NUM")==0)
        new.elem.lex.numVal=el.lex.numVal;
    else if(strcmp(G.terminals[el.curr],"TK_RNUM")==0)
        new.elem.lex.rVal=el.lex.rVal;
    else{
        // printf("inside create node when not rnum/num %d\n",new.elem.curr);
        new.elem.lex.lexemeStr=el.lex.lexemeStr;
    }
    // new.elem.lex = el.lex; 
    new.elem.lineNo = el.lineNo; 
    new.elem.parentNodeSymbolID = el.parentNodeSymbolID;
    new.elem.ruleNumber = el.ruleNumber;
    // printf("%d \n", new.elem.curr);
    return new;
}

void addChild(treeN* parent, treeN* new) {

    // printf("%u \n", parent); 
    if (parent->numChild != 0) { 
        // printf("%u %d %u \n", parent, parent->numChild, parent->children[0]); 
        parent->children = (treeN**) realloc(parent->children, sizeof(treeN) * (parent->numChild + 1)); 
        parent->children[parent->numChild] = new; 
        parent->numChild += 1; 
        // printf("%u %u \n", new, parent->children[0]); 
    } 
    else { 
        parent->children = (treeN**) malloc(sizeof(treeN)); 
        parent->children[0] = new; 
        parent->numChild = 1; 
        // printf("%u %u \n", new, parent->children[0]); 
    } 
        
} 
