// #pragma once
#include <stdio.h> 
#include <stdbool.h> 
#include "string.h" 
#include "symbolTable.h" 
// #include "ast.c" 

// TK_NUM: 0
// TK_RNUM: 1
// booleanExpression: -3
// Error : -1 
// Void : -2 

bool isLogOp(astNode *root, grammar G) {
    if(root->elem->isLeaf){
        int curr = root->elem->curr;
        curr--;
        if(curr == TK_AND || curr == TK_OR) return true;
    }
    else return false;
} 

bool isRelOp(astNode *root, grammar G) {
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
            false;
        }
    }
} 

int findIDtype(astNode* root, grammar G, symbolTable* sTable, int index) { 

    // printf("In fIT %s \n", root->elem->lex.lexemeStr); 
    int indF = -1; 
    int t1 = -1; 
    if (sTable->tables[index]->numEntries != 0) { 
        indF = searchS(root->elem->lex.lexemeStr, sTable->tables[index]->entries); 
    } 
    if (indF != -1) { 
        int t = sTable->tables[index]->entries[indF]->type; 
        if (sTable->allTypes[t]->ref == -1) { 
            t1 = t; 
        } 
        else { 
            t1 = sTable->allTypes[t]->ref; 
        }
    } 

    int indInp = -1; 
    int t2 = -1; 
    if (sTable->tables[index]->function->inId != -1) { 
        indInp = searchS(root->elem->lex.lexemeStr, sTable->allTypes[sTable->tables[index]->function->inId]->fields); 
    } 
    if (indInp != -1) { 
        int t = sTable->allTypes[sTable->tables[index]->function->inId]->fields[indInp]->type; 
        if (sTable->allTypes[t]->ref == -1) { 
            t2 = t; 
        } 
        else { 
            t2 = sTable->allTypes[t]->ref; 
        }
    } 

    int indOut = -1; 
    int t3 = -1; 
    if (sTable->tables[index]->function->outId != -1) { 
        indOut = searchS(root->elem->lex.lexemeStr, sTable->allTypes[sTable->tables[index]->function->outId]->fields); 
    } 
    if (indOut != -1) { 
        int t = sTable->allTypes[sTable->tables[index]->function->outId]->fields[indOut]->type; 
        if (sTable->allTypes[t]->ref == -1) { 
            t3 = t; 
        } 
        else { 
            t3 = sTable->allTypes[t]->ref; 
        }
    } 

    int indG = -1; 
    int t4 = -1; 
    if (sTable->numEntries != 0) { 
        indG = searchS(root->elem->lex.lexemeStr, sTable->entries); 
    } 
    if (indG != -1) { 
        int t = sTable->entries[indG]->type; 
        if (sTable->allTypes[t]->ref == -1) { 
            t4 = t; 
        } 
        else { 
            t4 = sTable->allTypes[t]->ref; 
        }
    } 

    if (t1 == -1) { 
        if (t2 == -1) { 
            if (t3 == -1) { 
                if (t4 == -1) { 
                    printf("Variable %s not defined at line %d ERROR \n", root->elem->lex.lexemeStr, root->elem->lineNo); 
                    return -1; 
                } 
                else { 
                    return t4; 
                }
            } 
            else { 
                return t3; 
            }
        } 
        else { 
            return t2; 
        }
    } 
    else { 
        printf("Variable %s declared in this function with type %d \n", root->elem->lex.lexemeStr, t1); 
        // printf("%s \n", sTable->allTypes[t1]->name); 
        return t1; 
    } 
}

int findNestedType(astNode *root, grammar G, symbolTable *sTable, int index) {   // used for <var>
    
    // astNode* fc = root; 

    // while (fc != NULL) { 
    //     printf("%d %d ", fc->elem->isLeaf, fc->elem->curr); 
    //     if (fc->elem->isLeaf == 1) { 
    //         printf("'%s' \n", G.terminals[fc->elem->curr]); 
    //     } 
    //     else { 
    //         printf("'%s' \n", G.nonTerminals[fc->elem->curr]); 
    //     } 
    //     fc = fc->next; 
    // } 
    
    if (root->elem->isLeaf == 1 && strcmp(G.terminals[root->elem->curr], "TK_NUM") == 0) { 
        return 0; 
    } 
    else if (root->elem->isLeaf == 1 && strcmp(G.terminals[root->elem->curr], "TK_RNUM") == 0) { 
        return 1; 
    }
    
    int t1 = findIDtype(root, G, sTable, index); 
    // printf("done in fNT t1 is %d \n", t1); 
    // printf("In fNT %s %d \n", G.terminals[root->elem->curr], t1); 
    if (t1 == -1) { 
        return -1; 
    }

    int tret; 
    // printf("%s %d \n", root->elem->lex.lexemeStr, t1); 
    if (root->next->elem->isLeaf == 1 && root->next->elem->curr == 0) { 
        return t1; 
    } 

    // printf("fNT next check %d %d \n", root->next->elem->isLeaf, root->next->elem->curr); 

    while (!(root->next->elem->isLeaf && root->next->elem->curr == 0)){ 
        astNode *fieldNode = root->next;
        // check whether field name is present
        // printf("%s \n", fieldNode->elem->lex.lexemeStr); 
        bool isPresent = false;
        for(int i = 0; i < HASH_SIZE; i++){
            if(sTable->allTypes[t1]->fields[i]->present == 1 && (strcmp(fieldNode->elem->lex.lexemeStr, sTable->allTypes[t1]->fields[i]->varName)==0)) { 
                isPresent = true; 
                t1 = sTable->allTypes[t1]->fields[i]->type; 
                // printf("In fNT T1 %d \n", t1); 
                break; 
            } 
        }
        if(isPresent){
            root = fieldNode; 
        }
        else{
            printf("The field %s is not present in %s \n",fieldNode->elem->lex.lexemeStr,root->elem->lex.lexemeStr);
            return -1;
        } 
    }
    // Last TK_FIELDID node reached i.e. return its type 
    // int test = searchS("maths", sTable->allTypes[t1]->fields); 
    // printf("%d \n", sTable->allTypes[t1]->fields[test]->type); 
    // printf("%d \n", tret); 
    return t1; 
    // return typeCheck(root,G,sTable,index);
} 

int matchReturnParams(astNode *temp, grammar G, symbolTable *sTable, int index) {
    
    int numOutputs = sTable->tables[index]->function->numOut;
    int outputParamsIndex = sTable->tables[index]->function->outId;
    
    // printf("In matchReturnParams \n"); 
    // astNode* fc = temp; 

    // while (fc != NULL) { 
    //     printf("%d %d ", fc->elem->isLeaf, fc->elem->curr); 
    //     if (fc->elem->isLeaf == 1) { 
    //         printf("'%s' \n", G.terminals[fc->elem->curr]); 
    //     } 
    //     else { 
    //         printf("'%s' \n", G.nonTerminals[fc->elem->curr]); 
    //     } 
    //     fc = fc->next; 
    // } 
    
    // temp = temp->child; 
    for (int i = 0; i < numOutputs; i++) { 
        
        if (temp == NULL) { 
            printf("Return statement incomplete ERROR \n"); 
            return -1; 
        }
        
        int t1 = findIDtype(temp, G, sTable, index); 
        if (t1 == sTable->allTypes[outputParamsIndex]->fields[sTable->tables[index]->function->outOrder[i]]->type) { 
            // Fine 
        } 
        else { 
            printf("Variable %s in return statement does not match output parameter at line %d ERROR \n", temp->elem->lex.lexemeStr, temp->elem->lineNo); 
            return -1; 
        } 
        temp = temp->next; 
    }
    
    printf("%s \n", G.terminals[temp->elem->curr]); 
    if (temp != NULL) { 
        if (!(temp->elem->isLeaf == 1 && temp->elem->curr == 0)) { 
            printf("Extra  return statement parameters at line %d ERROR \n", temp->elem->lineNo); 
            return -1; 
        } 
        else { 
            printf("Valid return statement \n"); 
            return -2; 
        }
    } 
    else { 
        printf("Valid return statement \n"); 
        return -2; 
    } 
}

int fIndex(char* fName, symbolTable* sTable) { 

    if (strcmp(fName, "main") == 0) { 
        return sTable->numF - 1; 
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
        return -2; 
    } 
    if (root->elem->isLeaf == 0) { // Non-terminals 
        
        if (strcmp(G.nonTerminals[root->elem->curr], "program") == 0) { 
            int t = 0; 
            astNode* f = root->child; 
            printf("In program \n"); 
            while (f != NULL) { 
                t = typeCheck(f, G, sTable, index); 
                if (t == -1) { 
                    return t; 
                } 
                f = f->next;    // move to the next node 
            } 
            return -2; // Everything is fine, type returned is hence void 
        }
        if (strcmp(G.nonTerminals[root->elem->curr], "function") == 0) { 
            astNode* f = root->child; 
            int line = root->child->elem->lineNo; 
            printf("In function \n"); 
            if (f->elem->isLeaf == 1 && strcmp(G.terminals[f->elem->curr], "TK_FUNID") == 0) { 
                index = fIndex(f->elem->lex.lexemeStr,sTable); 
                // printf("done %d \n", index); 
            } 
            else { 
                printf("First child of <function> is not TK_FUNID at line %d ERROR \n", line); 
                return -1; 
            } 
            // index = fInd; 
            int t = 0; 
            while (f != NULL) { 
                // if (f->elem->isLeaf == 0) { 
                //     printf("checking %s in function \n", G.nonTerminals[f->elem->curr]); 
                // } 
                // else { 
                //     printf("checking %s in function \n", G.terminals[f->elem->curr]); 
                // } 
                t = typeCheck(f, G, sTable, index); 
                if (t == -1) { 
                    return t; 
                } 
                f = f->next;    // move to the next node 
            } 
            return -2; // Everything is fine, type returned is hence void 
        } 
        else if (strcmp(G.nonTerminals[root->elem->curr], "mainFunction") == 0) { 
            index = fIndex("main", sTable); 
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
            // int t1 = typeCheck(root->child, G, sTable, index); 
            
            // printf("In assignmentStmt \n"); 
            // astNode* fc = root->child; 

            // while (fc != NULL) { 
            //     printf("%d %d ", fc->elem->isLeaf, fc->elem->curr); 
            //     if (fc->elem->isLeaf == 1) { 
            //         printf("'%s' \n", G.terminals[fc->elem->curr]); 
            //     } 
            //     else { 
            //         printf("'%s' \n", G.nonTerminals[fc->elem->curr]); 
            //     } 
            //     fc = fc->next; 
            // } 
            
            int t1 = typeCheck(root->child, G, sTable, index); 
            // printf("done in assignmentStmt %d \n", t1); 
            astNode* temp = root->child; 
            while (!(temp->elem->isLeaf == 1 && strcmp(G.terminals[temp->elem->curr], "TK_ASSIGNOP") == 0)) { 
                temp = temp->next; 
            } 
            int line = temp->elem->lineNo; 
            temp = temp->next; 
            // printf("%s \n", G.nonTerminals[temp->elem->curr]);
            int t2 = typeCheck(temp, G, sTable, index); 

            if (t1 == t2) { 
                if (t1 == -1) { 
                    // printf("Error at line %d \n", root->child->elem->lineNo); 
                    return -1; 
                } 
                else { 
                    printf("Both sides of = have the same type at line %d \n", line); 
                    return -2; // Everything is fine, type returned is hence void 
                }
            } 
            else { 
                if (t1 == -1 || t2 == -1) { 
                    // printf("Error at line %d \n", root->child->elem->lineNo); 
                    return -1; 
                } 
                else { 
                    printf("Both sides of = do not have the same type at line %d ERROR \n", line); 
                    return -1; 
                }
            } 
        } 
        else if (strcmp(G.nonTerminals[root->elem->curr], "arithmeticExpression") == 0) { 
            
            
            // First while saving types : 
            // -4 : MUL 
            // -5 : DIV 
            // -6 : PLUS 
            // -7 : MINUS 

            // For operator overloading info stored in lex.numval : 
            // MUL : 0 if int x int (returns int)
            // MUL : 1 if real x real (returns real) 
            // MUL : 3 if record x int (returns record) 
            // MUL : 6 if int x record (returns record)
            // MUL : 4 if int x real (returns real) 
            // MUL : 5 if real x int (returns real)
            // DIV : 0 if real x real (returns real)
            // DIV : 1 if int x int (returns real) 
            // DIV : 3 if record x int (returns record) 
            // DIV : 4 if int x real (returns real) 
            // DIV : 5 if real x int (returns real) 
            // PLUS : 0 if int x int (returns int) 
            // PLUS : 1 if real x real (returns real) 
            // PLUS : 3 if record x record (returns record) 
            // PLUS : 4 if int x real (returns real) 
            // PLUS : 5 if real x int (returns real) 
            // MINUS : 0 if int x int (returns int) 
            // MINUS : 1 if real x real (returns real) 
            // MINUS : 3 if record x record (returns record) 
            // MINUS : 4 if int x real (returns real) 
            // MINUS : 5 if real x int (returns real) 
            
            
            int* types; 
            // int* startNodeNo; 
            
            // printf("ARITHMETIC \n"); 
            // astNode* fc = root->child; 

            // while (fc != NULL) { 
            //     printf("%d %d ", fc->elem->isLeaf, fc->elem->curr); 
            //     if (fc->elem->isLeaf == 1) { 
            //         printf("'%s' \n", G.terminals[fc->elem->curr]); 
            //     } 
            //     else { 
            //         printf("'%s' \n", G.nonTerminals[fc->elem->curr]); 
            //     } 
            //     fc = fc->next; 
            // } 
            
            // printf("Done printing \n"); 
            astNode* temp = root->child; 
            // printf("done \n"); 
            int count = 0; 
            // int node = 0; 
            int line; 
            while (temp != NULL) { 
                if (temp->elem->isLeaf == 0 && strcmp(G.nonTerminals[temp->elem->curr], "arithmeticExpression") == 0) { 
                    // printf("AE \n"); 
                    count += 1; 
                    if (count == 1) { 
                        types = (int*) malloc(sizeof(int)); 
                    } 
                    else { 
                        types = realloc(types, (sizeof(int) * count)); 
                    } 
                    types[count - 1] = typeCheck(temp, G, sTable, index); 
                    temp = temp->next; 

                } 
                else if (temp->elem->isLeaf == 1) { 
                    if (temp->elem->curr == 0) { 
                        // printf("eps \n"); 
                        // skip 
                        temp = temp->next; 
                    } 
                    else if (strcmp(G.terminals[temp->elem->curr], "TK_NUM") == 0 || strcmp(G.terminals[temp->elem->curr], "TK_RNUM") == 0 || strcmp(G.terminals[temp->elem->curr], "TK_ID") == 0) { 
                        // printf("var %s \n", G.terminals[temp->elem->curr]); 
                        line = temp->elem->lineNo; 
                        count += 1; 
                        if (count == 1) { 
                            types = (int*) malloc(sizeof(int)); 
                        } 
                        else { 
                            types = realloc(types, (sizeof(int) * count)); 
                        } 
                        types[count - 1] = findNestedType(temp, G, sTable, index); 
                        // printf("done %d \n", types[count - 1]); 
                        if (strcmp(G.terminals[temp->elem->curr], "TK_ID") == 0) { 
                            // printf("id \n"); 
                            while (temp->elem->curr != 0) { 
                                temp = temp->next; 
                            }
                        } 
                        else { 
                            temp = temp->next; 
                        }
                    } 
                    else { 
                        // printf("op \n"); 
                        if (strcmp(G.terminals[temp->elem->curr], "TK_MUL") == 0) { 
                            count += 1; 
                            if (count == 1) { 
                                types = (int*) malloc(sizeof(int)); 
                            } 
                            else { 
                                types = realloc(types, (sizeof(int) * count)); 
                            } 
                            types[count - 1] = -4; 
                            temp = temp->next; 
                        } 
                        else if (strcmp(G.terminals[temp->elem->curr], "TK_DIV") == 0) { 
                            count += 1; 
                            if (count == 1) { 
                                types = (int*) malloc(sizeof(int)); 
                            } 
                            else { 
                                types = realloc(types, (sizeof(int) * count)); 
                            } 
                            types[count - 1] = -5; 
                            temp = temp->next; 
                        } 
                        else if (strcmp(G.terminals[temp->elem->curr], "TK_PLUS") == 0) { 
                            count += 1; 
                            if (count == 1) { 
                                types = (int*) malloc(sizeof(int)); 
                            } 
                            else { 
                                types = realloc(types, (sizeof(int) * count)); 
                            } 
                            types[count - 1] = -6; 
                            temp = temp->next; 
                        } 
                        else if (strcmp(G.terminals[temp->elem->curr], "TK_MINUS") == 0) { 
                            count += 1; 
                            if (count == 1) { 
                                types = (int*) malloc(sizeof(int)); 
                            } 
                            else { 
                                types = realloc(types, (sizeof(int) * count)); 
                            } 
                            types[count - 1] = -7; 
                            temp = temp->next; 
                        } 
                    }
                } 
            } 

            // printf("done \n"); 
            // for (int i = 0; i < count; i++) { 
            //     printf("types[%d] = %d \n", i, types[i]); 
            // } 

            // printf("Done printing \n"); 
            
            int count2 = 0; 
            int* inds; 
            int cInd = 0; 
            temp = root->child; 
            while (count2 < count) { // For multiplication and division 
                if (temp->elem->isLeaf == 0) { 
                    count2 += 1; 
                    temp = temp->next; 
                } 
                else { 
                    if (temp->elem->curr == 0) { 
                        // skip 
                        temp = temp->next; 
                    } 
                    else if (strcmp(G.terminals[temp->elem->curr], "TK_MUL") == 0 || strcmp(G.terminals[temp->elem->curr], "TK_DIV") == 0) { 
                        
                        if (types[count2] == -4) { 
                            
                            if (cInd == 0) { 
                                inds = (int*) malloc(sizeof(int)); 
                            } 
                            else { 
                                inds = realloc(inds, sizeof(int) * (cInd + 1)); 
                            } 
                            inds[cInd] = count2; 
                            cInd += 1; 
                            
                            int t1 = types[count2 - 1]; 
                            int t2 = types[count2 + 1]; 
                            if (t1 == t2) { 
                                if (t1 == 0) { 
                                    types[count2] = 0; 
                                    types[count2 - 1] = 0; 
                                    types[count2 + 1] = 0; 
                                    temp->elem->lex.numVal = 0; 
                                } 
                                else if (t1 == 1) { 
                                    types[count2] = 1; 
                                    types[count2 - 1] = 1; 
                                    types[count2 + 1] = 1; 
                                    temp->elem->lex.numVal = 1; 
                                } 
                                else { 
                                    types[count2] = -1; 
                                    types[count2 - 1] = -1; 
                                    types[count2 + 1] = -1; 
                                    printf("Wrong types of matching operands for TK_MUL at line %d ERROR \n", line ); 
                                }
                            } 
                            else { 
                                if (t1 == 0 && t2 == 1) { 
                                    types[count2] = 1; 
                                    types[count2 - 1] = 1; 
                                    types[count2 + 1] = 1; 
                                    temp->elem->lex.numVal = 4;
                                } 
                                else if (t1 == 1 && t2 == 0) { 
                                    types[count2] = 1; 
                                    types[count2 - 1] = 1; 
                                    types[count2 + 1] = 1; 
                                    temp->elem->lex.numVal = 5; 
                                }
                                else if (t1 == 0 && t2 != -1) { 
                                    types[count2] = t2; 
                                    types[count2 - 1] = types[count2]; 
                                    types[count2 + 1] = types[count2]; 
                                    temp->elem->lex.numVal = 6; 
                                } 
                                else if (t1 != -1 && t2 == 0) { 
                                    types[count2] = t1; 
                                    types[count2 - 1] = types[count2]; 
                                    types[count2 + 1] = types[count2]; 
                                    temp->elem->lex.numVal = 3; 
                                } 
                                else { 
                                    types[count2] == -1; 
                                    types[count2 - 1] = -1; 
                                    types[count2 + 1] = -1; 
                                    printf("Operand types do not match for TK_MUL at line %d ERROR \n", line); 
                                } 
                            }
                        } 
                        else if (types[count2] == -5) { 
                            
                            if (cInd == 0) { 
                                inds = (int*) malloc(sizeof(int)); 
                            } 
                            else { 
                                inds = realloc(inds, sizeof(int) * (cInd + 1)); 
                            } 
                            inds[cInd] = count2; 
                            cInd += 1; 
                            
                            int t1 = types[count2 - 1]; 
                            int t2 = types[count2 + 1]; 
                            if (t1 == t2) { 
                                if (t1 == 0 || t1 == 1) { 
                                    types[count2] = 1; 
                                    types[count2 - 1] = 1; 
                                    types[count2 + 1] = 1; 
                                    temp->elem->lex.numVal = types[count2]; 
                                } 
                                else { 
                                    types[count2] = -1; 
                                    types[count2 - 1] = -1; 
                                    types[count2 + 1] = -1; 
                                    printf("Wrong types of matching operands for TK_DIV at line %d ERROR \n", line); 
                                }
                            } 
                            else { 
                                if (t1 == 0 && t2 == 1) { 
                                    types[count2] = 1; 
                                    types[count2 - 1] = 1; 
                                    types[count2 + 1] = 1; 
                                    temp->elem->lex.numVal = 4; 
                                } 
                                else if (t1 == 1 && t2 == 0) { 
                                    types[count2] = 1; 
                                    types[count2 - 1] = 1; 
                                    types[count2 + 1] = 1; 
                                    temp->elem->lex.numVal = 5; 
                                } 
                                else if (t1 != -1 && t2 == 0) { 
                                    types[count2] = t1; 
                                    types[count2 - 1] = types[count2]; 
                                    types[count2 + 1] = types[count2]; 
                                    temp->elem->lex.numVal = 3; 
                                } 
                                else { 
                                    types[count2] = -1; 
                                    types[count2 - 1] = -1; 
                                    types[count2 + 1] = -1; 
                                    printf("Operand types do not match for TK_DIV at line %d ERROR \n", line); 
                                }
                            }
                        } 
                        
                        // temp->elem->lex.numVal = types[count2]; 
                        temp = temp->next; 
                        count2 += 1; 
                    } 
                    else if (strcmp(G.terminals[temp->elem->curr], "TK_ID") == 0) { 
                        count2 += 1; 
                        while (temp->elem->curr != 0) { 
                            temp = temp->next; 
                            // node += 1; 
                        } 
                    } 
                    else { 
                        count2 += 1; 
                        temp = temp->next; 
                    }
                } 
            } 

            printf("First pass in arithmetic done \n"); 

            int* newT = (int*) malloc(sizeof(int) * (count - (cInd * 3) + cInd)); 
            int j = 0; 
            int i = 0; 
            int k = 0; 
            if (cInd != 0) { 
                while (i < count) { 
                    if (inds[j] == i + 1) { 
                        newT[k] = types[i + 1]; 
                        k += 1; 
                        j += 1; 
                        i += 3; 
                    } 
                    else { 
                        newT[k] = types[i]; 
                        k += 1; 
                        j += 1; 
                        i += 1; 
                    }
                } 
            } 
            else { 
                while (i < count) { 
                    newT[i] = types[i]; 
                    i += 1; 
                }
            }
            count = count - (cInd * 3) + cInd; 
            // printf("Copy done: %d %d \n", count, newT[0]); 
            // for (int i = 0; i < count; i++) { 
            //     printf("newT[%d] = %d \n", i, newT[i]); 
            // }

            count2 = 0; 
            temp = root->child; 
            int* inds2; 
            int cInd2 = 0; 
            while (count2 < count) { // For addition and subtraction 
                if (temp->elem->isLeaf == 0) { 
                    count2 += 1; 
                    temp = temp->next; 
                } 
                else { 
                    if (temp->elem->curr == 0) { 
                        // skip 
                        temp = temp->next; 
                    } 
                    else if (strcmp(G.terminals[temp->elem->curr], "TK_PLUS") == 0 || strcmp(G.terminals[temp->elem->curr], "TK_MINUS") == 0) { 
                        
                        if (newT[count2] == -6 || newT[count2] == -7) { 
                            
                            if (cInd2 == 0) { 
                                inds2 = (int*) malloc(sizeof(int)); 
                            } 
                            else { 
                                inds2 = realloc(inds2, sizeof(int) * (cInd2 + 1)); 
                            } 
                            inds2[cInd2] = count2; 
                            cInd2 += 1; 
                            
                            int t1 = newT[count2 - 1]; 
                            int t2 = newT[count2 + 1]; 
                            if (t1 == t2) { 
                                if (t1 == 0) { 
                                    newT[count2] = 0; 
                                    newT[count2 - 1] = 0; 
                                    newT[count2 + 1] = 0; 
                                    temp->elem->lex.numVal = 0; 
                                } 
                                else if (t1 == 1) { 
                                    newT[count2] = 1; 
                                    newT[count2 - 1] = 1; 
                                    newT[count2 + 1] = 1; 
                                    temp->elem->lex.numVal = 1; 
                                } 
                                else if (t1 != -1) { 
                                    newT[count2] = t1; 
                                    newT[count2 - 1] = t1; 
                                    newT[count2 + 1] = t1; 
                                    temp->elem->lex.numVal = 3; 
                                } 
                                else { 
                                    newT[count2] = -1; 
                                    newT[count2 - 1] = -1; 
                                    newT[count2 + 1] = -1; 
                                    printf("Wrong types of matching operands for TK_PLUS/TK_MINUS at line %d ERROR \n", line); 
                                } 
                            } 
                            else { 
                                
                                if (t1 == 0 && t2 == 1) { 
                                    newT[count2] = 1; 
                                    newT[count2 - 1] = 1; 
                                    newT[count2 + 1] = 1; 
                                    temp->elem->lex.numVal = 4; 
                                } 
                                else if (t1 == 1 && t2 == 0) { 
                                    newT[count2] = 1; 
                                    newT[count2 - 1] = 1; 
                                    newT[count2 + 1] = 1; 
                                    temp->elem->lex.numVal = 5; 
                                } 
                                else { 
                                    newT[count2] = -1; 
                                    newT[count2 - 1] = -1; 
                                    newT[count2 + 1] = -1; 
                                    printf("Operand types do not match for TK_PLUS/TK_MINUS at line %d ERROR \n", line); 
                                } 
                            }
                        } 

                        // temp->elem->lex.numVal = types[count2]; 
                        temp = temp->next; 
                        count2 += 1; 
                    } 
                    else if (strcmp(G.terminals[temp->elem->curr], "TK_ID") == 0) { 
                        count2 += 1; 
                        while (temp->elem->curr != 0) { 
                            temp = temp->next; 
                            // node += 1; 
                        } 
                    } 
                    else { 
                        count2 += 1; 
                        temp = temp->next; 
                    }
                } 
            } 

            printf("Second pass in arithmetic done \n"); 
            int* fT = (int*) malloc(sizeof(int) * (count - (cInd2 * 3) + cInd2)); 
            j = 0; 
            i = 0; 
            k = 0; 
            if (cInd2 != 0) { 
                while (i < count) { 
                    if (inds2[j] == i + 1) { 
                        fT[k] = newT[i + 1]; 
                        k += 1; 
                        j += 1; 
                        i += 3; 
                    } 
                    else { 
                        fT[k] = newT[i]; 
                        k += 1; 
                        j += 1; 
                        i += 1; 
                    }
                } 
            } 
            else { 
                while (i < count) { 
                    fT[i] = newT[i]; 
                    i += 1; 
                }
            }
        
            count = count - (cInd2 * 3) + cInd2; 
            // printf("Done copying : %d %d \n", count, fT[0]); 
            // for (int i = 0; i < count; i++) { 
            //     printf("fT[%d] = %d \n", i, fT[i]); 
            // }
            
            if (count != 1) { 
                printf("Check arithmetic expression for correctness at line %d ERROR \n", line); 
                return -1; 
            } 
            else { 
                return fT[0]; 
            } 

        } 
        else if (strcmp(G.nonTerminals[root->elem->curr], "funCallStmt") == 0) { 
            
            // printf("In funCallStmt \n"); 
            
            // astNode* fc = root->child; 

            // while (fc != NULL) { 
            //     printf("%d %d ", fc->elem->isLeaf, fc->elem->curr); 
            //     if (fc->elem->isLeaf == 1) { 
            //         printf("'%s' \n", G.terminals[fc->elem->curr]); 
            //     } 
            //     else { 
            //         printf("'%s' \n", G.nonTerminals[fc->elem->curr]); 
            //     } 
            //     fc = fc->next; 
            // }
            
            char* fName = root->child->next->elem->lex.lexemeStr; 
            int fInd = fIndex(fName, sTable); 
            int line = root->child->next->elem->lineNo; 
            if (fInd == -1) { 
                printf("Function %s not defined at line %d ERROR \n", fName, line); 
                return -1; 
            }
            // printf("The function is %s %d \n", fName, fInd); 
            int inputT = sTable->tables[fInd]->function->inId; 
            int outputT = sTable->tables[fInd]->function->outId; 
            // int t1 = typeCheck(root->child, G, sTable, index); 
            // int t2 = typeCheck(root->child->next->next, G, sTable, index); 
            
            // printf("done \n"); 
            astNode* inP = root->child->next->next->child; 
            
            // fc = root->child->next->next->child; 

            // while (fc != NULL) { 
            //     printf("%d %d ", fc->elem->isLeaf, fc->elem->curr); 
            //     if (fc->elem->isLeaf == 1) { 
            //         printf("'%s' \n", G.terminals[fc->elem->curr]); 
            //     } 
            //     else { 
            //         printf("'%s' \n", G.nonTerminals[fc->elem->curr]); 
            //     } 
            //     fc = fc->next; 
            // }
            
            if (inP->elem->isLeaf == 1 && inP->elem->curr == 0 && sTable->tables[fInd]->function->numIn == 0) { 
                printf("Input parameters valid and 0 at line %d \n", line); 
            } 
            else { 
                for (int i = 0; i < sTable->tables[fInd]->function->numIn; i++) { 
                    
                    if (inP->elem->isLeaf == 1 && inP->elem->curr == 0) { 
                        printf("Input parameters for function call incomplete ERROR \n"); 
                        return -1; 
                    } 

                    // printf("%s \n", inP->elem->lex.lexemeStr); 

                    int t1 = findIDtype(inP, G, sTable, index); 
                    // printf("%d \n", t1); 
                    if (t1 == sTable->allTypes[sTable->tables[fInd]->function->inId]->fields[sTable->tables[fInd]->function->inOrder[i]]->type) { 
                        // Fine 
                    } 
                    else { 
                        printf("Variable %s of input parameter list does not match at line %d ERROR \n", inP->elem->lex.lexemeStr, line); 
                        return -1; 
                    } 
                    inP = inP->next; 

                } 

                if (!(inP->elem->isLeaf == 1 && inP->elem->curr == 0)) { 
                    printf("Extra input parameters at line %d ERROR \n", line); 
                    return -1; 
                }
            } 

            printf("Input parameters are valid at line %d \n", line); 

            // fc = root->child->child; 

            // while (fc != NULL) { 
            //     printf("%d %d ", fc->elem->isLeaf, fc->elem->curr); 
            //     if (fc->elem->isLeaf == 1) { 
            //         printf("'%s' \n", G.terminals[fc->elem->curr]); 
            //     } 
            //     else { 
            //         printf("'%s' \n", G.nonTerminals[fc->elem->curr]); 
            //     } 
            //     fc = fc->next; 
            // } 

            // printf("%d \n", sTable->tables[fInd]->function->numOut); 
            astNode* outP = root->child->child; 
            if (outP->elem->isLeaf == 1 && outP->elem->curr == 0 && sTable->tables[fInd]->function->numOut == 0) { 
                printf("Output parameters valid and 0 at line %d \n", root->child->next->elem->lineNo); 
            } 
            else { 
                for (int i = 0; i < sTable->tables[fInd]->function->numOut; i++) { 
                
                    if (outP->elem->isLeaf == 1 && outP->elem->curr == 0) { 
                        printf("Output parameters for function call incomplete ERROR \n"); 
                        return -1; 
                    }
                
                    int t1 = findNestedType(outP, G, sTable, index); 
                    if (t1 == sTable->allTypes[sTable->tables[fInd]->function->outId]->fields[sTable->tables[fInd]->function->outOrder[i]]->type) { 
                        // Fine 
                    } 
                    else { 
                        printf("Variable %s of output parameter list does not match at line %d ERROR \n", outP->elem->lex.lexemeStr, outP->elem->lineNo); 
                        return -1; 
                    }
                    outP = outP->next; 
                } 

                if (!(outP->next->elem->isLeaf == 1 && strcmp(G.terminals[outP->next->elem->curr], "TK_ASSIGNOP") == 0)) { 
                    printf("Extra output parameters at line %d ERROR \n", outP->elem->lineNo); 
                    return -1; 
                }
            } 
            
            printf("Output parameters are valid at line %d \n", line); 

            return -2; 

        } 
        else if (strcmp(G.nonTerminals[root->elem->curr], "iterativeStmt") == 0) { // Line number not printed
            
            // astNode* fc = root->child; 

            // while (fc != NULL) { 
            //     printf("%d %d ", fc->elem->isLeaf, fc->elem->curr); 
            //     if (fc->elem->isLeaf == 1) { 
            //         printf("'%s' \n", G.terminals[fc->elem->curr]); 
            //     } 
            //     else { 
            //         printf("'%s' \n", G.nonTerminals[fc->elem->curr]); 
            //     } 
            //     fc = fc->next; 
            // } 
            
            int t; 
            int t1 = typeCheck(root->child,G,sTable,index);
            if (t1 == -3) { 
                astNode *f = root->child->next; 
                while (f != NULL) { 
                    t = typeCheck(f, G, sTable, index); 
                    if (t == -1) { 
                        return t; 
                    }  
                    f = f->next;    // move to the next node 
                } 
                return -2; 
            } 
            else { 
                return -1; 
            }
            
            // astNode *temp = root->child->next;
            // int t2 = -2;
            // while(!(temp->elem->curr && strcmp(G.terminals[temp->elem->curr],"eps")==0)){
            //     if(typeCheck(temp,G,sTable,index)==-1)t2=-1;
            //     temp = temp->next;
            // }
            // return t1==-3? t2: -1;
        } 
        else if (strcmp(G.nonTerminals[root->elem->curr], "booleanExpression") == 0) { 
            // printf("In booleanExpression \n"); 
            
            astNode* fc = root->child; 

            // while (fc != NULL) { 
            //     printf("%d %d ", fc->elem->isLeaf, fc->elem->curr); 
            //     if (fc->elem->isLeaf == 1) { 
            //         printf("'%s' \n", G.terminals[fc->elem->curr]); 
            //     } 
            //     else { 
            //         printf("'%s' \n", G.nonTerminals[fc->elem->curr]); 
            //     } 
            //     fc = fc->next; 
            // } 
            
            int line; 
            if (root->child->elem->isLeaf && strcmp(G.terminals[root->child->elem->curr],"TK_NOT")==0) { 
                // <booleanExpression> ===> TK_NOT TK_OP <booleanExpression1> TK_CL
                line = root->child->elem->lineNo; 
                int t1 = typeCheck(root->child->next,G,sTable,index);
                if (t1 == -3) { 
                    printf("Valid boolean expression at line %d \n", line); 
                    return -3; 
                } 
                else{
                    printf("Invalid operands for boolean expression on line %d \n", line); 
                    return -1; 
                }
            }
            else if(root->child->next != NULL && isLogOp(root->child->next, G)){
                // <booleanExpression> ===> TK_OP <booleanExpression1> TK_CL <logicalOp> TK_OP <booleanExpression2> TK_CL
                line = root->child->next->elem->lineNo; 
                int t1 = typeCheck(root->child,G,sTable,index);
                int t2 = typeCheck(root->child->next->next,G,sTable,index);
                if (t1 == -3 && t2 == -3) { 
                    printf("Valid boolean expression at line %d \n", line); 
                    return -3; 
                } 
                else { 
                    printf("Invalid operands for boolean expression on line %d \n", line); 
                    return -1; 
                }
                
                // return t1==-3 && t2==-3 ? -3: -1;

            } 
            else{
                // case
                // <booleanExpression> ===> <var> <relationalOp> <var1>
                astNode *temp = root->child;
                int t1 = findNestedType(root->child, G, sTable, index); 
                // printf("done \n"); 
                while (!(temp->elem->isLeaf == 1 && temp->elem->curr == 0)) { 
                    temp = temp->next; 
                } 
                temp = temp->next; 
                astNode* relOp = temp; 
                line = relOp->elem->lineNo; 
                temp = temp->next; 
                int t2 = findNestedType(temp, G, sTable, index); 
                if (isRelOp(relOp,G) && (t1==0 || t1 == 1) && (t2 == 0 || t2 == 1)) { 
                    printf("Valid boolean expression at line %d \n", line); 
                    return -3;
                } 
                else{
                    printf("Invalid operands for boolean expression on line %d \n", line);
                    return -1;
                }
                
            }
            return -1;
        }
        else if (strcmp(G.nonTerminals[root->elem->curr], "ioStmt") == 0) { 
            // printf("In ioStmt \n"); 
            int t1 = -1;
            int line; 
            if(strcmp(G.terminals[root->child->elem->curr],"TK_READ")==0){
                line = root->child->elem->lineNo; 
                t1 = findNestedType(root->child->next,G,sTable,index); 
                // printf("done %d \n", t1); 
            }
            else if(strcmp(G.terminals[root->child->elem->curr],"TK_WRITE")==0){
                line = root->child->elem->lineNo; 
                t1 = findNestedType(root->child->next,G,sTable,index);
            }
            if(t1 == 0 || t1 == 1) { 
                printf("Valid io statement at line %d \n", line); 
                return -2; 
            } 
            else{
                printf("Cannot perform io operation on line %d \n", line);
                return -1;
            }
        }
        else if (strcmp(G.nonTerminals[root->elem->curr], "conditionalStmt") == 0) { 
            // <conditionalStmt> ===> TK_IF TK_OP <booleanExpression> TK_CL TK_THEN <stmt> <otherStmts> <elsePart>
            // astNode* fc = findChild(root, 1, false, 0); 
            // int countN = 1; 
            // while (fc->next != NULL) { 
            //     // countN += 1; 
            //     printf("%d %d ", fc->elem->isLeaf, fc->elem->curr); 
            //     if (fc->elem->isLeaf == 1) { 
            //         printf("'%s' \n", G.terminals[fc->elem->curr]); 
            //     } 
            //     else { 
            //         printf("'%s' \n", G.nonTerminals[fc->elem->curr]); 
            //     } 
            //     fc = fc->next; 
            // } 

            // printf("%s \n", G.nonTerminals[fc->elem->curr]); 

            int t1 = typeCheck(findChild(root, 1, false, 0), G, sTable, index); 
            // printf("%s \n", G.nonTerminals[fc->elem->curr]); 
            // int t1 = typeCheck(fc, G, sTable, index);
            int t2 = -2;
            int line = root->child->next->elem->lineNo; 
            astNode *temp = root->child->next->next;
            while(!(temp->elem->isLeaf && temp->elem->curr == 0)){    // till otherStmts = eps is encountered
                if(typeCheck(temp,G,sTable,index)==-1)t2 = -1;
                temp = temp->next;
            }
            
            // printf("%d %d \n", t1, t2); 

            if(t1 != -3){
                printf("Condition for if is incorrect at line %d\n",line);
            }
            else if(t2 != -2){
                printf("Incorrect stmts inside if block %d\n", line);
            }
            else{ 
                // temp = findChild(root,5,false,0);
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
            
            int line = root->child->elem->lineNo; 
            
            // astNode* fc = root->child; 

            // while (fc != NULL) { 
            //     printf("%d %d ", fc->elem->isLeaf, fc->elem->curr); 
            //     if (fc->elem->isLeaf == 1) { 
            //         printf("'%s' \n", G.terminals[fc->elem->curr]); 
            //     } 
            //     else { 
            //         printf("'%s' \n", G.nonTerminals[fc->elem->curr]); 
            //     } 
            //     fc = fc->next; 
            // } 
            
            // astNode *temp = findChild(root,1,false,0);
            // If function is main
            if(strcmp(sTable->tables[index]->function->fId, "main") == 0){
                if(!(root->child->next->child->elem->isLeaf && root->child->next->child->elem->curr == 0)){
                    printf("Main function cannot return values at line %d\n", line);
                    return -1;
                }
                else{
                    printf("Main function return statement is valid \n"); 
                    return -2;
                }
            }
            // temp = temp->next; 
            printf("%s \n", G.nonTerminals[root->child->next->elem->curr]); 
            if (root->child->next->child->elem->isLeaf == 1 && root->child->next->child->elem->curr == 0) { 
                if (sTable->tables[index]->function->numOut == 0) { 
                    printf("Valid return statement at line %d \n", line); 
                    return -2; 
                } 
                else { 
                    printf("Return paramters not included at line %d ERROR\n", line); 
                    return -1; 
                }
            } 
            
            int t1 = matchReturnParams(root->child->next->child, G, sTable, index);
            if (t1!=-2){
                printf("Return parameters at line %d do not match with the function definition\n", line);
                return -1;
            }
            // printf("Function %s done \n", sTable->tables[index]->function->fId); 
            return -2;

        }
        else { 
            return -2; 
        }
    } 
    else { // Terminals 
        if (strcmp(G.terminals[root->elem->curr], "TK_NUM") == 0 || strcmp(G.terminals[root->elem->curr], "TK_RNUM") == 0 || strcmp(G.terminals[root->elem->curr], "TK_ID") == 0) { 
            return findNestedType(root, G, sTable, index); 
        } 
        else { 
            return -2; 
        } 
    }
} 

void main() { 
    
    // int numEntries = 0; 
    // int hashSize = 0; 
    // entry** hashT = initialiseS(); 
    // hashSize = size; 
    // entry* new1 = (entry*) malloc(sizeof(entry)); 
    // // printf("done \n"); 
    // // entry* new1 = makeEntry("c453", 0, 0, 1); 
    
    // new1->varName = "c453"; 
    // new1->offset = 4; 
    // entry* new2 = (entry*) malloc(sizeof(entry)); 
    // new2->varName = "kfjuw"; 
    // new2->offset = 8; 
    // printf("done \n"); 
    // initialiseS(hashT); 
    // printf("done \n"); 
    // int ins1 = insertSRez(new1, hashT, numEntries, hashSize); 
    // if (ins1 >= hashSize) { 
    //     printf("Insert 1 (Doubled): %d \n", ins1 - hashSize); 
    //     hashSize *= 2; 
    // } 
    // else { 
    //     printf("Insert 1 (Not doubled): %d \n", ins1); 
    // } 
    // int ins2 = insertSRez(new2, hashT, numEntries, hashSize); 
    // if (ins2 >= hashSize) { 
    //     printf("Insert 1 (Doubled): %d \n", ins2 - hashSize); 
    //     hashSize *= 2; 
    // } 
    // else { 
    //     printf("Insert 1 (Not doubled): %d \n", ins2); 
    // } 

    // printf("%d \n", insertS(new1, hashT)); 
    // printf("done \n"); 
    // printf("%d \n", insertS(new2, hashT)); 
    // int ind = searchS(new1->varName, hashT); 
    // printf("%d \n", ind); 
    // printf("%s %d %d %d %d %d \n", hashT[ind]->varName, hashT[ind]->offset, hashT[ind]->present, hashT[ind]->type, hashT[ind]->lineNo, hashT[ind]->width); 

    char* file; 
    file = "grammar.txt"; 
    grammar C; 
    C = readGrammar(file); 
    printf("Grammar read and saved \n"); 
    printf("%d %d %d %d \n", C.totalNumRules, C.allRules[36].numOrs, C.allRules[31].epsilon, C.allRules[28].epsilon); 
    
    // printf("'%s' %d %d \n", C.nonTerminals[23], C.allRules[23].numOrs, C.allRules[23].epsilon); 
    // printf("%d %d '%s' '%s' \n", C.allRules[23].RHS[0].symbols[0].symbol, C.allRules[23].RHS[1].symbols[0].symbol, C.nonTerminals[C.allRules[23].RHS[0].symbols[0].symbol], C.terminals[C.allRules[23].RHS[1].symbols[0].symbol]); 

    // prettyPrintGrammar(C);
    
    C.ff = ComputeFirstAndFollowSets(C); 
    printf("First and Follow computed \n"); 
    printf("\n***** \n"); 
    // int n = 48; 
    // printf("C.ff[n].numFirst %d  C.ff[n].numFollow %d \n", C.ff[n].numFirst[0], C.ff[n].numFollow); 
    // for (int i = 0; i < C.ff[n].numFollow; i++) { 
    //     printf("%d '%s' \n", C.ff[n].follow[i], C.terminals[C.ff[n].follow[i]]); 
    // } 
    
    printf("\n***** \n"); 

    // int trial = 10; 
    // // printf("%d %d %d %d %d %d \n", C.allRules[23].numOrs, C.ff[24].numFirst[0], C.ff[24].numFirst[1], C.ff[24].numFirst[2], C.ff[24].numFirst[3], C.ff[24].numFirst[4]); 
    // printf("C.allRules[trial].numOrs %d ", C.allRules[trial].numOrs); 
    // for (int i = 0; i < C.allRules[trial].numOrs; i++) { 
    //     printf("C.ff[trial].numFirst[i] %d ", C.ff[trial].numFirst[i]); 
    // } 

    // printRule(C,trial,-1);
    
    parseTable* T = intializeParseTable(C.numNonTerminals,C.numTerminals);
    createParseTable(C,C.ff,T);
    // printParseTable(C,T);
    printf("Parse Table created \n"); 

    // printf("%d %d %d '%s' '%s' \n", C.allRules[2].numOrs, C.ff[2].numFirst[0], C.ff[2].numFirst[1], C.terminals[C.ff[2].first[0][0]], C.terminals[C.ff[2].first[1][0]]); 
    // printf("%d %d %d \n", C.allRules[0].numOrs, C.allRules[0].RHS[0].numSyms, C.allRules[0].RHS[0].symbols[1].type); 
    // printf("%d %d %d %d %d %d '%s' '%s' '%s' '%s' \n", C.ff[23].numFirst, C.ff[23].numFollow, C.ff[23].follow[0], C.ff[23].follow[1], C.ff[23].follow[2], C.ff[23].follow[3], C.terminals[C.ff[23].follow[0]], C.terminals[C.ff[23].follow[1]], C.terminals[C.ff[23].follow[2]], C.terminals[C.ff[23].follow[3]]); 

    char* testCaseFile = "./testcases_stage1/t5.txt"; 
    // FILE *fp = fopen("./testcases_stage1/t2.txt","r"); 
    // initialize();
    // fp = getStream(fp, 0);
    // tokenInfo currToken = getNextToken(fp); 
    // printf("%d '%s' \n", findIndex(C.terminals, C.numTerminals, enumToStringP[currToken.tkn_name]), enumToStringP[currToken.tkn_name]); 
    treeN rootNode; 
    rootNode = parseInputSourceCode(testCaseFile, C, T); 
    // printf("%u %d %d %d %d %d \n", &rootNode, rootNode.elem.curr, rootNode.numChild, rootNode.elem.lineNo, rootNode.elem.isLeaf, rootNode.elem.parentNodeSymbolID); 
    // printf("%u %u \n", rootNode.children[0], rootNode.children[1]); 
    // printf("%u %u \n", rootNode.children[1]->children[1], rootNode.children[1]->children[1]->children[5]); 
    // printf("%d '%s' %d '%s' \n", rootNode.children[1]->children[1], C.nonTerminals[rootNode.children[1]->children[1]->elem.curr], rootNode.children[1]->children[1]->children[5], C.terminals[rootNode.children[1]->children[1]->children[5]->elem.curr]); 
    // printParseTree(&rootNode,"op.txt",C);
    int *insertPrev = (int *)malloc(sizeof(int));
    *insertPrev = 0;
    int *count = (int *)malloc(sizeof(int));
    *count = 0;
    astNode *astroot = mknode(rootNode.elem,C);
    constructAst(astroot, &rootNode,C,insertPrev,astroot);
    printf("*************************************************************************************************\n\n");
    printf("Printing Abstract Syntax Tree\n");
    printAST(astroot,C,count);
    printf("*************************************************************************************************\n\n");
    // printf("Level 1 printing\n");
    // printf("Root : isLeaf: %d curr: %d name: %s Line: %d \n",astroot->elem->isLeaf,astroot->elem->curr,astroot->elem->isLeaf?C.terminals[astroot->elem->curr]:C.nonTerminals[astroot->elem->curr],astroot->elem->lineNo);
    // astNode *curr = astroot->child;
    // while(curr!=NULL){
    //     printf("isLeaf: %d curr: %d name:%s Line:%d\n",curr->elem->isLeaf,curr->elem->curr,curr->elem->isLeaf?C.terminals[curr->elem->curr]:C.nonTerminals[curr->elem->curr],curr->elem->lineNo);
    //     curr = curr->next;
    // }
    printf("Root : %d %d %s \n", astroot->elem->isLeaf, astroot->elem->curr, C.nonTerminals[astroot->elem->curr]); 
    astNode* curr = astroot->child; 
    // astNode* curr = astroot->child->child; 
    // printf("%s \n", curr->elem->lex.lexemeStr); 
    while (curr != NULL) { 
        printf("%d %d ", curr->elem->isLeaf, curr->elem->curr); 
        if (curr->elem->isLeaf == 1) { 
            printf("'%s' \n", C.terminals[curr->elem->curr]); 
        } 
        else { 
            printf("'%s' \n", C.nonTerminals[curr->elem->curr]); 
        } 
        curr = curr->next; 
    } 

    // printf("%d \n", findIndex(C.terminals, C.numTerminals, "TK_GLOBAL")); 
    symbolTable* sTable = constructST(astroot, C); 

    // int ind9 = searchS("b5c6", sTable->tables[1]->entries); 
    // printf("%s %d Fields : %d \n", sTable->tables[1]->entries[ind9]->varName, sTable->tables[1]->entries[ind9]->type, sTable->allTypes[sTable->tables[1]->entries[ind9]->type]->numFields); 
    // int ind10 = searchS("maths", sTable->allTypes[sTable->tables[1]->entries[ind9]->type]->fields); 
    // int ind11 = searchS("physics", sTable->allTypes[sTable->tables[1]->entries[ind9]->type]->fields); 
    // int ind12 = searchS("chemistry", sTable->allTypes[sTable->tables[1]->entries[ind9]->type]->fields); 
    // printf("%s \n", sTable->allTypes[sTable->tables[1]->entries[ind9]->type]->fields[ind10]->varName); 
    // printf("%d \n", strcmp("maths", sTable->allTypes[sTable->tables[1]->entries[ind9]->type]->fields[ind10]->varName)); 
    // printf("%d %d %d \n", ind10, ind11, ind12); 
    // printf("%d %d \n", sTable->numTypes, sTable->numEntries); 
    // int ind1 = searchS("b3b444", sTable->entries); 
    // printf("%d %d %d %s \n", sTable->entries[ind1]->lineNo, sTable->entries[ind1]->offset, sTable->entries[ind1]->type, sTable->entries[ind1]->varName); 
    // printf("%d %d %d \n", sTable->tables[0]->currWidth, sTable->tables[1]->currWidth, sTable->mainOffset); 
    
    // printf("%d %d %d %d %d \n", sTable->numF, sTable->numTypes, sTable->tables[0]->currWidth, sTable->tables[0]->numEntries, sTable->tables[0]->function->inId); 
    // printf("%d %d \n", sTable->allTypes[0]->typeId, sTable->allTypes[1]->typeId); 
    // printf("%s %s \n", sTable->allTypes[2]->name, sTable->allTypes[3]->name); 
    // int ind1 = searchS("d5cc34", sTable->allTypes[4]->fields); 
    // int ind2 = searchS("b5c6", sTable->allTypes[4]->fields); 
    // printf("%d %d %d \n", sTable->allTypes[4]->numFields, sTable->allTypes[4]->fields[ind1]->type, sTable->allTypes[4]->fields[ind2]->type); 
    // int ind3 = searchTypes("#two", sTable); 
    // printf("%s \n", sTable->allTypes[ind3]->name); 
    // ind1 = searchS("b5b567", sTable->tables[0]->entries); 
    // ind2 = searchS("b3", sTable->tables[0]->entries); 
    // ind3 = searchS("d5", sTable->tables[0]->entries); 
    // printf("%d %d %d %d \n", sTable->tables[0]->numEntries, sTable->tables[0]->entries[ind1]->type, sTable->tables[0]->entries[ind2]->type, sTable->tables[0]->entries[ind3]->type); 
    // ind1 = searchS("beginpoint", sTable->allTypes[2]->fields); 
    // ind2 = searchS("endpoint", sTable->allTypes[2]->fields); 
    // printf("%d \n", sTable->allTypes[3]->width); 
    // printf("%s %d %s %d %d \n", sTable->allTypes[2]->name, sTable->allTypes[2]->numFields, sTable->allTypes[2]->fields[ind2]->varName, sTable->allTypes[2]->fields[ind2]->type, sTable->allTypes[2]->fields[ind2]->offset); 
    
    // int ind4 = searchTypes("#two", sTable); 
    // printf("%d %s \n", sTable->allTypes[ind4]->numAl, sTable->allTypes[ind4]->aliases[0]); 
    // int ind5 = searchTypes("#point", sTable); 
    // printf("%d %d \n", ind4, sTable->allTypes[ind5]->ref); 
    // int ind6 = searchTypes("#variantrecord", sTable); 
    // printf("%d %d \n", sTable->allTypes[ind6]->width, ind6); 
    // int ind7 = searchS("b5c6", sTable->allTypes[4]->fields); 
    // printf("%s %d \n", sTable->allTypes[4]->fields[ind7]->varName, sTable->allTypes[4]->fields[ind7]->type); 
    // ind7 = searchS("beginpoint", sTable->allTypes[ind4]->fields); 
    // printf("%s \n", sTable->allTypes[ind4]->fields[ind7]->varName); 
    // printf("Number of functions: %d \n", sTable->numF); 
    // int ind8 = searchS("b3b444", sTable->entries); 
    // printf("%s %d %d \n", sTable->entries[ind8]->varName, sTable->entries[ind8]->type, sTable->entries[ind8]->offset); 
    // printf("%d %d %d \n", sTable->tables[0]->function->inOrder[0], sTable->tables[0]->function->inOrder[1], sTable->tables[0]->function->inOrder[2]); 
    // printf("%s %s %s \n", sTable->allTypes[sTable->tables[0]->function->inId]->fields[sTable->tables[0]->function->inOrder[0]]->varName, sTable->allTypes[sTable->tables[0]->function->inId]->fields[sTable->tables[0]->function->inOrder[1]]->varName, sTable->allTypes[sTable->tables[0]->function->inId]->fields[sTable->tables[0]->function->inOrder[2]]->varName); 
    
    // printf("*************************************************************************************************\n\n"); 
    printf("Type Checking \n"); 
    int tc = typeCheck(astroot, C, sTable, -1); 
    // int ind9 = searchS("b5c6", sTable->tables[1]->entries); 
    // printf("%s %d Fields : %d \n", sTable->tables[1]->entries[ind9]->varName, sTable->tables[1]->entries[ind9]->type, sTable->allTypes[sTable->tables[1]->entries[ind9]->type]->numFields); 
    // int ind10 = searchS("maths", sTable->allTypes[sTable->tables[1]->entries[ind9]->type]->fields); 
    // printf("%s \n", sTable->allTypes[sTable->tables[1]->entries[ind9]->type]->fields[ind10]->varName); 
    // printf("%d \n", strcmp("maths", sTable->allTypes[sTable->tables[1]->entries[ind9]->type]->fields[ind10]->varName)); 
    // printf("First child of function : %d %d %s %s \n", astroot->child->child->child->elem->isLeaf, astroot->child->child->child->elem->curr, C.terminals[astroot->child->child->child->elem->curr], astroot->child->child->child->elem->lex.lexemeStr); 
    // printf("rule number %d LHS %s RHS %s\n",getRuleNumber(52,1,C),C.nonTerminals[C.allRules[52].LHS],C.terminals[C.allRules[52].RHS[1].symbols[0].symbol]);
}