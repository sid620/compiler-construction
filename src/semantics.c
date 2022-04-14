#include "semantics.h"
/*
    will be called when root corresponds to <declaration>
    index is the symbolTable number of corresponding function
*/
void checkDeclarationsSemantics(astNode *root, symbolTable *sTable, int index, grammar G){
    astNode *temp = findChild(root,1,false,0);  // finds child corresponding to TK_ID
    astNode *temp1 = findChild(root,2,false,0);
    bool isGlobal = strcmp(temp->elem->lex.lexemeStr,"eps")==0? false: true;
    if(!isGlobal){
        int i = searchS(temp->elem->lex.lexemeStr,sTable->tables[index]->entries);
        int j = searchS(temp->elem->lex.lexemeStr,sTable->entries); /* Check if identifier is global*/
        /* Checks if variable is declared multiple times*/
        if(i==-1){
            printf("ERROR: Identifier %s at line number %d is not present in symbolTable\n",temp->elem->lex.lexemeStr,temp->elem->lineNo);
        }
        else if(sTable->tables[index]->entries[i]->count > 1){
            printf("ERROR: Identifier %s at line %d has been declared previously at line %d.\n",sTable->tables[index]->entries[i]->varName,sTable->tables[index]->entries[i]->lineNo,sTable->tables[index]->entries[i]->lineNo);
            (sTable->tables[index]->entries[i]->count)--;
        }
        else if(sTable->tables[index]->entries[i]->lineNo != temp->elem->lineNo){
            printf("ERROR: Identifier %s at line %d has been declared previously at line %d.\n",temp->elem->lex.lexemeStr,temp->elem->lineNo,sTable->tables[index]->entries[i]->lineNo);    
        }
        if (j!=-1){
            printf("ERROR: Global identifier %s at line %d is redeclared at line number %d.\n",sTable->entries[j]->varName,sTable->entries[j]->lineNo,sTable->tables[index]->entries[i]->lineNo);
        }
    }
    else{
        /* Search for the index in global entries */
        int i = searchS(temp->elem->lex.lexemeStr,sTable->entries);

        /* Search for the index in the corresponding symbol table of functions */
        int j = searchS(temp->elem->lex.lexemeStr,sTable->tables[index]->entries);
        if(i == -1){
            printf("ERROR: Global Identifier %s at line number %d is not present in symbolTable\n",temp->elem->lex.lexemeStr,temp->elem->lineNo);            
        }
        else if(sTable->entries[i]->count > 1){ /* Global global redeclaration at same line*/
            printf("ERROR: Global Identifier %s at line %d has been declared previously at line %d\n",sTable->entries[i]->varName,sTable->entries[i]->lineNo,sTable->entries[i]->lineNo);
            (sTable->tables[index]->entries[i]->count)--;
        }
        else if(sTable->entries[i]->lineNo != temp->elem->lineNo){  /* Global global redeclaration at different lines*/
            printf("ERROR: Global Identifier %s at line %d has been declared previously at line %d\n",temp->elem->lex.lexemeStr,temp->elem->lineNo,sTable->entries[i]->lineNo);    
        }
        
    }
    /* Checks if global variable has been declared previously anywhere in the program */
}



void checkFuncCallParameters(astNode *root, symbolTable *sTable, grammar G){
    /*
        traverse the AST and find whereever the function is being called and then check if:
        1. type of input parameter is Union 
    */

    // astNode* child = root->child;
    // astNode* grandchild, *params;
    // while (child != NULL) {
    //     if(child->elem->isLeaf == 0 && strcmp(G.nonTerminals[child->elem->curr], "function") == 0){
    //         grandchild = child->child;
    //         while (grandchild!=NULL){
    //             if(grandchild->elem->isLeaf == 0 && strcmp(G.nonTerminals[grandchild->elem->curr], "input_par") == 0){
    //                 params = grandchild->child; 
    //                 while(params != NULL) { 
    //                     if(params->elem->isLeaf == 1 && strcmp(G.terminals[params->elem->curr], "TK_UNION") == 0){
    //                         printf("ERROR: function expects UNION to function at line %d\n",params->elem->lineNo);
    //                     }
    //                     params = params->next;
    //                 }
    //             }
    //             grandchild = grandchild->next;
    //         }
    //     }
    //     child = child->next; 
    // } 
 
    astNode* child = root->child;
    astNode* grandchild, *inParamList;
    int numInputParams =0;
    while (child != NULL) {
        if(child->elem->isLeaf == 0 && strcmp(G.nonTerminals[child->elem->curr], "function") == 0){
            grandchild = child->child;
            while (grandchild!=NULL){
                if(grandchild->elem->isLeaf == 0 && strcmp(G.nonTerminals[grandchild->elem->curr], "funCallStmt") == 0){
                    inParamList = grandchild->child->next->next->child; // go to inputParameters list
                    while(inParamList != NULL) {    //traverse TK_IDs (inputParameters) 
                        if( (inParamList->elem->isLeaf == 1)  // redundant, should always be true
                            && (inParamList->elem->curr != 0 )  //non epsilon
                            ){
                            if((searchTypes(inParamList->elem->lex.lexemeStr, sTable)!=-1 )  // existence of param being passed in symbolTable
                            && (sTable->allTypes[searchTypes(inParamList->elem->lex.lexemeStr, sTable)]->typeId == 4) // is of type union
                                ) {
                                    printf("ERROR: cannot pass UNION to function at line %d\n",inParamList->elem->lineNo);
                                }
                            numInputParams+=1;
                        }                    
                        inParamList = inParamList->next;
                        if(inParamList==NULL){
                            int tableNum = 0;
                            for(int i=0;i<sTable->numEntries;i++){
                                if(strcmp(sTable->tables[i]->function->fId, grandchild->elem->lex.lexemeStr)==0)
                            }
                            if(numInputParams==)
                        }
                    }
                }
                grandchild = grandchild->next;
            }
        }
        child = child->next; 
    } 

}


/* 
check semantics associated with functionCall statement
the root here corresponds to funCallStmt
*/
void checkFunctionCall(astNode *root, symbolTable *sTable, int index, grammar G){
    /* Find node corresponding to FUNID*/
    astNode *temp = findChild(root,1,false,0);
    /* Stores index corresponding to that function*/
    int i = fIndex(temp->elem->lex.lexemeStr,sTable);

    if(i == -1){
        printf("ERROR: function %s for function call at line %d is not defined.\n",temp->elem->lex.lexemeStr,temp->elem->lineNo);
    }
    else{
        /* Check if recursion is happening*/
        if(index == i){
            printf("ERROR: function call statement at line number: %d is recursive.\n",temp->elem->lineNo);
        }
        /* function definition of function called precedes the function calling it*/
        if(index < i){
            printf("ERROR: function %s called at line number %d is defined after function %s.\n",sTable->tables[i]->function->fId,temp->elem->lineNo,sTable->tables[index]->function->fId);
        }
        /* Check if number of paramters are the same */
        /* outputParams */
        astNode *temp1 = findChild(root,0,false,0);
        /* inputParams */
        astNode *temp2 = findChild(root,2,false,0);
        int inpNumber = 0;
        while(!(temp2.))
    }
}


void checkIterativeStmt(astNode *root, symbolTable *sTable, grammar G){
    astNode* child = root->child;
    while(child!=NULL){
        if() // check <assignmentStmt>
        if() // check <funCallStmt>
        if() // check <ioStmt>
        if() // check <conditionalStmt>
    }
}

void checkOverloading (astNode *root, symbolTable *sTable, grammar G){
    int num_entries = sTable->numEntries;
    while(num_entries--){
        
    }
}