// #pragma once
#include <stdio.h> 
#include <stdbool.h> 
#include "string.h" 
#include "symbolTable.c" 
#include "ast.c" 
// TK_NUM: 0
// TK_RNUM: 1
// booleanExpression: -3
// Error : -1 
// Void : -2 
bool isLogOp(astNode *root, grammar G){
    if(root->elem->isLeaf){
        int curr = root->elem->curr;
        curr--;
        if(curr == TK_AND || curr == TK_OR) return true;
    }
    else return false;
}
bool isRelOp(astNode *root, grammar G){
    if(root->elem->isLeaf){
        int curr = root->elem->curr;
        curr--;
        switch (curr){
        case TK_LT:
        case TK_LE:
        case TK_GE:
        case TK_EQ:
        case TK_NE:
        return true;
        default:
        return false;
        }
    }
}
int findNestedType(astNode *root, grammar G, symbolTable *sTable, int index){   // used for <var>
    while(!(root->next->elem->isLeaf && strcmp(G.terminals[root->next->elem->curr],"eps"))){
        astNode *fieldNode = root->next;
        // find index where record corresponding to root is defined
        int t1 = searchTypes(root->elem->lex.lexemeStr,sTable);
        // check whether field name is present
        bool isPresent = false;
        for(int i = 0;i < sTable->allTypes[t1]->numFields;i++){
            if(strcmp(fieldNode->elem->lex.lexemeStr,sTable->entries[i]->varName)==0)
                isPresent = true;
        }
        if(isPresent){
            root = fieldNode;
        }
        else{
            printf("The field %s is not present in %s \n",fieldNode->elem->lex.lexemeStr,root->elem->lex.lexemeStr);
            return -1;
        }
    }
    return typeCheck(root,G,sTable,index);
}
int matchReturnParams(astNode *temp, symbolTable *sTable, int index, grammar G){
    int numOutputs = sTable->tables[index]->function->numOut;
    int outputParamsIndex = sTable->tables[index]->function->outId;
    int i = 0;
    int t1 = -2;
    while(i < numOutputs){
        if(strcmp(sTable->allTypes[outputParamsIndex]->fields[i],temp->child->elem->lex.lexemeStr)!=0){
            return -1;
        }
        else{
            temp = findChild(temp,1,false,0);
            i++;
        }
    }
    return t1;
}
int fIndex(char* fName, symbolTable* sTable) { 

    if (strcmp(fName, "main") == 0) { 
        return sTable->numF; 
    }
    for (int i = 0; i < sTable->numF; i++) { 
        if (strcmp(sTable->tables[i]->function->fId, fName) == 0) { 
            return i; 
        }
    } 
    return -1; 
}

int typeCheck(astNode* root, grammar G, symbolTable* sTable, int index) { 

    if (root == NULL) { 
        printf("Empty AST ERROR \n"); 
        return NULL; 
    } 
    if (root->elem->isLeaf == 0) { // Non-terminals 
        
        if (strcmp(G.nonTerminals[root->elem->curr], "function") == 0) { 
            astNode* f = root->child; 
            if (f->elem->isLeaf == 1 && strcmp(G.terminals[f->elem->curr], "TK_FUNID") == 0) { 
                index = fIndex(f->elem->lex.lexemeStr,sTable); 
            } 
            else { 
                printf("First child of <function> is not TK_FUNID \n"); 
                return -1; 
            } 
            // index = fInd; 
            int t = 0; 
            while (f != NULL) { 
                t = typeCheck(f, G, sTable, index); 
                if (t == -1) { 
                    return t; 
                }  
            } 
            return -2; // Everything is fine, type returned is hence void 
        } 
        else if (strcmp(G.nonTerminals[root->elem->curr], "mainFunction") == 0) { 
            index = fIndex("main",sTable); 
            // index = fInd; 
            astNode* f = root->child; 
            int t = 0; 
            while (f != NULL) { 
                t = typeCheck(f, G, sTable, index); 
                if (t == -1) { 
                    return t; 
                }  
                f = f->next;    // move to the next node 
            } 
            return -2; // Everything is fine, type returned is hence void 
        }
        else if (strcmp(G.nonTerminals[root->elem->curr], "assignmentStmt") == 0) { 
            // old
            int t1 = typeCheck(root->child, G, sTable, index); 
            int t2 = typeCheck(root->child->next->next, G, sTable, index); 

            if (t1 == t2) { 
                if (t1 == -1) { 
                    // printf("Error at line %d \n", root->child->elem->lineNo); 
                    return -1; 
                } 
                else { 
                    printf("Both sides of = have the same type \n"); 
                    return -2; // Everything is fine, type returned is hence void 
                }
            } 
            else { 
                if (t1 == -1 || t2 == -1) { 
                    // printf("Error at line %d \n", root->child->elem->lineNo); 
                    return -1; 
                } 
                else { 
                    printf("Both sides of = do not have the same type ERROR \n"); 
                    return -1; 
                }
            } 
        }
        else if(strcmp(G.nonTerminals[root->elem->curr], "iterativeStmt") == 0){
            int t1 = typeCheck(root->child,G,sTable,index);
            astNode *temp = root->child->next;
            int t2 = -2;
            while(!(temp->elem->curr && strcmp(G.terminals[temp->elem->curr],"eps")==0)){
                if(typeCheck(temp,G,sTable,index)==-1)t2=-1;
                temp = temp->next;
            }
            return t1==-3? t2: -1;
        } 
        else if (strcmp(G.nonTerminals[root->elem->curr], "booleanExpression") == 0) { 
            astNode *temp = root->child->next;
            if(isLogOp(temp,G)){
                // <booleanExpression> ===> TK_OP <booleanExpression1> TK_CL <logicalOp> TK_OP <booleanExpression2> TK_CL
                int t1 = typeCheck(root->child,G,sTable,index);
                int t2 = typeCheck(root->child->next->next,G,sTable,index);
                return t1==-3 && t2==-3 ? -3: -1;

            }
            else if(root->child->elem->isLeaf && strcmp(G.terminals[root->child->elem->curr],"TK_NOT")==0){
                // <booleanExpression> ===> TK_NOT TK_OP <booleanExpression1> TK_CL
                int t1 = typeCheck(root->child->next,G,sTable,index);
                if(t1 == -3)return -3;
                else{
                    printf("Invalid operands for boolean expression on line %d\n", root->child->elem->lineNo);
                }
            }
            else{
                // case
                // <booleanExpression> ===> <var> <relationalOp> <var1>
                astNode *temp = root->child;
                while(!(root->elem->isLeaf && strcmp(G.terminals[root->elem->curr],"eps"))){
                    temp = temp->next;
                }
                // Now temp is pointing to the node before logical Operator
                int t1 = findNestedType(root->child, G, sTable, index);
                int t2 = findNestedType(temp->next->next, G, sTable, index);
                if(isRelOp(temp->next,G) && (t1==0 || t1 == 1) && (t2 == 0 || t2 == 1))
                    return -3;
                else{
                    printf("Invalid operands for boolean expression on line %d\n", temp->next->elem->lineNo);
                    return -1;
                }
                
            }
            return -1;
        }
        else if (strcmp(G.nonTerminals[root->elem->curr], "ioStmt") == 0) { 
            int t1 = -1;
            if(strcmp(G.terminals[root->child->elem->curr],"TK_READ")==0){
                t1 = findNestedType(root->child->next,G,sTable,index);
            }
            else if(strcmp(G.terminals[root->child->elem->curr],"TK_WRITE")==0){
                t1 = findNestedType(root->child->next,G,sTable,index);
            }
            if(t1 == 0 || t1 == 1) return -2;
            else{
                printf("Cannot perform io operation on line %d\n",root->child->elem->lineNo);
                return -1;
            }
        }
        else if (strcmp(G.nonTerminals[root->elem->curr], "conditionalStmt") == 0) { 
            // <conditionalStmt> ===> TK_IF TK_OP <booleanExpression> TK_CL TK_THEN <stmt> <otherStmts> <elsePart>
            int t1 = typeCheck(findChild(root,1,false,0),G,sTable,index);
            int t2 = -2;
            astNode *temp = findChild(root,3,false,0);
            while(!(temp->elem->isLeaf && strcmp(G.terminals[temp->elem->curr],"eps"))){    // till otherStmts = eps is encountered
                if(typeCheck(temp,G,sTable,index)==-1)t2 = -1;
                temp = temp->next;
            }
            if(t1 != -3){
                printf("Condition for if is incorrect at line %d\n",root->child->elem->lineNo);
            }
            else if(t2 != -2){
                printf("Incorrect stmts inside if block %d\n",root->child->elem->lineNo);
            }
            else{ 
                temp = findChild(root,5,false,0);
                //<elsePart> ===> TK_ENDIF
                if(temp->elem->isLeaf && strcmp(G.terminals[temp->elem->curr],"TK_ENDIF")==0) return -2;
                return typeCheck(temp,G,sTable,index); // type check elsePart
            }
        }
        else if (strcmp(G.nonTerminals[root->elem->curr], "elsePart") == 0) { 
            // <elsePart> ===> TK_ELSE <stmt> <otherStmts> TK_ENDIF
            astNode *temp = root->child;
            int t1 = -2;
            while(!(temp->elem->isLeaf && strcmp(G.terminals[temp->elem->curr],"eps")==0)){
                if(typeCheck(temp,G,sTable,index)==-1)t1 = -1;
                temp = temp->next;
            }
            if(t1 == -2 && temp->next->elem->isLeaf && strcmp(G.terminals[temp->next->elem->curr],"TK_ENDIF")==0) return -2;
            else{
                printf("There is error in else part at line %d\n",temp->next->elem->lineNo);
                return -1;
            } 
            
        }
        else if (strcmp(G.nonTerminals[root->elem->curr], "returnStmt") == 0) { 
            astNode *temp = findChild(root,1,false,0);
            // If function is main
            if(index == sTable->numF){
                if(!(root->child->next->elem->isLeaf && strcmp(G.terminals[root->child->next->elem->curr],"eps"))){
                    printf("Main function cannot return values at line %d\n",root->child->elem->lineNo);
                    return -1;
                }
                else{
                    return -2;
                }
            }
            int t1 = matchReturnParams(temp,sTable,index,G);
            if (t1!=-2){
                printf("Return parameters at line %d do not match with the function definition\n",root->child->elem->lineNo);
                return -1;
            }
            return -2;

        }
        else if (strcmp(G.nonTerminals[root->elem->curr], "arithmeticExpression") == 0) { 
            

        }
    } 
    else { // Terminals 
        if (strcmp(G.terminals[root->elem->curr], "TK_NUM") == 0) { 
            return 0; 
        } 
        else if (strcmp(G.terminals[root->elem->curr], "TK_RNUM") == 0) { 
            return 1; 
        } 
        else if (strcmp(G.terminals[root->elem->curr], "TK_ID") == 0) { 
            int ind = searchS(root->elem->lex.lexemeStr, sTable->tables[index]->entries); 
            int t = sTable->tables[index]->entries[ind]->type; 
            return t; 
        }
    }
}