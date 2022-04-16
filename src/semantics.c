#include "semantics.h"
/*root corresponds to idList*/
void checkIdListScope(astNode *root,symbolTable *sTable, int index, grammar G){
    while(!(root->elem->isLeaf && strcmp(G.terminals[root->elem->curr],"eps")==0)){
        checkIdentifierScope(root,G,sTable,index);
        root = root->next;
    }
}
/*root is arithmeticExpression*/
void checkArithExprScope(astNode *root,symbolTable *sTable, int index, grammar G){
    root = root->child;
    while(root!=NULL){
        if(root->elem->isLeaf && strcmp(G.terminals[root->elem->curr],"TK_NUM")==0){
        // printf("current root in arithExprScope %d at line %d\n",root->elem->lex.numVal,root->elem->lineNo);
        }
        else if(root->elem->isLeaf && strcmp(G.terminals[root->elem->curr],"TK_RNUM")==0){
        // printf("current root in arithExprScope %f at line %d\n",root->elem->lex.rVal,root->elem->lineNo);
        }
        else if(root->elem->isLeaf && strcmp(G.terminals[root->elem->curr],"TK_ID")==0){
        // printf("current root in arithExprScope %s at line %d\n",root->elem->lex.lexemeStr,root->elem->lineNo);
        }
        if(root->elem->isLeaf && strcmp(G.terminals[root->elem->curr],"TK_ID")==0){
            // printf("Identifier in ArithExprScope %s\n",root->elem->lex.lexemeStr);
            checkIdentifierScope(root,G,sTable,index);
            while(!(root->elem->isLeaf && strcmp(G.terminals[root->elem->curr],"eps")==0)){
                root = root->next;
            }
        }
        if(root->elem->isLeaf==0 && strcmp(G.nonTerminals[root->elem->curr],"arithmeticExpression")==0){
            checkArithExprScope(root,sTable,index,G);
        }
        root = root->next;
    }
}
/*root is boolExpression*/
void checkBoolExprScope(astNode *root, symbolTable *sTable,int index,grammar G){
    astNode *temp = findChild(root,0,false,0);
    // printf("bool expression %s rule number %d\n",G.nonTerminals[root->elem->curr],root->elem->ruleNumber);
    // printf("Entered boolExpr Scope %s \n",G.terminals[temp->next->next->elem->curr]);
    if(temp->elem->isLeaf && strcmp(G.terminals[temp->elem->curr],"TK_NOT")==0){
        temp = temp->next;
        checkBoolExprScope(temp,sTable,index,G);
    }
    else if(temp->next->elem->isLeaf && isLogOp(temp->next,G)){
        checkBoolExprScope(temp,sTable,index,G);
        checkBoolExprScope(temp->next->next,sTable,index,G);
    }
    else{
        /*check first <var>*/
        checkIdentifierScope(temp,G,sTable,index);
        while(!(temp->elem->isLeaf && strcmp(G.terminals[temp->elem->curr],"eps")==0)){
            temp = temp->next;
        }
        /*node corresponding to second <var>*/
        // printf("BoolExprScope terminal %s at line %d\n",G.terminals[temp->next->elem->curr],temp->next->elem->lineNo);
        /*corresponds to second <var>*/
        temp = temp->next->next;
        checkIdentifierScope(temp,G,sTable,index);
    }
    
}
/* root corresponds to <var> */
void checkIdentifierScope(astNode *root,grammar G,symbolTable *sTable, int index){
    // printf("Identifier name %s\n",root->elem->lex.lexemeStr);
    if(!(root->elem->isLeaf && (strcmp(G.terminals[root->elem->curr],"TK_NUM")==0 || strcmp(G.terminals[root->elem->curr],"TK_RNUM")==0))){
        // printf("Function name %s\n",sTable->tables[index]->function->fId);
        int j = -1;
        int numIn = sTable->tables[index]->function->numIn;
        int numOut = sTable->tables[index]->function->numOut;
        int inId = sTable->tables[index]->function->inId;
        int outId = sTable->tables[index]->function->outId;
        if(sTable->tables[index]->entries!=NULL){
            j = searchS(root->elem->lex.lexemeStr,sTable->tables[index]->entries);
            // if(j==-1){
            //     printf("ERROR: Identifier %s used at line %d is out of scope.\n",root->elem->lex.lexemeStr,root->elem->lineNo);
            // }
        }
        if(j==-1){
            /*check in input params of function*/
            if(numIn>0)
            j = searchS(root->elem->lex.lexemeStr,sTable->allTypes[inId]->fields);
        }
        if(j==-1){
            /*check in output params of function*/
            if(numOut>0)
            j = searchS(root->elem->lex.lexemeStr,sTable->allTypes[outId]->fields);
        }
        if(j==-1){
            /*check in global*/
            if(sTable->numEntries!=0)
            j = searchS(root->elem->lex.lexemeStr,sTable->entries);
            if(j==-1)
            printf("ERROR: Identifier %s used at line %d is out of scope.\n",root->elem->lex.lexemeStr,root->elem->lineNo);
        }
    }
    
}
/*root corresponds to <singleOrRecId>*/
void getVarName(char **name, astNode *root, grammar G, symbolTable *sTable, int index){
    int l = 0;
    astNode *temp = root;
    while(!(temp->elem->isLeaf && strcmp(G.terminals[temp->elem->curr],"eps")==0)){
        l+=strlen(temp->elem->lex.lexemeStr);
        temp = temp->next;
        if(strcmp(G.terminals[temp->elem->curr],"eps")!=0)l++;  // adding character for dot
    }
    char str[l+1];
    int k = 0;
    temp = root;
    while(!(temp->elem->isLeaf && strcmp(G.terminals[temp->elem->curr],"eps")==0)){
        for(int h=k;h<k+strlen(temp->elem->lex.lexemeStr);h++){
            str[h]=temp->elem->lex.lexemeStr[h-k];
        }
        k+=strlen(temp->elem->lex.lexemeStr);
        temp = temp->next;
        if(strcmp(G.terminals[temp->elem->curr],"eps")!=0){
            str[k]='.';
            k++;
        }
    }
    str[l]='\0';
    *name = (char *)malloc((l+1)*sizeof(char));
    strcpy(*name,str);
    // printf("getVarName %s\n",*name);
}
/*root corresponds to <boolExpression>*/
void loadBooleanExpressionVariables(astNode *root,grammar G,char ***boolExpVariables,int *numberOfBoolExpVariables){
    astNode *temp = findChild(root,0,false,0);
    // printf("loadBoolExpr node %s\n",G.terminals[temp->elem->curr]);
    // printf("loadBoolExpr node %s\n",G.nonTerminals[temp->elem->curr]);
    if(temp->elem->isLeaf && strcmp(G.terminals[temp->elem->curr],"TK_NOT")==0){
        // printf("Hello\n");
        temp = temp->next;
        loadBooleanExpressionVariables(temp,G,boolExpVariables,numberOfBoolExpVariables);
    }
    else if(temp->next->elem->isLeaf && isLogOp(temp->next,G)){
        printf("Line number %d",temp->elem->lineNo);
        loadBooleanExpressionVariables(temp,G,boolExpVariables,numberOfBoolExpVariables);
        loadBooleanExpressionVariables(temp->next->next,G,boolExpVariables,numberOfBoolExpVariables);
    }
    else {
        // astNode *temp1 = findChild(root,2,false,0);
        /* Is of type <var> */
        /* Total length of identifier including dots*/
        int l = 0;
        // printf("Identifier in loadBoolean %s\n",G.nonTerminals[temp->elem->curr]);
        if(temp->elem->isLeaf && (strcmp(G.terminals[temp->elem->curr],"TK_NUM")==0 || strcmp(G.terminals[temp->elem->curr],"TK_RNUM")==0)){

        }
        else{
            while(!(temp->elem->isLeaf && strcmp(G.terminals[temp->elem->curr],"eps")==0)){
                l+=strlen(temp->elem->lex.lexemeStr);
                if(strcmp(G.terminals[temp->next->elem->curr],"eps")!=0){
                    l++;
                }
                temp = temp->next;
            }
            if(*numberOfBoolExpVariables == 0){
                /*corresponding to first <var>*/
                temp = findChild(root,0,false,0);
                *boolExpVariables = (char **)malloc(((*numberOfBoolExpVariables)+1)*sizeof(char *));
                (*boolExpVariables)[*numberOfBoolExpVariables] = (char *)malloc((l+1)*sizeof(char));
                char str[l+1]; 
                int h = 0;

                while(!(temp->elem->isLeaf && strcmp(G.terminals[temp->elem->curr],"eps")==0)){
                    for(int g = h; g<strlen(temp->elem->lex.lexemeStr)+h;g++){
                        str[g]=temp->elem->lex.lexemeStr[g-h];
                    }
                    h+=strlen(temp->elem->lex.lexemeStr);
                    if(strcmp(G.terminals[temp->next->elem->curr],"eps")!=0){
                        str[h] = '.';
                        h++;
                    }
                    temp = temp->next;
                }
                str[l]='\0';
                strcpy((*boolExpVariables)[*numberOfBoolExpVariables],str);
                (*numberOfBoolExpVariables)++;
                // printf("String generated %s booleanVariablesCount %d\n",(*boolExpVariables)[(*numberOfBoolExpVariables)-1],*numberOfBoolExpVariables);
            } 
            else{
                temp = findChild(root,0,false,0);
                *boolExpVariables = (char **)realloc(*boolExpVariables,((*numberOfBoolExpVariables)+1)*sizeof(char *));
                (*boolExpVariables)[*numberOfBoolExpVariables] = (char *)malloc((l+1)*sizeof(char));
                char str[l+1];
                int h = 0;

                while(!(temp->elem->isLeaf && strcmp(G.terminals[temp->elem->curr],"eps")==0)){
                    for(int g = h; g<strlen(temp->elem->lex.lexemeStr)+h;g++){
                        str[g]=temp->elem->lex.lexemeStr[g-h];
                    }
                    h+=strlen(temp->elem->lex.lexemeStr);
                    if(strcmp(G.terminals[temp->next->elem->curr],"eps")!=0){
                        str[h] = '.';
                        h++;
                    }
                    temp = temp->next;
                }
                str[l]='\0';
                strcpy((*boolExpVariables)[*numberOfBoolExpVariables],str);
                (*numberOfBoolExpVariables)++;
                // printf("String generated %s booleanVariablesCount %d\n",(*boolExpVariables)[(*numberOfBoolExpVariables)-1],*numberOfBoolExpVariables);
            }
            temp = temp->next;
        }
        /*temp is currently pointing to relop*/
        temp = temp->next;
        /*Now pointing to second var*/
        astNode *temp1 = temp;
        l = 0;
        // printf("identifier of second <var> %s at line %d\n",G.terminals[temp->elem->curr],temp->elem->lineNo);
        if(temp->elem->isLeaf && (strcmp(G.terminals[temp->elem->curr],"TK_NUM")==0 || strcmp(G.terminals[temp->elem->curr],"TK_RNUM")==0)){
            // printf("Yes is num/rnum\n");
        }
        else{
            while(!(temp->elem->isLeaf && strcmp(G.terminals[temp->elem->curr],"eps")==0)){
                l+=strlen(temp->elem->lex.lexemeStr);
                if(strcmp(G.terminals[temp->next->elem->curr],"eps")!=0){
                    l++;
                }
                temp = temp->next;
            }
            if(*numberOfBoolExpVariables == 0){
                temp = temp1;
                *boolExpVariables = (char **)malloc(((*numberOfBoolExpVariables)+1)*sizeof(char *));
                (*boolExpVariables)[*numberOfBoolExpVariables] = (char *)malloc((l+1)*sizeof(char));
                char str[l+1]; 
                int h = 0;

                while(!(temp->elem->isLeaf && strcmp(G.terminals[temp->elem->curr],"eps")==0)){
                    for(int g = h; g<strlen(temp->elem->lex.lexemeStr)+h;g++){
                        str[g]=temp->elem->lex.lexemeStr[g-h];
                    }
                    h+=strlen(temp->elem->lex.lexemeStr);
                    if(strcmp(G.terminals[temp->next->elem->curr],"eps")!=0){
                        str[h] = '.';
                        h++;
                    }
                    temp = temp->next;
                }
                str[l]='\0';
                strcpy((*boolExpVariables)[*numberOfBoolExpVariables],str);
                (*numberOfBoolExpVariables)++;
                // printf("String generated %s booleanVariablesCount %d\n",(*boolExpVariables)[(*numberOfBoolExpVariables)-1],*numberOfBoolExpVariables);
            } 
            else{
                temp = temp1;
                *boolExpVariables = (char **)realloc(*boolExpVariables,((*numberOfBoolExpVariables)+1)*sizeof(char *));
                (*boolExpVariables)[*numberOfBoolExpVariables] = (char *)malloc((l+1)*sizeof(char));
                char str[l+1];
                int h = 0;

                while(!(temp->elem->isLeaf && strcmp(G.terminals[temp->elem->curr],"eps")==0)){
                    for(int g = h; g<strlen(temp->elem->lex.lexemeStr)+h;g++){
                        str[g]=temp->elem->lex.lexemeStr[g-h];
                    }
                    h+=strlen(temp->elem->lex.lexemeStr);
                    if(strcmp(G.terminals[temp->next->elem->curr],"eps")!=0){
                        str[h] = '.';
                        h++;
                    }
                    temp = temp->next;
                }
                str[l]='\0';
                strcpy((*boolExpVariables)[*numberOfBoolExpVariables],str);
                (*numberOfBoolExpVariables)++;
                // printf("String generated %s booleanVariablesCount %d\n",(*boolExpVariables)[(*numberOfBoolExpVariables)-1],*numberOfBoolExpVariables);
            }
            /*completed reading second <var>*/
        }
    }
}
/*root corresponds to conditional*/
void checkConditionalInIterative(astNode *root,symbolTable *sTable,int index,grammar G,int **isOutAssigned,char ***boolExpVariables,int *numberOfBoolExpVariables, bool **isChanged){
    /*Check scope corresponding to boolExpr*/
    astNode *temp = findChild(root,1,false,0);
    // printf("non-terminal before boolExprScope %s\n",G.nonTerminals[temp->elem->curr]);
    checkBoolExprScope(temp,sTable,index,G);
    /*Node corresponding to first stmt*/
    temp = findChild(root,3,false,0);

    while(!(temp->elem->isLeaf && strcmp(G.terminals[temp->elem->curr],"eps")==0)){
        if(strcmp(G.nonTerminals[temp->elem->curr],"ioStmt")==0){
            if(strcmp(G.terminals[temp->child->elem->curr],"TK_WRITE")==0){
                /* get var name */
                astNode *temp1 = findChild(temp,1,false,0);
                char **name = (char **)malloc(sizeof(char *));
                getVarName(name,temp1,G,sTable,index);
                for(int k = 0;k<(*numberOfBoolExpVariables);k++){
                    if(strcmp(*name,(*boolExpVariables)[k])==0){
                        (*isChanged)[k]=true;
                    }
                }
                /*checking if outputs are assigned*/
                int outId = sTable->tables[index]->function->outId;
                int outNum = sTable->tables[index]->function->numOut;
                for(int k=0;k<outNum;k++){
                    int outFieldNumber = sTable->tables[index]->function->outOrder[k];
                    if(strcmp(sTable->allTypes[outId]->fields[outFieldNumber]->varName,temp1->elem->lex.lexemeStr)==0){
                        (*isOutAssigned)[k]=1;
                    }
                }
            }
            /*Check identifier scope*/
            astNode *temp1 = findChild(temp,1,false,0);
            checkIdentifierScope(temp1,G,sTable,index);
        }
        if(strcmp(G.nonTerminals[temp->elem->curr],"assignmentStmt")==0){
            /*Node corresponding to singleOrRecId*/
            astNode *temp1 = findChild(temp,0,false,0);
            char **name = (char **)malloc(sizeof(char *));
            getVarName(name,temp1,G,sTable,index);
            for(int k = 0;k<(*numberOfBoolExpVariables);k++){
                if(strcmp(*name,(*boolExpVariables)[k])==0){
                    (*isChanged)[k]=true;
                }
            }
            int outId = sTable->tables[index]->function->outId;
            int outNum = sTable->tables[index]->function->numOut;
            for(int k=0;k<outNum;k++){
                int outFieldNumber = sTable->tables[index]->function->outOrder[k];
                if(strcmp(sTable->allTypes[outId]->fields[outFieldNumber]->varName,temp1->elem->lex.lexemeStr)==0){
                    (*isOutAssigned)[k]=1;
                }
            }
            checkIdentifierScope(temp1,G,sTable,index);
            /*Move to arithExpression*/
            while(!(temp1->elem->isLeaf && strcmp(G.terminals[temp1->elem->curr],"TK_ASSIGNOP")==0)){
                temp1 = temp1->next;
            }
            temp1 = temp1->next;
            checkArithExprScope(temp1,sTable,index,G);

        }
        if(strcmp(G.nonTerminals[temp->elem->curr],"conditionalStmt")==0){
            checkConditionalInIterative(temp,sTable,index,G,isOutAssigned,boolExpVariables,numberOfBoolExpVariables,isChanged);
        }
        if(strcmp(G.nonTerminals[temp->elem->curr],"funCallStmt")==0){
            /* Take output parameters */
            astNode *temp1 = findChild(temp,0,false,0);
            while(!(temp1->elem->isLeaf && strcmp(G.terminals[temp1->elem->curr],"eps")==0)){
                for(int k = 0;k<*numberOfBoolExpVariables;k++){
                    if(strcmp(temp1->elem->lex.lexemeStr,(*boolExpVariables)[k])==0){
                        (*isChanged)[k] = true;
                    }
                }
                /*Check if output is assigned*/
                int outId = sTable->tables[index]->function->outId;
                int outNum = sTable->tables[index]->function->numOut;
                for(int k=0;k<outNum;k++){
                    int outFieldNumber = sTable->tables[index]->function->outOrder[k];
                    if(strcmp(sTable->allTypes[outId]->fields[outFieldNumber]->varName,temp1->elem->lex.lexemeStr)==0){
                        (*isOutAssigned)[k]=1;
                    }
                }
                temp1 = temp1->next;
            }
            /*IdList of output Parameters*/
            temp1 = temp1->child;
            checkIdListScope(temp1,sTable,index,G);
            /*IdList of input parameters*/
            temp1 = findChild(temp,2,false,0);
            temp1 = temp1->next;
            checkIdListScope(temp1,sTable,index,G);
        }
        if(strcmp(G.nonTerminals[temp->elem->curr],"iterativeStmt")==0){
            checkNestedIterative(temp,sTable,index,G,isOutAssigned,boolExpVariables,numberOfBoolExpVariables,isChanged);
        }
        temp = temp->next;
    }
    /*Goto Else Part*/
    temp = temp->next;
    /*If there are no stmts present*/
    if(!(temp->elem->isLeaf)){
        /*goto first statement*/
        temp = findChild(temp,0,false,0);
        while(!(temp->elem->isLeaf && strcmp(G.terminals[temp->elem->curr],"eps")==0)){
        if(strcmp(G.nonTerminals[temp->elem->curr],"ioStmt")==0){
            if(strcmp(G.terminals[temp->child->elem->curr],"TK_WRITE")==0){
                /* get var name */
                astNode *temp1 = findChild(temp,1,false,0);
                char **name = (char **)malloc(sizeof(char *));
                getVarName(name,temp1,G,sTable,index);
                for(int k = 0;k<(*numberOfBoolExpVariables);k++){
                    if(strcmp(*name,(*boolExpVariables)[k])==0){
                        (*isChanged)[k]=true;
                    }
                }
                int outId = sTable->tables[index]->function->outId;
                int outNum = sTable->tables[index]->function->numOut;
                for(int k=0;k<outNum;k++){
                    int outFieldNumber = sTable->tables[index]->function->outOrder[k];
                    if(strcmp(sTable->allTypes[outId]->fields[outFieldNumber]->varName,temp1->elem->lex.lexemeStr)==0){
                        (*isOutAssigned)[k]=1;
                    }
                }
            }
            /*Check identifier scope*/
            astNode *temp1 = findChild(temp,1,false,0);
            checkIdentifierScope(temp1,G,sTable,index);
        }
        if(strcmp(G.nonTerminals[temp->elem->curr],"assignmentStmt")==0){
            /*Node corresponding to singleOrRecId*/
            astNode *temp1 = findChild(temp,0,false,0);
            char **name = (char **)malloc(sizeof(char *));
            getVarName(name,temp1,G,sTable,index);
            for(int k = 0;k<(*numberOfBoolExpVariables);k++){
                if(strcmp(*name,(*boolExpVariables)[k])==0){
                    (*isChanged)[k]=true;
                }
            }
            int outId = sTable->tables[index]->function->outId;
            int outNum = sTable->tables[index]->function->numOut;
            for(int k=0;k<outNum;k++){
                int outFieldNumber = sTable->tables[index]->function->outOrder[k];
                if(strcmp(sTable->allTypes[outId]->fields[outFieldNumber]->varName,temp1->elem->lex.lexemeStr)==0){
                    (*isOutAssigned)[k]=1;
                }
            }
            checkIdentifierScope(temp1,G,sTable,index);
            /*Move to arithExpression*/
            while(!(temp1->elem->isLeaf && strcmp(G.terminals[temp1->elem->curr],"TK_ASSIGNOP")==0)){
                temp1 = temp1->next;
            }
            temp1 = temp1->next;
            checkArithExprScope(temp1,sTable,index,G);

        }
        if(strcmp(G.nonTerminals[temp->elem->curr],"conditionalStmt")==0){
            checkConditionalInIterative(temp,sTable,index,G,isOutAssigned,boolExpVariables,numberOfBoolExpVariables,isChanged);
        }
        if(strcmp(G.nonTerminals[temp->elem->curr],"funCallStmt")==0){
            /* Take output parameters */
            astNode *temp1 = findChild(temp,0,false,0);
            while(!(temp1->elem->isLeaf && strcmp(G.terminals[temp1->elem->curr],"eps")==0)){
                for(int k = 0;k<*numberOfBoolExpVariables;k++){
                    if(strcmp(temp1->elem->lex.lexemeStr,(*boolExpVariables)[k])==0){
                        (*isChanged)[k] = true;
                    }
                }
                int outId = sTable->tables[index]->function->outId;
                int outNum = sTable->tables[index]->function->numOut;
                for(int k=0;k<outNum;k++){
                    int outFieldNumber = sTable->tables[index]->function->outOrder[k];
                    if(strcmp(sTable->allTypes[outId]->fields[outFieldNumber]->varName,temp1->elem->lex.lexemeStr)==0){
                        (*isOutAssigned)[k]=1;
                    }
                }
                temp1 = temp1->next;
            }
            /*IdList of output Parameters*/
            temp1 = temp1->child;
            checkIdListScope(temp1,sTable,index,G);
            /*IdList of input parameters*/
            temp1 = findChild(temp,2,false,0);
            temp1 = temp1->next;
            checkIdListScope(temp1,sTable,index,G);
        }
        if(strcmp(G.nonTerminals[temp->elem->curr],"iterativeStmt")==0){
            checkNestedIterative(temp,sTable,index,G,isOutAssigned,boolExpVariables,numberOfBoolExpVariables,isChanged);
        }
        temp = temp->next;
    }
        

    }

}
void checkNestedIterative(astNode *root,symbolTable *sTable,int index,grammar G,int **isOutAssigned,char ***boolExpVariablesComplete,int *numberOfBoolExpVariablesTotal,bool **isChanged){
    /*Go to boolExpression of current loop*/
    astNode *temp = findChild(root,0,false,0);
    int *numberOfBoolExpVariablesCurrent = (int *)malloc(sizeof(int));
    *numberOfBoolExpVariablesCurrent = 0;
    char ***boolExpVariablesCurrent = (char ***)malloc(sizeof(char **));
    loadBooleanExpressionVariables(temp,G,boolExpVariablesCurrent,numberOfBoolExpVariablesCurrent);
    /* Update boolean expressions list*/
    int start = *numberOfBoolExpVariablesTotal;
    int end = (*numberOfBoolExpVariablesTotal)+(*numberOfBoolExpVariablesCurrent);
    
    *boolExpVariablesComplete = (char **)realloc(*boolExpVariablesComplete,sizeof(char **)*((*numberOfBoolExpVariablesTotal)+(*numberOfBoolExpVariablesCurrent)));
    *isChanged = (bool *)realloc(*isChanged,sizeof(bool *)*((*numberOfBoolExpVariablesTotal)+(*numberOfBoolExpVariablesCurrent)));
    for(int i = start;i < end;i++){
        (*boolExpVariablesComplete)[i]=(*boolExpVariablesCurrent)[i-start];
    }
    /*node corresponding to first stmt*/
    temp = findChild(root,1,false,0);
    if(strcmp(G.nonTerminals[temp->elem->curr],"ioStmt")==0){
        if(strcmp(G.nonTerminals[temp->child->elem->curr],"TK_WRITE")==0){
            /* get var name */
            astNode *temp1 = findChild(temp,1,false,0);
            char **name = (char **)malloc(sizeof(char *));
            getVarName(name,temp1,G,sTable,index);
            for(int k = 0;k<end;k++){
                if(strcmp(*name,(*boolExpVariablesComplete)[k])==0){
                    (*isChanged)[k]=true;
                }
            }
            int outId = sTable->tables[index]->function->outId;
            int outNum = sTable->tables[index]->function->numOut;
            for(int k=0;k<outNum;k++){
                int outFieldNumber = sTable->tables[index]->function->outOrder[k];
                if(strcmp(sTable->allTypes[outId]->fields[outFieldNumber]->varName,temp1->elem->lex.lexemeStr)==0){
                    (*isOutAssigned)[k]=1;
                }
            }
        }
    }
    if(strcmp(G.nonTerminals[temp->elem->curr],"assignmentStmt")==0){
        astNode *temp1 = findChild(temp,0,false,0);
        char **name = (char **)malloc(sizeof(char *));
        getVarName(name,temp1,G,sTable,index);
        for(int k = 0;k<end;k++){
            if(strcmp(*name,(*boolExpVariablesComplete)[k])==0){
                (*isChanged)[k]=true;
            }
        }
        int outId = sTable->tables[index]->function->outId;
        int outNum = sTable->tables[index]->function->numOut;
        for(int k=0;k<outNum;k++){
            int outFieldNumber = sTable->tables[index]->function->outOrder[k];
            if(strcmp(sTable->allTypes[outId]->fields[outFieldNumber]->varName,temp1->elem->lex.lexemeStr)==0){
                (*isOutAssigned)[k]=1;
            }
        }
    }
    if(strcmp(G.nonTerminals[temp->elem->curr],"conditionalStmt")==0){
        checkConditionalInIterative(temp,sTable,index,G,isOutAssigned,boolExpVariablesCurrent,numberOfBoolExpVariablesCurrent,isChanged);
    }
    if(strcmp(G.nonTerminals[temp->elem->curr],"funCallStmt")==0){
        /* Take output parameters */
        astNode *temp1 = findChild(temp,0,false,0);
        while(!(temp1->elem->isLeaf && strcmp(G.terminals[temp1->elem->curr],"eps")==0)){
            for(int k = 0;k<end;k++){
                if(strcmp(temp1->elem->lex.lexemeStr,(*boolExpVariablesComplete)[k])==0){
                    (*isChanged)[k] = true;
                }
            }
            int outId = sTable->tables[index]->function->outId;
            int outNum = sTable->tables[index]->function->numOut;
            for(int k=0;k<outNum;k++){
                int outFieldNumber = sTable->tables[index]->function->outOrder[k];
                if(strcmp(sTable->allTypes[outId]->fields[outFieldNumber]->varName,temp1->elem->lex.lexemeStr)==0){
                    (*isOutAssigned)[k]=1;
                }
            }
            temp1 = temp1->next;
        }
    }
    if(strcmp(G.nonTerminals[temp->elem->curr],"iterativeStmt")==0){
        checkNestedIterative(temp,sTable,index,G,isOutAssigned,boolExpVariablesCurrent,numberOfBoolExpVariablesCurrent,isChanged);
    }
    bool isLoopChange = false;
    for(int i = start; i<end;i++){
        isLoopChange |= (*isChanged)[i];
    }
    if(!isLoopChange){
        /*Line number corresponds to first Leaf of boolean expression*/
        printf("ERROR: while loop at line %d will not terminate.\n",root->child->child->elem->lineNo);
    }
}
/*Checks if str is an output of sTable->tables[index]->function*/
bool isOutputParams(char *str, symbolTable *sTable, int index, grammar G){
    bool isOutput = false;
    for(int i = 0;i<sTable->tables[index]->function->numOut;i++){
        int outId = sTable->tables[index]->function->outId;
        int outFieldNumber = sTable->tables[index]->function->outOrder[i];
        if(strcmp(str,sTable->allTypes[outId]->fields[outFieldNumber]->varName)==0)
            return true;
    }
    return false;
}
/*
    will be called when root corresponds to <declaration>
    index is the symbolTable number of corresponding function
*/
void checkDeclarationsSemantics(astNode *root, symbolTable *sTable, int index, grammar G){
    astNode *temp = findChild(root,1,false,0);  // finds child corresponding to TK_ID
    astNode *temp2 = findChild(root,0,false,0); // datatype
    if(temp2->elem->isLeaf && (strcmp(G.terminals[temp2->elem->curr],"TK_RECORD")==0 || strcmp(G.terminals[temp2->elem->curr],"TK_UNION")==0 || strcmp(G.terminals[temp2->elem->curr],"TK_RUID")==0)){
        if(strcmp(G.terminals[temp2->elem->curr],"TK_RUID")!=0)temp2 = temp2->next;
        int i = searchTypes(temp2->elem->lex.lexemeStr,sTable);
        if(i==-1){
            printf("ERROR: The type of identifier %s at line %d is not defined\n",temp2->next->elem->lex.lexemeStr,temp2->next->elem->lineNo);
        }
        return;
    }
    astNode *temp1 = findChild(root,2,false,0); // child corresponding to global    
    bool isGlobal = (temp1->elem->isLeaf && strcmp(G.terminals[temp1->elem->curr],"eps")==0)? false: true;
    // if(isGlobal)
    // printf(" Is global\n");
    if(!isGlobal){
        int i = -1;
        /*check if declaration is in corresponding symbolTable*/
        if(sTable->tables[index]->numEntries!=0)
        i = searchS(temp->elem->lex.lexemeStr,sTable->tables[index]->entries);
        int j = -1;
        if(sTable->numEntries!=0)
        j = searchS(temp->elem->lex.lexemeStr,sTable->entries); /* Check if identifier is global*/
        /* Checks if variable is declared multiple times*/
        if(i==-1 && j==-1){
            printf("ERROR: Identifier %s at line number %d is not present in symbolTable\n",temp->elem->lex.lexemeStr,temp->elem->lineNo);
        }
        else if(i!=-1 && sTable->tables[index]->entries[i]->count > 1){
            printf("ERROR: Identifier %s at line %d has been declared previously at line %d.\n",sTable->tables[index]->entries[i]->varName,sTable->tables[index]->entries[i]->lineNo,sTable->tables[index]->entries[i]->lineNo);
            (sTable->tables[index]->entries[i]->count)--;
        }
        else if(i!=-1 && sTable->tables[index]->entries[i]->lineNo != temp->elem->lineNo){
            printf("ERROR: Identifier %s at line %d has been declared previously at line %d.\n",temp->elem->lex.lexemeStr,temp->elem->lineNo,sTable->tables[index]->entries[i]->lineNo);    
        }
        if (j!=-1){
            printf("ERROR: Global identifier %s at line %d is redeclared at line number %d.\n",sTable->entries[j]->varName,sTable->entries[j]->lineNo,temp->elem->lineNo);
        }
    }
    else{
        /* Search for the index in global entries */
        if(sTable->numEntries!=0){
            int i = searchS(temp->elem->lex.lexemeStr,sTable->entries);
            // printf("Global %s\n",temp->elem->lex.lexemeStr);
            /* Search for the index in the corresponding symbol table of functions */
            int j = -1;
            if(sTable->tables[index]->numEntries!=0)
            j=searchS(temp->elem->lex.lexemeStr,sTable->tables[index]->entries);
            if(i == -1){
                printf("ERROR: Global Identifier %s at line number %d is not present in symbolTable\n",temp->elem->lex.lexemeStr,temp->elem->lineNo);            
            }
            else if(sTable->entries[i]->count > 1){ /* Global global redeclaration at same line*/
                printf("ERROR: Global Identifier %s at line %d has been declared previously at line %d\n",sTable->entries[i]->varName,temp->elem->lineNo,sTable->entries[i]->lineNo);
                (sTable->tables[index]->entries[i]->count)--;
            }
            else if(sTable->entries[i]->lineNo != temp->elem->lineNo){  /* Global global redeclaration at different lines*/
                printf("ERROR: Global Identifier %s at line %d has been declared previously at line %d\n",temp->elem->lex.lexemeStr,temp->elem->lineNo,sTable->entries[i]->lineNo);    
            }
        }
    }
    /* Checks if global variable has been declared previously anywhere in the program */
}


/* 
check semantics associated with functionCall statement
the root here corresponds to funCallStmt
*/
void checkFunctionCall(astNode *root, symbolTable *sTable, int index, grammar G, int **isOutAssigned){
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
        /* Check if number and type of input paramters are the same */

        /* inputParams */
        astNode *temp2 = findChild(root,2,false,0);
        int inpNumber = 0;
        temp2 = temp2->child;
        while(!(temp2->elem->isLeaf && strcmp(G.terminals[temp2->elem->curr],"eps")==0)){
            inpNumber++;
            temp2 = temp2->next;
        }
        if(inpNumber!=sTable->tables[i]->function->numIn){
            printf("ERROR: The number of input parameters for function %s in function Call statement at line No %d does not match with the function definition at line %d.\n",temp->elem->lex.lexemeStr,temp->elem->lineNo,sTable->tables[i]->function->line);
        }
        else{
            bool isMatch = true;
            int j = 0;
            temp2 = findChild(root,2,false,0);
            temp2 = temp2->child;
            while(j<sTable->tables[i]->function->numIn){
                isMatch = true;
                int typeIndex = findIDtype(temp2,G,sTable,index);
                int paramPosition = sTable->tables[i]->function->inOrder[j];
                int paramType = sTable->allTypes[sTable->tables[i]->function->inId]->fields[paramPosition]->type;
                if(sTable->allTypes[typeIndex]->typeId == 4){
                    /*Checks if input parameter is of type union*/
                    printf("ERROR: The parameter %s at function call statement in line %d is of type union.\n",sTable->allTypes[typeIndex]->name,temp->elem->lineNo);
                }
                if(paramType!=typeIndex){
                    isMatch = false;
                }
                // if(sTable->allTypes[typeIndex]->typeId == 3){
                //     /* Check if record is of variant record type */
                //     bool isVariant = false;
                //     bool isTagPresent = false;
                //     for(int k=0;k<sTable->allTypes[typeIndex]->numFields;k++){
                //         /* find type of the field */
                //         int typeField = sTable->allTypes[sTable->allTypes[typeIndex]->fields[k]->type]->typeId;
                //         if(typeField == 3){
                //             isVariant = true;
                //         }
                //         /* If an attribute tagvalue is present of type real or int*/
                //         if(strcmp(sTable->allTypes[typeIndex]->fields[k]->varName,"tagvalue")==0 && (typeField == 0)){
                //             isTagPresent = true;
                //         }
                //         if(isVariant && !isTagPresent){
                //             printf("ERROR: Variant record %s passed as parameter in line %d does not have valid tagvalue attribute.\n",sTable->allTypes[typeIndex]->fields[k]->varName,temp->elem->lineNo);
                //         }
                //     }
                // }
                if(!isMatch){
                    int id = sTable->tables[i]->function->inId;
                    int inpFieldNumber = sTable->tables[i]->function->inOrder[j];
                    printf("ERROR: The type of input parameter %s at line %d does not match with type of %s in function definition at line %d.\n",temp2->elem->lex.lexemeStr,temp2->elem->lineNo,sTable->allTypes[id]->fields[inpFieldNumber]->varName,sTable->tables[i]->function->line);
                }
                j++;
                temp2=temp2->next;
            }

        }
        /*Check if output parameters and their types match*/
        astNode *temp1 = findChild(root,0,false,0);
        int outNumber = 0;
        temp1 = temp1->child;
        while(!(temp1->elem->isLeaf && strcmp(G.terminals[temp1->elem->curr],"eps")==0)){
            outNumber++;
            temp1 = temp1->next;
        }
        if(outNumber!=sTable->tables[i]->function->numOut){
            printf("ERROR: The number of output parameters for function %s in function Call statement at line No %d does not match with the function definition at line %d.\n",temp->elem->lex.lexemeStr,temp->elem->lineNo,sTable->tables[i]->function->line);
        }
        else{
            bool isMatch = true;
            int j = 0;
            temp1 = findChild(root,0,false,0);
            temp1 = temp1->child;
            int outId = sTable->tables[i]->function->outId;
            while(j<sTable->tables[i]->function->numOut){
                isMatch = true;
                int outFieldNumber = sTable->tables[i]->function->outOrder[j];
                int outFieldType = sTable->allTypes[outId]->fields[outFieldNumber]->type;
                if(outFieldType!=findIDtype(temp1,G,sTable,index)){
                    isMatch = false;
                }
                /*Check if the outparameter will be returned by the function*/
                if(isOutputParams(temp1->elem->lex.lexemeStr,sTable,index,G)){
                    (*isOutAssigned)[j] = 1;
                }
                j++;
                temp1=temp1->next;
                if(!isMatch){
                    int id = sTable->tables[i]->function->outId;
                    int outFieldNumber = sTable->tables[i]->function->outOrder[j];
                    printf("ERROR: The type of output parameter %s at line %d does not match with type of %s in function definition at line %d.\n",temp1->elem->lex.lexemeStr,temp1->elem->lineNo,sTable->allTypes[id]->fields[outFieldNumber]->varName,sTable->tables[i]->function->line);
                }
            }
        }
    }
}
void checkFunction(astNode *root, symbolTable *sTable, grammar G, int index){
    checkOverloading(root,sTable,G,index);
    // bool isMain = index == sTable->numF? true: false;
    int **isOutAssigned = (int **)malloc(sizeof(int *));
    // printf("%d outputParams %d\n",index,sTable->tables[index]->function->numOut);
    *isOutAssigned = (int *)calloc(sTable->tables[index]->function->numOut,sizeof(int));
    astNode *temp=NULL;
    /*Reach first child which is stmts when root is <function>*/
    if(index!=sTable->numF-1)
    temp = findChild(root,3,false,0); 
    /*Reach first child which is stmts when root is mainProgram*/
    else
    temp = findChild(root,0,false,0);
    /* Traverse till return statement is reached */
    while(!(!temp->elem->isLeaf && strcmp(G.nonTerminals[temp->elem->curr],"returnStmt")==0)){
        if(!temp->elem->isLeaf && strcmp(G.nonTerminals[temp->elem->curr],"declaration")==0){
            // printf("In %s of function\n",G.nonTerminals[temp->elem->curr]);
            checkDeclarationsSemantics(temp,sTable,index,G);
        }
        if(!temp->elem->isLeaf && strcmp(G.nonTerminals[temp->elem->curr],"funCallStmt")==0){
            checkFunctionCall(temp,sTable,index,G,isOutAssigned);
        }
        if(!temp->elem->isLeaf && strcmp(G.nonTerminals[temp->elem->curr],"iterativeStmt")==0){
            // printf("Entered iterative\n");
            checkIterativeStmt(temp,sTable,index,G,isOutAssigned);
        }   
        if(!temp->elem->isLeaf && strcmp(G.nonTerminals[temp->elem->curr],"assignmentStmt")==0){
            /*Node corresponding to singleOrRecId*/
            astNode *temp1 = findChild(temp,0,false,0);
            char **name = (char **)malloc(sizeof(char *));
            getVarName(name,temp1,G,sTable,index);
            for(int k = 0;k<sTable->tables[index]->function->numOut;k++){
                int j = sTable->tables[index]->function->outOrder[k];
                int outId = sTable->tables[index]->function->outId;
                if(strcmp(sTable->allTypes[outId]->fields[j]->varName,temp1->elem->lex.lexemeStr)==0){
                    (*isOutAssigned)[k]=1;
                }
            }
            checkIdentifierScope(temp1,G,sTable,index);
            /*Move to arithExpression*/
            while(!(temp1->elem->isLeaf && strcmp(G.terminals[temp1->elem->curr],"TK_ASSIGNOP")==0)){
                temp1 = temp1->next;
            }
            temp1 = temp1->next;
            // printf("Non terminal before arithExprScope %s\n",G.nonTerminals[temp1->elem->curr]);
            checkArithExprScope(temp1,sTable,index,G);
        }
        if(!temp->elem->isLeaf && strcmp(G.nonTerminals[temp->elem->curr],"ioStmt")==0){
            if(strcmp(G.terminals[temp->child->elem->curr],"TK_WRITE")==0){
                /* get var name */
                astNode *temp1 = findChild(temp,1,false,0);
                char **name = (char **)malloc(sizeof(char *));
                getVarName(name,temp1,G,sTable,index);
                for(int k = 0;k<sTable->tables[index]->function->numOut;k++){
                    int j = sTable->tables[index]->function->outOrder[k];
                    int outId = sTable->tables[index]->function->outId;
                    if(strcmp(sTable->allTypes[outId]->fields[j]->varName,temp1->elem->lex.lexemeStr)==0){
                        (*isOutAssigned)[k]=1;
                    }
                }
            }
            /*Check identifier scope*/
            astNode *temp1 = findChild(temp,1,false,0);
            checkIdentifierScope(temp1,G,sTable,index);
        }               
        if(!temp->elem->isLeaf && strcmp(G.nonTerminals[temp->elem->curr],"conditionalStmt")==0){
            // printf("In %s of function\n",G.nonTerminals[temp->elem->curr]);
            checkConditionalStmt(temp,sTable,index,G,isOutAssigned);
        }
        temp = temp->next;
    }
    
    for(int i=0;i<sTable->tables[index]->function->numOut;i++){
        if((*isOutAssigned)[i]==0){
            int fieldNumber = sTable->tables[index]->function->outOrder[i];
            int outId = sTable->tables[index]->function->outId;
            // printf("Field number %d\n",fieldNumber);
            printf("ERROR: Output parameter %s for function %s at line %d has not been assigned a value.\n",sTable->allTypes[outId]->fields[fieldNumber]->varName,sTable->tables[index]->function->fId,root->child->elem->lineNo);
        }
    }
}
void checkConditionalStmt(astNode *root, symbolTable *sTable,int index,grammar G, int **isOutAssigned){
    astNode *temp = findChild(root,1,false,0);
    // printf("terminal printing %s\n",G.terminals[temp->next->next->elem->curr]);
    checkBoolExprScope(temp,sTable,index,G);
    /*temp corresponds to first stmt in IF part*/
    temp = findChild(root,3,false,0);
    while(!(temp->elem->isLeaf && strcmp(G.terminals[temp->elem->curr],"eps")==0)){
        if(!temp->elem->isLeaf && strcmp(G.nonTerminals[temp->elem->curr],"funCallStmt")==0){
            checkFunctionCall(temp,sTable,index,G,isOutAssigned);
        }
        if(!temp->elem->isLeaf && strcmp(G.nonTerminals[temp->elem->curr],"iterativeStmt")==0){
            checkIterativeStmt(temp,sTable,index,G,isOutAssigned);
        }
        if(!temp->elem->isLeaf && strcmp(G.nonTerminals[temp->elem->curr],"ioStmt")==0){
            if(strcmp(G.terminals[temp->child->elem->curr],"TK_WRITE")==0){
                /* get var name */
                astNode *temp1 = findChild(temp,1,false,0);
                char **name = (char **)malloc(sizeof(char *));
                getVarName(name,temp1,G,sTable,index);
                for(int k = 0;k<sTable->tables[index]->function->numOut;k++){
                    int j = sTable->tables[index]->function->outOrder[k];
                    int outId = sTable->tables[index]->function->outId;
                    if(strcmp(sTable->allTypes[outId]->fields[j]->varName,temp1->elem->lex.lexemeStr)==0){
                        (*isOutAssigned)[k]=1;
                    }
                }
            }
            /*Check identifier scope*/
            astNode *temp1 = findChild(temp,1,false,0);
            checkIdentifierScope(temp1,G,sTable,index);
        }
        if(!temp->elem->isLeaf && strcmp(G.nonTerminals[temp->elem->curr],"assignmentStmt")==0){
            /*Node corresponding to singleOrRecId*/
            astNode *temp1 = findChild(temp,0,false,0);
            char **name = (char **)malloc(sizeof(char *));
            getVarName(name,temp1,G,sTable,index);
            for(int k = 0;k<sTable->tables[index]->function->numOut;k++){
                int j = sTable->tables[index]->function->outOrder[k];
                int outId = sTable->tables[index]->function->outId;
                if(strcmp(sTable->allTypes[outId]->fields[j]->varName,temp1->elem->lex.lexemeStr)==0){
                    (*isOutAssigned)[k]=1;
                }
            }
            checkIdentifierScope(temp1,G,sTable,index);
            /*Move to arithExpression*/
            while(!(temp1->elem->isLeaf && strcmp(G.terminals[temp1->elem->curr],"TK_ASSIGNOP")==0)){
                temp1 = temp1->next;
            }
            temp1 = temp1->next;
            checkArithExprScope(temp1,sTable,index,G);  
        }
        if(!temp->elem->isLeaf && strcmp(G.nonTerminals[temp->elem->curr],"conditionalStmt")==0){
            checkConditionalStmt(temp,sTable,index,G,isOutAssigned);
        }
        temp = temp->next;
    }    
    /*temp is shifted to elsePart*/
    temp = temp->next;
    if(!(temp->elem->isLeaf && strcmp(G.terminals[temp->elem->curr],"TK_ENDIF")==0)){
        temp = findChild(temp,0,false,0);
        while(!(temp->elem->isLeaf && strcmp(G.terminals[temp->elem->curr],"eps")==0)){
            if(!temp->elem->isLeaf && strcmp(G.nonTerminals[temp->elem->curr],"funCallStmt")==0){
                checkFunctionCall(temp,sTable,index,G,isOutAssigned);
            }
            if(!temp->elem->isLeaf && strcmp(G.nonTerminals[temp->elem->curr],"iterativeStmt")==0){
                checkIterativeStmt(temp,sTable,index,G,isOutAssigned);
            }
            if(!temp->elem->isLeaf && strcmp(G.nonTerminals[temp->elem->curr],"ioStmt")==0){
                if(strcmp(G.terminals[temp->child->elem->curr],"TK_WRITE")==0){
                    /* get var name */
                    astNode *temp1 = findChild(temp,1,false,0);
                    char **name = (char **)malloc(sizeof(char *));
                    getVarName(name,temp1,G,sTable,index);
                    for(int k = 0;k<sTable->tables[index]->function->numOut;k++){
                        int j = sTable->tables[index]->function->outOrder[k];
                        int outId = sTable->tables[index]->function->outId;
                        if(strcmp(sTable->allTypes[outId]->fields[j]->varName,temp1->elem->lex.lexemeStr)==0){
                            (*isOutAssigned)[k]=1;
                        }
                    }
                }
                /*Check identifier scope*/
                astNode *temp1 = findChild(temp,1,false,0);
                checkIdentifierScope(temp1,G,sTable,index);
            }
            if(!temp->elem->isLeaf && strcmp(G.nonTerminals[temp->elem->curr],"assignmentStmt")==0){
                /*Node corresponding to singleOrRecId*/
                astNode *temp1 = findChild(temp,0,false,0);
                char **name = (char **)malloc(sizeof(char *));
                getVarName(name,temp1,G,sTable,index);
                for(int k = 0;k<sTable->tables[index]->function->numOut;k++){
                    int j = sTable->tables[index]->function->outOrder[k];
                    int outId = sTable->tables[index]->function->outId;
                    if(strcmp(sTable->allTypes[outId]->fields[j]->varName,temp1->elem->lex.lexemeStr)==0){
                        (*isOutAssigned)[k]=1;
                    }
                }
                checkIdentifierScope(temp1,G,sTable,index);
                /*Move to arithExpression*/
                while(!(temp1->elem->isLeaf && strcmp(G.terminals[temp1->elem->curr],"TK_ASSIGNOP")==0)){
                    temp1 = temp1->next;
                }
                temp1 = temp1->next;
                checkArithExprScope(temp1,sTable,index,G);  
            }
            if(!temp->elem->isLeaf && strcmp(G.nonTerminals[temp->elem->curr],"conditionalStmt")==0){
                checkConditionalStmt(temp,sTable,index,G,isOutAssigned);
            }
            temp = temp->next;
        }
    }  
}
void checkIterativeStmt(astNode *root, symbolTable *sTable,int index,grammar G, int **isOutAssigned){
    astNode *temp = findChild(root,0,false,0);
    char ***boolExpVariables=(char ***)malloc(sizeof(char **));
    // printf("boolExp initialized\n");
    int *numberOfBoolExpVariables=(int *)malloc(sizeof(int));
    *numberOfBoolExpVariables = 0;
    bool relopEncountered = false;
    // printf("temp iter %s\n",G.nonTerminals[temp->elem->curr]);
    loadBooleanExpressionVariables(temp,G,boolExpVariables,numberOfBoolExpVariables);
    // printf("boolExp loaded\n");
    int start = 0;
    int end = *numberOfBoolExpVariables;
    bool **isChanged = (bool **)malloc(sizeof(bool *));
    *isChanged = (bool *)calloc(end,sizeof(bool));
    // printf("variables initialized\n");
    /*node corresponding to first stmt*/
    temp = findChild(root,1,false,0);
    while(!(temp->elem->isLeaf && strcmp(G.terminals[temp->elem->curr],"eps")==0)){
        if(strcmp(G.nonTerminals[temp->elem->curr],"ioStmt")==0){
            if(strcmp(G.nonTerminals[temp->child->elem->curr],"TK_WRITE")==0){
                /* get var name */
                astNode *temp1 = findChild(temp,1,false,0);
                char **name = (char **)malloc(sizeof(char *));
                getVarName(name,temp1,G,sTable,index);
                for(int k = 0;k<(*numberOfBoolExpVariables);k++){
                    if(strcmp(*name,(*boolExpVariables)[k])==0){
                        (*isChanged)[k]=true;
                    }
                }
                int outId = sTable->tables[index]->function->outId;
                int outNum = sTable->tables[index]->function->numOut;
                for(int k=0;k<outNum;k++){
                    int outFieldNumber = sTable->tables[index]->function->outOrder[k];
                    if(strcmp(sTable->allTypes[outId]->fields[outFieldNumber]->varName,temp1->elem->lex.lexemeStr)==0){
                        (*isOutAssigned)[k]=1;
                    }
                }
            }
        }
        if(strcmp(G.nonTerminals[temp->elem->curr],"assignmentStmt")==0){
            astNode *temp1 = findChild(temp,0,false,0);
            // printf("output %s\n",temp1->elem->lex.lexemeStr);
            char **name = (char **)malloc(sizeof(char *));
            getVarName(name,temp1,G,sTable,index);
            // printf("before assignment checking loop %s\n",(*name));
            for(int k = 0;k<(*numberOfBoolExpVariables);k++){
                // printf("checking assignment in iterative %s boolExpVar %s\n",*name,(*boolExpVariables)[k]);
                if(strcmp(*name,(*boolExpVariables)[k])==0){
                    (*isChanged)[k]=true;
                }
            }
            int outId = sTable->tables[index]->function->outId;
            int outNum = sTable->tables[index]->function->numOut;
            for(int k=0;k<outNum;k++){
                int outFieldNumber = sTable->tables[index]->function->outOrder[k];
                if(strcmp(sTable->allTypes[outId]->fields[outFieldNumber]->varName,temp1->elem->lex.lexemeStr)==0){
                    (*isOutAssigned)[k]=1;
                }
            }
        }
        if(strcmp(G.nonTerminals[temp->elem->curr],"conditionalStmt")==0){
            // printf("Entered conditional in iterative\n");
            checkConditionalInIterative(temp,sTable,index,G,isOutAssigned,boolExpVariables,numberOfBoolExpVariables,isChanged);
        }
        if(strcmp(G.nonTerminals[temp->elem->curr],"funCallStmt")==0){
            /* Take output parameters */
            astNode *temp1 = findChild(temp,0,false,0);
            // printf("Non terminal name %s.\n",G.nonTerminals[temp1->elem->curr]);
            /*Goto idList*/
            temp1 = temp1->child;
            while(!(temp1->elem->isLeaf && strcmp(G.terminals[temp1->elem->curr],"eps")==0)){
                for(int k = 0;k<*numberOfBoolExpVariables;k++){
                    if(strcmp(temp1->elem->lex.lexemeStr,(*boolExpVariables)[k])==0){
                        (*isChanged)[k] = true;
                    }
                }
                int outId = sTable->tables[index]->function->outId;
                int outNum = sTable->tables[index]->function->numOut;
                for(int k=0;k<outNum;k++){
                    int outFieldNumber = sTable->tables[index]->function->outOrder[k];
                    if(strcmp(sTable->allTypes[outId]->fields[outFieldNumber]->varName,temp1->elem->lex.lexemeStr)==0){
                        (*isOutAssigned)[k]=1;
                    }
                }
                temp1 = temp1->next;
            }
        }
        if(strcmp(G.nonTerminals[temp->elem->curr],"iterativeStmt")==0){
            checkNestedIterative(temp,sTable,index,G,isOutAssigned,boolExpVariables,numberOfBoolExpVariables,isChanged);
        }
        temp = temp->next;
    }
    bool isLoopChange = false;
    for(int i = start; i<end;i++){
        isLoopChange = isLoopChange | (*isChanged)[i];
    }
    if(!isLoopChange){
        /*Line number corresponds to first Leaf of boolean expression*/
        printf("ERROR: while loop at line %d will not terminate.\n",root->child->child->elem->lineNo);
    }
    
}
void checkOverloading(astNode *root, symbolTable *sTable, grammar G, int index){
    astNode *temp = findChild(root,0,false,0);
    for(int i = 0; i<index; i++){
        if(strcmp(temp->elem->lex.lexemeStr,sTable->tables[index]->function->fId)==0){
            printf("ERROR: Function at line %d has been previously declared at line %d.\n",sTable->tables[index]->function->line,sTable->tables[i]->function->line);
            break;
        }
    }
}
/*Here root correspomds to <program>*/
void semanticCheck(astNode *root, symbolTable *sTable, grammar G){
    /*corresponds to first function*/
    astNode *temp = findChild(root,0,false,0);
    while(temp!=NULL){
        if(!(temp->elem->isLeaf && strcmp(G.terminals[temp->elem->curr],"eps")==0)){
            astNode *temp1 = findChild(temp,0,false,0);
            // printf("Function name %s if\n",G.nonTerminals[temp->elem->curr]);
            int index=-1;
            if(strcmp(G.nonTerminals[temp->elem->curr],"mainFunction")==0)index=sTable->numF-1;
            else index = fIndex(temp1->elem->lex.lexemeStr,sTable);
            // printf("Index in symbolTable %d\n",index);
            checkFunction(temp,sTable,G,index);
        }
        else{
            // printf("Function name %s else\n",G.terminals[root->elem->curr]);
        }
        temp = temp->next;
    }
}
void main() { 
    
    char* file; 
    file = "grammar.txt"; 
    grammar C; 
    C = readGrammar(file); 
    printf("Grammar read and saved \n"); 
    printf("%d %d %d %d \n", C.totalNumRules, C.allRules[36].numOrs, C.allRules[31].epsilon, C.allRules[28].epsilon); 

    
    C.ff = ComputeFirstAndFollowSets(C); 
    printf("First and Follow computed \n"); 
    printf("\n***** \n"); 

    
    printf("\n***** \n"); 

    
    parseTable* T = intializeParseTable(C.numNonTerminals,C.numTerminals);
    createParseTable(C,C.ff,T);
    // printParseTable(C,T);
    printf("Parse Table created \n"); 

    char* testCaseFile = "./testcases/c6.txt"; 

    treeN rootNode; 
    rootNode = parseInputSourceCode(testCaseFile, C, T); 

    int *insertPrev = (int *)malloc(sizeof(int));
    *insertPrev = 0;
    int *count = (int *)malloc(sizeof(int));
    *count = 0;
    astNode *astroot = mknode(rootNode.elem,C);
    constructAst(astroot, &rootNode,C,insertPrev,astroot);
    printf("*************************************************************************************************\n\n");
    printf("Printing Abstract Syntax Tree\n");
    printAST(astroot,C,count);
    printf("Number of nodes in AST %d\n",*count);
    printf("*************************************************************************************************\n\n");

    printf("Constructing symbol table and performing type checking \n"); 
    symbolTable* sTable = constructST(astroot, C); 
    printf("%s\n",C.nonTerminals[astroot->elem->curr]);
    typeCheck(astroot,C,sTable,-1);
    printf("*************************************************************************************************\n\n");
    printf("Semantic Analysis\n");
    semanticCheck(astroot,sTable,C);
} 
