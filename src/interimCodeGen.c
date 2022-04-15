#include "interimCodeGen.h"
#include <stdlib.h>

ThreeAddrCode* makeThreeAddrCode(char*op, char*arg1, char*arg2, int label, astNode* root){
    ThreeAddrCode* code = (ThreeAddrCode*)malloc(sizeof(ThreeAddrCode));
    code->op = op;
    code->arg1 = arg1;
    code->arg2= arg2;
    code->label = label;
    // code->node = root;
    return code;
}

ThreeAddrCode* getCodeForStmt(astNode*root){
    // from the root check for different type of statement 
    switch (root->elem->ruleNumber) {
        case 45:{


            // <assignmentStmt>
            makeThreeAddrCode("ADD","","",0,root); 
            // makeThreeAddrCode("SUB","","",0,root);
        }
        break;

        case 2:{
            // 
        }
        break;
    }
}

ThreeAddrCode** getInterimCode(astNode* root){
    
    ThreeAddrCode* code = getCodeForStmt(root);
    if (code)
    
}

ThreeAddrCode** generateInterimCode(astNode* root){
    /*
        Traverses the AST and returns the 3 address codes 
    */
    astNode* child = root->child;

    // get TAC for each function
    while(child!=NULL){
        // for all nodes
        getInterimCode(child);
        child = child->next;
    }
    
}
