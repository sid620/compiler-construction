#include "interimCodeGen.h"
#include <stdlib.h>

void makeThreeAddrCode(char*op, char*arg1, char*arg2, int label, astNode* root){
    ThreeAddrCode* code = (ThreeAddrCode*)malloc(sizeof(ThreeAddrCode));
    code->op = op;
    code->arg1 = arg1;
    code->arg2= arg2;
    code->label = label;
    // code->node = root;
}

void generateCodeForStmt(astNode*root, int stmtType){
    // from the root check for different type of statement 
    switch (stmtType) {
        case 1:{
            // Arithmetic op
            makeThreeAddrCode("ADD","","",0,root); 
            // makeThreeAddrCode("SUB","","",0,root);
        }
        case 2:{
            // Assignment op
        }
    }
}

ThreeAddrCode** generateInterimCode(astNode* root){
    /*
        Traverses the AST and returns the 3 address codes  
    */
    astNode* child = root->child;

    while(child!=NULL){
        // for all statements,
        // generateCodeForStmt()

        child = child->next;
    }
    
}
