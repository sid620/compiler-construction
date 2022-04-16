// #pragma once
#include <stdio.h> 
#include <stdbool.h> 
#include "string.h" 
#include "symbolTable.h" 
// #include "ast.c"

// #define HASH_SIZE 101 
int size = 101; 

/*
    Hashes string 
*/
int hashS(char* lexeme, int i){
    int x = h1(lexeme);
    return (x + i * h2(x)) % size;
} 

/*
    Search string in hash table, returns index j of entry in hashtable else -1 if not found 
*/
int searchS(char* lexeme, entry** hashT){
    int i = 0; 
    int j = hashS(lexeme, i);
    // printf("hash value of lexeme in searchS %d\n",j);
    // printf("%s %d \n", lexeme, j); 
    // for (int k = 0; k < size; k++) { 
    //     if (hashT[k]->present == 1) { 
    //         printf("%d is present \n", k); 
    //     }
    // }
    while(hashT[j]->present == 1 && i < size) { 
        // printf("Comparing %d \n", strcmp(lexeme, hashT[j]->varName)); 
        if(strcmp(lexeme, hashT[j]->varName) == 0) return j; 
        j = hashS(lexeme, i); 
        // printf("In while %s %d %d \n", lexeme, j, hashT[j]->present); 
        i++;
    }
    return -1;
} 

/*
    Inserts the entry created into the hash table    
*/
int insertS(entry* new, entry** hashT){
    int i = 0;
    int j;
    char* lexeme = new->varName; 
    // printf("doneI \n"); 
    while(i < size){
        j = hashS(lexeme, i);
        // printf("doneI %d %d %d \n", j, j < size, hashT[j]->present); 
        if(hashT[j]->present == 0){
            // printf("in if \n"); 
            int l = strlen(lexeme); 
            // printf("%d \n", l); 
            hashT[j]->varName = (char*) malloc(sizeof(char) * (l + 1)); 
            for(int i = 0; i < l; i++){
                hashT[j]->varName[i] = lexeme[i];
            } 
            hashT[j]->varName[l] = '\0'; 
            // printf("doneI \n"); 
            hashT[j]->present = 1; 
            hashT[j]->lineNo = new->lineNo; 
            hashT[j]->offset = new->offset; 
            hashT[j]->type = new->type; 
            // hashT[j]->width = new->width; 
            return j;
        }
        else if(hashT[j]->present && hashT[j]->lineNo == new->lineNo){
            (hashT[j]->count)++;
        }
        i++;
    }
} 

// int insertSRez(entry* new, entry** hashT, int numEntries, int hashSize) { 
//     if (numEntries == hashSize) { 
//         hashSize *= 2; 
//         entry** newT = (entry**) malloc(sizeof(entry*) * hashSize); 
//         entry* move = (entry*) malloc(sizeof(entry)); 
//         for (int i = 0; i < numEntries; i++) { 
//             move = hashT[i]; 
//             insertS(move, newT); 
//         } 
//         hashT = newT; 
//         return (hashSize / 2) + insertS(new, hashT); 
//     } 
//     return insertS(new, hashT); 
// }

/*
    Initializes a hash table and allocates memory    
*/
entry** initialiseS() { 
    entry** hashT = (entry**) malloc(sizeof(entry*) * size); 
    for (int i = 0; i < size; i++) { 
        hashT[i] = (entry*) malloc(sizeof(entry) * 1); 
        hashT[i]->present = 0; 
        // printf("%d \n", hashT[i]->present); 
    } 
    return hashT; 
} 

/*
    Creates a new entry of the variable to be added to the function's hash table
*/
entry* makeEntry(char* lexeme, int type, int offset, int lineNo) { 

    entry* new = (entry*) malloc(sizeof(entry)); 
    new->lineNo = lineNo; 
    new->offset = offset; 
    new->type = type; 
    new->count = 0;
    int l = strlen(lexeme); 
    new->varName = (char*) malloc(sizeof(char) * (l + 1)); 
    for(int i = 0; i < l; i++){
        new->varName[i] = lexeme[i];
    } 
    new->varName[l] = '\0'; 
    // printf("In makeEntry %s \n", new->varName); 
    return new; 
} 

/*
    Insert a variable of new type expression to the symbol table
*/
void insertType(typeInfo* new, symbolTable* sTable) { 
    new->numFields = 0;
    sTable->allTypes= realloc(sTable->allTypes, sizeof(typeInfo*) * (sTable->numTypes + 1)); 
    sTable->allTypes[sTable->numTypes] = new; 
    sTable->numTypes += 1; 
} 

/*
    Prints the type expression of the variable
*/
void printType(char* typeName, symbolTable* sTable) { 
    int tInd = searchTypes(typeName, sTable); 
    if (sTable->allTypes[tInd]->numFields == 0) { 
        printf("%s", sTable->allTypes[tInd]->name); 
        return; 
    } 
    printf("<"); 
    for (int i = 0; i < sTable->allTypes[tInd]->numFields; i++) { 
        char* newType = sTable->allTypes[sTable->allTypes[tInd]->fields[sTable->allTypes[tInd]->fOrder[i]]->type]->name; 
        printType(newType, sTable); 
        if (i != sTable->allTypes[tInd]->numFields - 1) { 
            printf(", "); 
        } 
    } 
    printf(">"); 
    return; 
}

/*
    Prints all variables in the symbol table    
*/

/*
    Prints Symbol Table
*/
void printSymbolTable( symbolTable* sTable){
    printAllVars(sTable);
}

/*
//     Returns function name at given line number
// */

// /*
//     Prints names of all global variables along with their type and aliases
// */
char* getGlobalFName(int lineNo, symbolTable* sTable) { 
    for (int i = 0; i < sTable->numF; i++) { 
        if (lineNo < sTable->tables[i]->function->lastLine) { 
            return sTable->tables[i]->function->fId; 
        }
    } 
    return "Not found"; 
}


void printGlobalVars(symbolTable* sTable) { 

    if (sTable->numEntries == 0) { 
        printf("No global variables found");
        return; 
    }
    for (int i = 0; i < size; i++) { 
        if (sTable->entries[i]->present == 1) { 
            int vInd = i; 
            char* fName = getGlobalFName(sTable->entries[i]->lineNo, sTable); 
            printf("Variable Name: %s; ", sTable->entries[vInd]->varName); 
            int tInd = sTable->entries[vInd]->type; 
            char* tName = sTable->allTypes[tInd]->name; 
            printf("Type Name(s): %s", tName); 
            tInd = searchTypes(tName, sTable); 
            if (strcmp(tName, sTable->allTypes[tInd]->name) != 0) { 
                printf(" %s", sTable->allTypes[tInd]->name); 
            }
            if (sTable->allTypes[tInd]->numAl != 0) { 
                printf(" "); 
                for (int j = 0; j < sTable->allTypes[tInd]->numAl; j++) { 
                    if (strcmp(sTable->allTypes[tInd]->aliases[j], tName) != 0) { 
                        if (j != sTable->allTypes[tInd]->numAl - 1) { 
                            printf("%s ", sTable->allTypes[tInd]->aliases[j]); 
                        } 
                        else { 
                            printf("%s", sTable->allTypes[tInd]->aliases[j]); 
                        }
                    }
                }
            } 
            printf("; Type Expression: "); 
            printType(tName, sTable); 
            printf("; "); 
            printf("Offset: %d; \n", sTable->entries[vInd]->offset); 
        }
    }
} 

void printGlobals(symbolTable* sTable) { 
    printf("Global Variables\n");
    if (sTable->numEntries == 0) { 
        printf("No global variables found");
        return; 
    }
    for (int i = 0; i < size; i++) { 
        if (sTable->entries[i]->present == 1) { 
            int vInd = i; 
            char* fName = getGlobalFName(sTable->entries[i]->lineNo, sTable); 
            printf("Variable Name: %s; Module (Scope): %s; ", sTable->entries[vInd]->varName, fName); 
            int tInd = sTable->entries[vInd]->type; 
            char* tName = sTable->allTypes[tInd]->name; 
            printf("Type Name(s): %s", tName); 
            tInd = searchTypes(tName, sTable); 
            if (strcmp(tName, sTable->allTypes[tInd]->name) != 0) { 
                printf(" %s", sTable->allTypes[tInd]->name); 
            }
            if (sTable->allTypes[tInd]->numAl != 0) { 
                printf(" "); 
                for (int j = 0; j < sTable->allTypes[tInd]->numAl; j++) { 
                    if (strcmp(sTable->allTypes[tInd]->aliases[j], tName) != 0) { 
                        if (j != sTable->allTypes[tInd]->numAl - 1) { 
                            printf("%s ", sTable->allTypes[tInd]->aliases[j]); 
                        } 
                        else { 
                            printf("%s", sTable->allTypes[tInd]->aliases[j]); 
                        }
                    }
                }
            } 
            printf("; Type Expression: "); 
            printType(tName, sTable); 
            printf("; "); 
            printf("Width: %d; isGlobal: global; Offset: %d; Variable Usage: --- \n", sTable->allTypes[tInd]->width, sTable->entries[vInd]->offset); 
        }
    }
} 

void printAllVars(symbolTable* sTable) { 

    for (int i = 0; i < sTable->numF; i++) { 
        char* fName = sTable->tables[i]->function->fId; 
        printf("In function %s \n", fName); 
        
        int inId = sTable->tables[i]->function->inId; 
        int outId = sTable->tables[i]->function->outId; 

        if (inId != -1) { 
            for (int j = 0; j < sTable->tables[i]->function->numIn; j++) { 
                int vInd = sTable->tables[i]->function->inOrder[j]; 
                printf("Variable Name: %s; Module (Scope): %s; ", sTable->allTypes[inId]->fields[vInd]->varName, fName); 
                int tInd = sTable->allTypes[inId]->fields[vInd]->type; 
                char* tName = sTable->allTypes[tInd]->name; 
                printf("Type Name(s): %s", tName); 
                tInd = searchTypes(tName, sTable); 
                if (strcmp(tName, sTable->allTypes[tInd]->name) != 0) { 
                    printf(" %s", sTable->allTypes[tInd]->name); 
                }
                if (sTable->allTypes[tInd]->numAl != 0) { 
                    printf(" "); 
                    for (int k = 0; k < sTable->allTypes[tInd]->numAl; k++) { 
                        if (strcmp(sTable->allTypes[tInd]->aliases[k], tName) != 0) { 
                            if (k != sTable->allTypes[tInd]->numAl - 1) { 
                                printf("%s ", sTable->allTypes[tInd]->aliases[k]); 
                            } 
                            else { 
                                printf("%s", sTable->allTypes[tInd]->aliases[k]); 
                            }
                        }
                    }
                } 
                printf("; Type Expression: "); 
                printType(tName, sTable); 
                printf("; "); 
                printf("Width: %d; isGlobal: ---; Offset: %d; Variable Usage: INPUT PARAMETER \n", sTable->allTypes[tInd]->width, sTable->allTypes[inId]->fields[vInd]->offset); 
            } 
        } 

        if (outId != -1) { 
            for (int j = 0; j < sTable->tables[i]->function->numOut; j++) { 
                int vInd = sTable->tables[i]->function->outOrder[j]; 
                printf("Variable Name: %s; Module (Scope): %s; ", sTable->allTypes[outId]->fields[vInd]->varName, fName); 
                int tInd = sTable->allTypes[outId]->fields[vInd]->type; 
                char* tName = sTable->allTypes[tInd]->name; 
                printf("Type Name(s): %s", tName); 
                tInd = searchTypes(tName, sTable); 
                if (strcmp(tName, sTable->allTypes[tInd]->name) != 0) { 
                    printf(" %s", sTable->allTypes[tInd]->name); 
                }
                if (sTable->allTypes[tInd]->numAl != 0) { 
                    printf(" "); 
                    for (int k = 0; k < sTable->allTypes[tInd]->numAl; k++) { 
                        if (strcmp(sTable->allTypes[tInd]->aliases[k], tName) != 0) { 
                            if (k != sTable->allTypes[tInd]->numAl - 1) { 
                                printf("%s ", sTable->allTypes[tInd]->aliases[k]); 
                            } 
                            else { 
                                printf("%s", sTable->allTypes[tInd]->aliases[k]); 
                            }
                        }
                    }
                } 
                printf("; Type Expression: "); 
                printType(tName, sTable); 
                printf("; "); 
                printf("Width: %d; isGlobal: ---; Offset: %d; Variable Usage: OUTPUT PARAMETER \n", sTable->allTypes[tInd]->width, sTable->allTypes[outId]->fields[vInd]->offset); 
            } 
        } 

        if (sTable->tables[i]->numEntries != 0) { 
            for (int j = 0; j < size; j++) { 
                if (sTable->tables[i]->entries[j]->present == 1) { 
                    int vInd = j; 
                    printf("Variable Name: %s; Module (Scope): %s; ", sTable->tables[i]->entries[vInd]->varName, fName); 
                    int tInd = sTable->tables[i]->entries[vInd]->type; 
                    char* tName = sTable->allTypes[tInd]->name; 
                    printf("Type Name(s): %s", tName); 
                    tInd = searchTypes(tName, sTable); 
                    if (strcmp(tName, sTable->allTypes[tInd]->name) != 0) { 
                        printf(" %s", sTable->allTypes[tInd]->name); 
                    }
                    if (sTable->allTypes[tInd]->numAl != 0) { 
                        printf(" "); 
                        for (int k = 0; k < sTable->allTypes[tInd]->numAl; k++) { 
                            if (strcmp(sTable->allTypes[tInd]->aliases[k], tName) != 0) { 
                                if (k != sTable->allTypes[tInd]->numAl - 1) { 
                                    printf("%s ", sTable->allTypes[tInd]->aliases[k]); 
                                } 
                                else { 
                                    printf("%s", sTable->allTypes[tInd]->aliases[k]); 
                                }
                            }
                        }
                    } 
                    printf("; Type Expression: "); 
                    printType(tName, sTable); 
                    printf("; "); 
                    printf("Width: %d; isGlobal: ---; Offset: %d; Variable Usage: LOCAL \n", sTable->allTypes[tInd]->width, sTable->tables[i]->entries[vInd]->offset); 
                } 
            } 
        } 
    } 
    printGlobals(sTable); 
} 

/*
    Prints the Action Record sizes
*/
void printARSizes(symbolTable* sTable) { 
    for (int i = 0; i < sTable->numF; i++) { 
        printf("%s %d \n", sTable->tables[i]->function->fId, sTable->tables[i]->currWidth); 
    }
} 

/*
    Returns the index of function with fName if present in the sTable, else returns -1 
*/
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

/*
    Prints record and union details
*/
void printRUInfo(symbolTable* sTable) { 
    for (int i = 2; i < sTable->numTypes; i++) { 
        if (sTable->allTypes[i]->isParam == 1) { 
            continue; 
        }
        int index = i; 
        printf("%s ", sTable->allTypes[i]->name); 
        if (sTable->allTypes[i]->ref != -1) { 
            index = sTable->allTypes[i]->ref; 
        } 
        printType(sTable->allTypes[index]->name, sTable); 
        printf(" %d \n", sTable->allTypes[index]->width); 
    }
}

/*
    Search for the variable name in symbol table, returns index i if found else -1 if not found 
*/
int searchTypes(char* name, symbolTable* sTable) { 
    // printf("%d \n", sTable->numTypes); 
    for (int i = 0; i < sTable->numTypes; i++) { 
        // printf("%s %s %d \n", sTable->allTypes[i]->name, name, strcmp(sTable->allTypes[i]->name, name)); 
        if (strcmp(sTable->allTypes[i]->name, name) == 0) { 
            if (sTable->allTypes[i]->ref != -1) { 
                return sTable->allTypes[i]->ref; 
            }
            return i; 
        } 
        else { 
            for (int j = 0; j < sTable->allTypes[i]->numAl; j++) { 
                if (strcmp(sTable->allTypes[i]->aliases[j], name) == 0) { 
                    return i; 
                }
            }
        }
    } 
    return -1; 
} 

/*
    Updates the width of symbol table as needed
*/
void updateWidth(int ind, symbolTable* sTable, int width) { 

    // printf("In updateWidth: %s %d \n", sTable->allTypes[ind]->name, width); 
    for (int i = 0; i < sTable->numTypes; i++) { 
        if (sTable->allTypes[i]->numFields != 0) { 
            if (sTable->allTypes[i]->typeId == 3) { 
                for (int j = 0; j < size; j++) { 
                    if (sTable->allTypes[i]->fields[j]->present == 1 && (sTable->allTypes[i]->fields[j]->type == ind || sTable->allTypes[sTable->allTypes[i]->fields[j]->type]->ref == ind)) { 
                        // printf("Updating width of record: %s \n", sTable->allTypes[i]->name); 
                        sTable->allTypes[i]->width += width; 
                    }
                }
            } 
            else if (sTable->allTypes[i]->typeId == 4) { 
                for (int j = 0; j < size; j++) { 
                    if (sTable->allTypes[i]->fields[j]->present == 1 && (sTable->allTypes[i]->fields[j]->type == ind || sTable->allTypes[sTable->allTypes[i]->fields[j]->type]->ref == ind)) { 
                        if (width > sTable->allTypes[i]->width) { 
                            // printf("Updating width of union: %s \n", sTable->allTypes[i]->name); 
                            sTable->allTypes[i]->width = width; 
                            break; 
                        }
                    }
                }
            }
        }
    }
} 

/*
    Function which recursively traverses the subtree of Non Terminals inside the <function> subtree
    and populates the symbol table appropriately
*/
void handleNT(astNode* root, grammar G, symbolTable* sTable, int index, int recInd) { 

    // if (strcmp(G.nonTerminals[root->elem->curr], "input_par") == 0) { 
    //     // printf("input par \n"); 
    //     int count = 0; 
    //     astNode* child = root->child; 
        
    //     typeInfo* in = (typeInfo*) malloc(sizeof(typeInfo)); 
    //     in->isParam = 1; 
    //     in->isPrim = 0; 
    //     in->numFields = 0; 
    //     in->name = "inPar"; 
    //     in->ref = -1; 
    //     in->numAl = 0; 
    //     // in->fInd = index; 

    //     while(child != NULL) { 
    //         if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_INT") == 0) { 
    //             count += 1; 
    //             child = child->next; 
    //             if (in->numFields == 0) { 
    //                 in->fields = initialiseS(); 
    //             } 
    //             in->numFields += 1; 

    //             if (count == 1) { 
    //                 sTable->tables[index]->function->inOrder = (int*) malloc(sizeof(int)); 
    //             } 
    //             else { 
    //                 sTable->tables[index]->function->inOrder = realloc(sTable->tables[index]->function->inOrder, (sizeof(int) * count)); 
    //             }

    //             entry* new = makeEntry(child->elem->lex.lexemeStr, 0, sTable->tables[index]->currWidth, child->elem->lineNo); 
    //             int ins = insertS(new, in->fields); 
    //             sTable->tables[index]->function->inOrder[count - 1] = ins; 
    //         } 
    //         else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_REAL") == 0) { 
    //             count += 1; 
    //             child = child->next; 
    //             if (in->numFields == 0) { 
    //                 in->fields = initialiseS(); 
    //             } 
    //             in->numFields += 1; 

    //             if (count == 1) { 
    //                 sTable->tables[index]->function->inOrder = (int*) malloc(sizeof(int)); 
    //             } 
    //             else { 
    //                 sTable->tables[index]->function->inOrder = realloc(sTable->tables[index]->function->inOrder, (sizeof(int) * count)); 
    //             } 

    //             entry* new = makeEntry(child->elem->lex.lexemeStr, 1, sTable->tables[index]->currWidth, child->elem->lineNo); 
    //             int ins = insertS(new, in->fields); 
    //             // sTable->tables[index]->currWidth += new->width; 
    //             sTable->tables[index]->function->inOrder[count - 1] = ins;  
    //         } 
    //         else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_RECORD") == 0) { 
    //             count += 1; 
    //             child = child->next; // Now it has RUID
                
    //             typeInfo* newT = (typeInfo*) malloc(sizeof(typeInfo)); 
    //             int l = strlen(child->elem->lex.lexemeStr); 
    //             newT->name = (char*) malloc(sizeof(char) * (l + 1)); 
    //             for(int i = 0; i < l; i++){
    //                 newT->name[i] = child->elem->lex.lexemeStr[i];
    //             } 
    //             newT->name[l] = '\0'; 
    //             newT->typeId = 3; 
    //             newT->ref = -1; 
    //             newT->numFields = 0; 
    //             newT->numAl = 0; 
    //             // newT->fInd = -1; 
    //             insertType(newT, sTable); 
    //             child = child->next; 

    //             if (in->numFields == 0) { 
    //                 in->fields = initialiseS(); 
    //             } 
    //             in->numFields += 1; 

    //             if (count == 1) { 
    //                 sTable->tables[index]->function->inOrder = (int*) malloc(sizeof(int)); 
    //             } 
    //             else { 
    //                 sTable->tables[index]->function->inOrder = realloc(sTable->tables[index]->function->inOrder, (sizeof(int) * count)); 
    //             } 

    //             entry* new = makeEntry(child->elem->lex.lexemeStr, sTable->numTypes - 1, sTable->tables[index]->currWidth, child->elem->lineNo); 
    //             int ins = insertS(new, in->fields); 
    //             sTable->tables[index]->function->inOrder[count - 1] = ins; 
    //         } 
    //         else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_UNION") == 0) { 
    //             count += 1; 
    //             child = child->next; 
                
    //             typeInfo* newT = (typeInfo*) malloc(sizeof(typeInfo)); 
    //             int l = strlen(child->elem->lex.lexemeStr); 
    //             newT->name = (char*) malloc(sizeof(char) * (l + 1)); 
    //             for(int i = 0; i < l; i++){
    //                 newT->name[i] = child->elem->lex.lexemeStr[i];
    //             } 
    //             newT->name[l] = '\0'; 
    //             newT->typeId = 4; 
    //             newT->ref = -1; 
    //             newT->numFields = 0; 
    //             newT->numAl = 0; 
    //             // newT->fInd = -1; 
    //             insertType(newT, sTable); 
    //             child = child->next; 

    //             if (in->numFields == 0) { 
    //                 in->fields = initialiseS(); 
    //             } 
    //             in->numFields += 1; 

    //             if (count == 1) { 
    //                 sTable->tables[index]->function->inOrder = (int*) malloc(sizeof(int)); 
    //             } 
    //             else { 
    //                 sTable->tables[index]->function->inOrder = realloc(sTable->tables[index]->function->inOrder, (sizeof(int) * count)); 
    //             } 

    //             entry* new = makeEntry(child->elem->lex.lexemeStr, sTable->numTypes - 1, sTable->tables[index]->currWidth, child->elem->lineNo); 
    //             int ins = insertS(new, in->fields); 
    //             sTable->tables[index]->function->inOrder[count - 1] = ins; 
    //         } 
    //         else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_RUID") == 0) { 
    //             count += 1; 
    //             // child = child->next; 
                
    //             typeInfo* newT = (typeInfo*) malloc(sizeof(typeInfo)); 
    //             int l = strlen(child->elem->lex.lexemeStr); 
    //             newT->name = (char*) malloc(sizeof(char) * (l + 1)); 
    //             for(int i = 0; i < l; i++){
    //                 newT->name[i] = child->elem->lex.lexemeStr[i];
    //             } 
    //             newT->name[l] = '\0'; 
    //             newT->ref = -1; 
    //             newT->numFields = 0; 
    //             newT->numAl = 0; 
    //             // newT->fInd = -1; 
    //             // newT->typeId; // TypeId not known, could be union or record 
    //             insertType(newT, sTable); 
    //             child = child->next; 

    //             if (in->numFields == 0) { 
    //                 in->fields = initialiseS(); 
    //             } 
    //             in->numFields += 1; 

    //             if (count == 1) { 
    //                 sTable->tables[index]->function->inOrder = (int*) malloc(sizeof(int)); 
    //             } 
    //             else { 
    //                 sTable->tables[index]->function->inOrder = realloc(sTable->tables[index]->function->inOrder, (sizeof(int) * count)); 
    //             } 

    //             entry* new = makeEntry(child->elem->lex.lexemeStr, sTable->numTypes - 1, sTable->tables[index]->currWidth, child->elem->lineNo); 
    //             int ins = insertS(new, in->fields); 
    //             sTable->tables[index]->function->inOrder[count - 1] = ins; 
    //         }
    //         else if (child->elem->isLeaf == 1 && child->elem->curr == 0) { 
    //             // skip 
    //             // printf("Epsilon encountered in \n"); 
    //         } // Record!!! 
    //         else { 
    //             // printf("Handling %s \n", G.nonTerminals[child->elem->curr]); 
    //             handleNT(child, G, sTable, index, 0); 
    //         } 
    //         child = child->next; 
    //     } 
    //     sTable->tables[index]->function->numIn = count; 
    //     insertType(in, sTable); 
    //     sTable->tables[index]->function->inId = sTable->numTypes - 1; 
    // } 
    if (strcmp(G.nonTerminals[root->elem->curr], "input_par") == 0) { 
        // printf("input par \n"); 
        int count = 0; 
        astNode* child = root->child; 
        
        typeInfo* in = (typeInfo*) malloc(sizeof(typeInfo)); 
        in->isParam = 1; 
        in->isPrim = 0; 
        in->numFields = 0; 
        in->name = "inPar"; 
        in->ref = -1; 
        in->numAl = 0; 
        // in->fInd = index; 

        while(child != NULL) { 
            if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_INT") == 0) { 
                count += 1; 
                child = child->next; 
                if (in->numFields == 0) { 
                    in->fields = initialiseS(); 
                } 
                in->numFields += 1; 

                if (count == 1) { 
                    sTable->tables[index]->function->inOrder = (int*) malloc(sizeof(int)); 
                } 
                else { 
                    sTable->tables[index]->function->inOrder = realloc(sTable->tables[index]->function->inOrder, (sizeof(int) * count)); 
                }

                entry* new = makeEntry(child->elem->lex.lexemeStr, 0, sTable->tables[index]->currWidth, child->elem->lineNo); 
                int ins = insertS(new, in->fields); 
                // sTable->tables[index]->currWidth += new->width; 
                // sTable->tables[index]->currWidth += sTable->allTypes[0]->width; 
                sTable->tables[index]->function->inOrder[count - 1] = ins; 
            } 
            else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_REAL") == 0) { 
                count += 1; 
                child = child->next; 
                if (in->numFields == 0) { 
                    in->fields = initialiseS(); 
                } 
                in->numFields += 1; 

                if (count == 1) { 
                    sTable->tables[index]->function->inOrder = (int*) malloc(sizeof(int)); 
                } 
                else { 
                    sTable->tables[index]->function->inOrder = realloc(sTable->tables[index]->function->inOrder, (sizeof(int) * count)); 
                } 

                entry* new = makeEntry(child->elem->lex.lexemeStr, 1, sTable->tables[index]->currWidth, child->elem->lineNo); 
                int ins = insertS(new, in->fields); 
                // sTable->tables[index]->currWidth += new->width; 
                // sTable->tables[index]->currWidth += sTable->allTypes[1]->width; 
                sTable->tables[index]->function->inOrder[count - 1] = ins;  
            } 
            else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_RECORD") == 0) { 
                count += 1; 
                child = child->next; 
                
                int ind = searchTypes(child->elem->lex.lexemeStr, sTable); 
                if (ind == -1) { 
                    typeInfo* newT = (typeInfo*) malloc(sizeof(typeInfo)); 
                    int l = strlen(child->elem->lex.lexemeStr); 
                    newT->name = (char*) malloc(sizeof(char) * (l + 1)); 
                    for(int i = 0; i < l; i++){
                        newT->name[i] = child->elem->lex.lexemeStr[i];
                    } 
                    newT->name[l] = '\0'; 
                    newT->typeId = 3; 
                    newT->ref = -1; 
                    newT->numFields = 0; 
                    newT->numAl = 0; 
                    // newT->fInd = -1; 
                    insertType(newT, sTable); 
                    ind = sTable->numTypes - 1; 
                } 

                child = child->next; 

                if (in->numFields == 0) { 
                    in->fields = initialiseS(); 
                } 
                in->numFields += 1; 

                if (count == 1) { 
                    sTable->tables[index]->function->inOrder = (int*) malloc(sizeof(int)); 
                } 
                else { 
                    sTable->tables[index]->function->inOrder = realloc(sTable->tables[index]->function->inOrder, (sizeof(int) * count)); 
                } 

                entry* new = makeEntry(child->elem->lex.lexemeStr, ind, sTable->tables[index]->currWidth, child->elem->lineNo); 
                int ins = insertS(new, in->fields); 
                // sTable->tables[index]->currWidth += sTable->allTypes[sTable->numTypes - 1]->width; 
                sTable->tables[index]->function->inOrder[count - 1] = ins; 
            } 
            else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_UNION") == 0) { 
                count += 1; 
                child = child->next; 
                
                int ind = searchTypes(child->elem->lex.lexemeStr, sTable); 
                if (ind == -1) { 
                    typeInfo* newT = (typeInfo*) malloc(sizeof(typeInfo)); 
                    int l = strlen(child->elem->lex.lexemeStr); 
                    newT->name = (char*) malloc(sizeof(char) * (l + 1)); 
                    for(int i = 0; i < l; i++){
                        newT->name[i] = child->elem->lex.lexemeStr[i];
                    } 
                    newT->name[l] = '\0'; 
                    newT->typeId = 4; 
                    newT->ref = -1; 
                    newT->numFields = 0; 
                    newT->numAl = 0; 
                    // newT->fInd = -1; 
                    insertType(newT, sTable); 
                    ind = sTable->numTypes - 1; 
                } 

                child = child->next; 

                if (in->numFields == 0) { 
                    in->fields = initialiseS(); 
                } 
                in->numFields += 1; 

                if (count == 1) { 
                    sTable->tables[index]->function->inOrder = (int*) malloc(sizeof(int)); 
                } 
                else { 
                    sTable->tables[index]->function->inOrder = realloc(sTable->tables[index]->function->inOrder, (sizeof(int) * count)); 
                } 

                entry* new = makeEntry(child->elem->lex.lexemeStr, ind, sTable->tables[index]->currWidth, child->elem->lineNo); 
                int ins = insertS(new, in->fields); 
                // sTable->tables[index]->currWidth += new->width; 
                // sTable->tables[index]->currWidth += sTable->allTypes[sTable->numTypes - 1]->width; 
                sTable->tables[index]->function->inOrder[count - 1] = ins; 
            } 
            else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_RUID") == 0) { 
                count += 1; 
                // child = child->next; 
                
                int ind = searchTypes(child->elem->lex.lexemeStr, sTable); 
                if (ind == -1) { 
                    typeInfo* newT = (typeInfo*) malloc(sizeof(typeInfo)); 
                    int l = strlen(child->elem->lex.lexemeStr); 
                    newT->name = (char*) malloc(sizeof(char) * (l + 1)); 
                    for(int i = 0; i < l; i++){
                        newT->name[i] = child->elem->lex.lexemeStr[i];
                    } 
                    newT->name[l] = '\0'; 
                    newT->ref = -1; 
                    newT->numFields = 0; 
                    newT->numAl = 0; 
                    // newT->fInd = -1; 
                    // newT->typeId; // TypeId not known, could be union or record 
                    insertType(newT, sTable); 
                    ind = sTable->numTypes - 1; 
                } 

                child = child->next; 

                if (in->numFields == 0) { 
                    in->fields = initialiseS(); 
                } 
                in->numFields += 1; 

                if (count == 1) { 
                    sTable->tables[index]->function->inOrder = (int*) malloc(sizeof(int)); 
                } 
                else { 
                    sTable->tables[index]->function->inOrder = realloc(sTable->tables[index]->function->inOrder, (sizeof(int) * count)); 
                } 

                entry* new = makeEntry(child->elem->lex.lexemeStr, ind, sTable->tables[index]->currWidth, child->elem->lineNo); 
                int ins = insertS(new, in->fields); 
                // sTable->tables[index]->currWidth += sTable->allTypes[sTable->numTypes - 1]->width; 
                sTable->tables[index]->function->inOrder[count - 1] = ins; 
            }
            else if (child->elem->isLeaf == 1 && child->elem->curr == 0) { 
                // skip 
                // printf("Epsilon encountered in \n"); 
            } // Record!!! 
            else { 
                // printf("Handling %s \n", G.nonTerminals[child->elem->curr]); 
                handleNT(child, G, sTable, index, 0); 
            } 
            child = child->next; 
        } 
        sTable->tables[index]->function->numIn = count; 
        insertType(in, sTable); 
        sTable->tables[index]->function->inId = sTable->numTypes - 1; 
    } 
    // else if (strcmp(G.nonTerminals[root->elem->curr], "output_par") == 0) { 
    //     // printf("output par \n"); 
    //     int count = 0; 
    //     astNode* child = root->child; 
        
    //     typeInfo* out = (typeInfo*) malloc(sizeof(typeInfo)); 
    //     out->isParam = 1; 
    //     out->isPrim = 0; 
    //     out->numFields = 0; 
    //     out->name = "outPar"; 
    //     out->ref = -1; 
    //     out->numAl = 0; 
    //     // out->fInd = index; 

    //     while(child != NULL) { 
    //         if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_INT") == 0) { 
    //             count += 1; 
    //             child = child->next; 
    //             if (out->numFields == 0) { 
    //                 out->fields = initialiseS(); 
    //             } 
    //             out->numFields += 1; 

    //             if (count == 1) { 
    //                 sTable->tables[index]->function->outOrder = (int*) malloc(sizeof(int)); 
    //             } 
    //             else { 
    //                 sTable->tables[index]->function->outOrder = realloc(sTable->tables[index]->function->outOrder, (sizeof(int) * count)); 
    //             } 

    //             entry* new = makeEntry(child->elem->lex.lexemeStr, 0, sTable->tables[index]->currWidth, child->elem->lineNo); 
    //             int ins = insertS(new, out->fields); 
    //             sTable->tables[index]->function->outOrder[count - 1] = ins; 
    //         } 
    //         else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_REAL") == 0) { 
    //             count += 1; 
    //             child = child->next; 
    //             if (out->numFields == 0) { 
    //                 out->fields = initialiseS(); 
    //             } 
    //             out->numFields += 1; 

    //             if (count == 1) { 
    //                 sTable->tables[index]->function->outOrder = (int*) malloc(sizeof(int)); 
    //             } 
    //             else { 
    //                 sTable->tables[index]->function->outOrder = realloc(sTable->tables[index]->function->outOrder, (sizeof(int) * count)); 
    //             } 

    //             entry* new = makeEntry(child->elem->lex.lexemeStr, 1, sTable->tables[index]->currWidth, child->elem->lineNo); 
    //             int ins = insertS(new, out->fields); 
    //             sTable->tables[index]->function->outOrder[count - 1] = ins; 
    //             // sTable->tables[index]->currWidth += new->width; 
    //             // printf("'%s' %d \n", new->varName, sTable->tables[index]->currWidth); 
    //         } 
    //         else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_RECORD") == 0) { 
    //             count += 1; 
    //             child = child->next; 
                
    //             typeInfo* newT = (typeInfo*) malloc(sizeof(typeInfo)); 
    //             int l = strlen(child->elem->lex.lexemeStr); 
    //             newT->name = (char*) malloc(sizeof(char) * (l + 1)); 
    //             for(int i = 0; i < l; i++){
    //                 newT->name[i] = child->elem->lex.lexemeStr[i];
    //             } 
    //             newT->name[l] = '\0'; 
    //             newT->typeId = 3; 
    //             newT->ref = -1; 
    //             newT->numFields = 0; 
    //             newT->numAl = 0; 
    //             // newT->fInd = -1; 
    //             insertType(newT, sTable); 
    //             child = child->next; 

    //             if (out->numFields == 0) { 
    //                 out->fields = initialiseS(); 
    //             } 
    //             out->numFields += 1; 

    //             if (count == 1) { 
    //                 sTable->tables[index]->function->outOrder = (int*) malloc(sizeof(int)); 
    //             } 
    //             else { 
    //                 sTable->tables[index]->function->outOrder = realloc(sTable->tables[index]->function->outOrder, (sizeof(int) * count)); 
    //             } 

    //             entry* new = makeEntry(child->elem->lex.lexemeStr, sTable->numTypes - 1, sTable->tables[index]->currWidth, child->elem->lineNo); 
    //             int ins = insertS(new, out->fields); 
    //             sTable->tables[index]->function->outOrder[count - 1] = ins; 
    //         } 
    //         else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_UNION") == 0) { 
    //             count += 1; 
    //             child = child->next; 
                
    //             typeInfo* newT = (typeInfo*) malloc(sizeof(typeInfo)); 
    //             int l = strlen(child->elem->lex.lexemeStr); 
    //             newT->name = (char*) malloc(sizeof(char) * (l + 1)); 
    //             for(int i = 0; i < l; i++){
    //                 newT->name[i] = child->elem->lex.lexemeStr[i];
    //             } 
    //             newT->name[l] = '\0'; 
    //             newT->typeId = 4; 
    //             newT->ref = -1; 
    //             newT->numFields = 0; 
    //             newT->numAl = 0; 
    //             // newT->fInd = -1; 
    //             insertType(newT, sTable); 
    //             child = child->next; 

    //             if (out->numFields == 0) { 
    //                 out->fields = initialiseS(); 
    //             } 
    //             out->numFields += 1; 

    //             if (count == 1) { 
    //                 sTable->tables[index]->function->outOrder = (int*) malloc(sizeof(int)); 
    //             } 
    //             else { 
    //                 sTable->tables[index]->function->outOrder = realloc(sTable->tables[index]->function->outOrder, (sizeof(int) * count)); 
    //             } 

    //             entry* new = makeEntry(child->elem->lex.lexemeStr, sTable->numTypes - 1, sTable->tables[index]->currWidth, child->elem->lineNo); 
    //             int ins = insertS(new, out->fields); 
    //             sTable->tables[index]->function->outOrder[count - 1] = ins; 
    //         } 
    //         else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_RUID") == 0) { 
    //             count += 1; 
    //             // child = child->next; 
                
    //             typeInfo* newT = (typeInfo*) malloc(sizeof(typeInfo)); 
    //             int l = strlen(child->elem->lex.lexemeStr); 
    //             newT->name = (char*) malloc(sizeof(char) * (l + 1)); 
    //             for(int i = 0; i < l; i++){
    //                 newT->name[i] = child->elem->lex.lexemeStr[i];
    //             } 
    //             newT->name[l] = '\0'; 
    //             newT->ref = -1; 
    //             newT->numFields = 0; 
    //             newT->numAl = 0; 
    //             // newT->fInd = -1; 
    //             // newT->typeId; // TypeId not known, could be union or record 
    //             insertType(newT, sTable); 
    //             child = child->next; 

    //             if (out->numFields == 0) { 
    //                 out->fields = initialiseS(); 
    //             } 
    //             out->numFields += 1; 

    //             if (count == 1) { 
    //                 sTable->tables[index]->function->outOrder = (int*) malloc(sizeof(int)); 
    //             } 
    //             else { 
    //                 sTable->tables[index]->function->outOrder = realloc(sTable->tables[index]->function->outOrder, (sizeof(int) * count)); 
    //             } 

    //             entry* new = makeEntry(child->elem->lex.lexemeStr, sTable->numTypes - 1, sTable->tables[index]->currWidth, child->elem->lineNo); 
    //             int ins = insertS(new, out->fields); 
    //             sTable->tables[index]->function->outOrder[count - 1] = ins; 
    //         }
    //         else if (child->elem->isLeaf == 1 && child->elem->curr == 0) { 
    //             // skip 
    //             // printf("Epsilon encountered out\n"); 
    //         } // Record!!! 
    //         else { 
    //             handleNT(child, G, sTable, index, 0); 
    //         } 
    //         child = child->next; 
    //     } 
    //     sTable->tables[index]->function->numOut = count; 
    //     insertType(out, sTable); 
    //     sTable->tables[index]->function->outId = sTable->numTypes - 1; 

    // } 
    else if (strcmp(G.nonTerminals[root->elem->curr], "output_par") == 0) { 
        // printf("output par \n"); 
        int count = 0; 
        astNode* child = root->child; 
        
        typeInfo* out = (typeInfo*) malloc(sizeof(typeInfo)); 
        out->isParam = 1; 
        out->isPrim = 0; 
        out->numFields = 0; 
        out->name = "outPar"; 
        out->ref = -1; 
        out->numAl = 0; 
        // out->fInd = index; 

        while(child != NULL) { 
            if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_INT") == 0) { 
                count += 1; 
                child = child->next; 
                if (out->numFields == 0) { 
                    out->fields = initialiseS(); 
                } 
                out->numFields += 1; 

                if (count == 1) { 
                    sTable->tables[index]->function->outOrder = (int*) malloc(sizeof(int)); 
                } 
                else { 
                    sTable->tables[index]->function->outOrder = realloc(sTable->tables[index]->function->outOrder, (sizeof(int) * count)); 
                } 

                entry* new = makeEntry(child->elem->lex.lexemeStr, 0, sTable->tables[index]->currWidth, child->elem->lineNo); 
                int ins = insertS(new, out->fields); 
                // sTable->tables[index]->currWidth += sTable->allTypes[0]->width; 
                sTable->tables[index]->function->outOrder[count - 1] = ins; 
            } 
            else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_REAL") == 0) { 
                count += 1; 
                child = child->next; 
                if (out->numFields == 0) { 
                    out->fields = initialiseS(); 
                } 
                out->numFields += 1; 

                if (count == 1) { 
                    sTable->tables[index]->function->outOrder = (int*) malloc(sizeof(int)); 
                } 
                else { 
                    sTable->tables[index]->function->outOrder = realloc(sTable->tables[index]->function->outOrder, (sizeof(int) * count)); 
                } 

                entry* new = makeEntry(child->elem->lex.lexemeStr, 1, sTable->tables[index]->currWidth, child->elem->lineNo); 
                int ins = insertS(new, out->fields); 
                // sTable->tables[index]->currWidth += sTable->allTypes[sTable->numTypes - 1]->width; 
                sTable->tables[index]->function->outOrder[count - 1] = ins; 
                // sTable->tables[index]->currWidth += new->width; 
                // printf("'%s' %d \n", new->varName, sTable->tables[index]->currWidth); 
            } 
            else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_RECORD") == 0) { 
                count += 1; 
                child = child->next; 
                
                int ind = searchTypes(child->elem->lex.lexemeStr, sTable); 
                if (ind == -1) { 
                    typeInfo* newT = (typeInfo*) malloc(sizeof(typeInfo)); 
                    int l = strlen(child->elem->lex.lexemeStr); 
                    newT->name = (char*) malloc(sizeof(char) * (l + 1)); 
                    for(int i = 0; i < l; i++){
                        newT->name[i] = child->elem->lex.lexemeStr[i];
                    } 
                    newT->name[l] = '\0'; 
                    newT->typeId = 3; 
                    newT->ref = -1; 
                    newT->numFields = 0; 
                    newT->numAl = 0; 
                    // newT->fInd = -1; 
                    insertType(newT, sTable); 
                    ind = sTable->numTypes - 1; 
                }
                
                child = child->next; 

                if (out->numFields == 0) { 
                    out->fields = initialiseS(); 
                } 
                out->numFields += 1; 

                if (count == 1) { 
                    sTable->tables[index]->function->outOrder = (int*) malloc(sizeof(int)); 
                } 
                else { 
                    sTable->tables[index]->function->outOrder = realloc(sTable->tables[index]->function->outOrder, (sizeof(int) * count)); 
                } 

                entry* new = makeEntry(child->elem->lex.lexemeStr, ind, sTable->tables[index]->currWidth, child->elem->lineNo); 
                int ins = insertS(new, out->fields); 
                sTable->tables[index]->function->outOrder[count - 1] = ins; 
            } 
            else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_UNION") == 0) { 
                count += 1; 
                child = child->next; 
                
                int ind = searchTypes(child->elem->lex.lexemeStr, sTable); 
                if (ind == -1) { 
                    typeInfo* newT = (typeInfo*) malloc(sizeof(typeInfo)); 
                    int l = strlen(child->elem->lex.lexemeStr); 
                    newT->name = (char*) malloc(sizeof(char) * (l + 1)); 
                    for(int i = 0; i < l; i++){
                        newT->name[i] = child->elem->lex.lexemeStr[i];
                    } 
                    newT->name[l] = '\0'; 
                    newT->typeId = 4; 
                    newT->ref = -1; 
                    newT->numFields = 0; 
                    newT->numAl = 0; 
                    // newT->fInd = -1; 
                    insertType(newT, sTable); 
                    ind = sTable->numTypes - 1; 
                }
                
                child = child->next; 

                if (out->numFields == 0) { 
                    out->fields = initialiseS(); 
                } 
                out->numFields += 1; 

                if (count == 1) { 
                    sTable->tables[index]->function->outOrder = (int*) malloc(sizeof(int)); 
                } 
                else { 
                    sTable->tables[index]->function->outOrder = realloc(sTable->tables[index]->function->outOrder, (sizeof(int) * count)); 
                } 

                entry* new = makeEntry(child->elem->lex.lexemeStr, ind, sTable->tables[index]->currWidth, child->elem->lineNo); 
                int ins = insertS(new, out->fields); 
                sTable->tables[index]->function->outOrder[count - 1] = ins; 
            } 
            else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_RUID") == 0) { 
                count += 1; 
                // child = child->next; 
                
                int ind = searchTypes(child->elem->lex.lexemeStr, sTable); 
                if (ind == -1) { 
                    typeInfo* newT = (typeInfo*) malloc(sizeof(typeInfo)); 
                    int l = strlen(child->elem->lex.lexemeStr); 
                    newT->name = (char*) malloc(sizeof(char) * (l + 1)); 
                    for(int i = 0; i < l; i++){
                        newT->name[i] = child->elem->lex.lexemeStr[i];
                    } 
                    newT->name[l] = '\0'; 
                    newT->ref = -1; 
                    newT->numFields = 0; 
                    newT->numAl = 0; 
                    // newT->fInd = -1; 
                    // newT->typeId; // TypeId not known, could be union or record 
                    insertType(newT, sTable); 
                    ind = sTable->numTypes - 1; 
                }
                
                child = child->next; 

                if (out->numFields == 0) { 
                    out->fields = initialiseS(); 
                } 
                out->numFields += 1; 

                if (count == 1) { 
                    sTable->tables[index]->function->outOrder = (int*) malloc(sizeof(int)); 
                } 
                else { 
                    sTable->tables[index]->function->outOrder = realloc(sTable->tables[index]->function->outOrder, (sizeof(int) * count)); 
                } 

                entry* new = makeEntry(child->elem->lex.lexemeStr, ind, sTable->tables[index]->currWidth, child->elem->lineNo); 
                int ins = insertS(new, out->fields); 
                sTable->tables[index]->function->outOrder[count - 1] = ins; 
            }
            else if (child->elem->isLeaf == 1 && child->elem->curr == 0) { 
                // skip 
                // printf("Epsilon encountered out\n"); 
            } // Record!!! 
            else { 
                handleNT(child, G, sTable, index, 0); 
            } 
            child = child->next; 
        } 
        sTable->tables[index]->function->numOut = count; 
        insertType(out, sTable); 
        sTable->tables[index]->function->outId = sTable->numTypes - 1; 

    } 
    // else if (strcmp(G.nonTerminals[root->elem->curr], "declaration") == 0) { 
        
    //     // printf("declaration \n"); 
    //     astNode* child = root->child; 
    //     while(child != NULL) { 
    //         if (child->elem->isLeaf == 1 && child->elem->curr == 0) { 
    //             // skip 
    //         } 
    //         else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_INT") == 0){ 
    //             child = child->next; 
                
    //             if (child->next->elem->isLeaf == 1 && child->next->elem->curr == 19) { 
    //                 if (sTable->numEntries == 0) { 
    //                     sTable->entries = initialiseS(); 
    //                 } 

    //                 entry* new = makeEntry(child->elem->lex.lexemeStr, 0, sTable->mainOffset, child->elem->lineNo); 
    //                 insertS(new, sTable->entries); 
    //                 sTable->mainOffset += sTable->allTypes[0]->width; 
    //                 sTable->numEntries += 1; 
    //             } 
    //             else { 
    //                 if (sTable->tables[index]->numEntries == 0) { 
    //                     sTable->tables[index]->entries = initialiseS(); 
    //                 }
            
    //                 entry* new = makeEntry(child->elem->lex.lexemeStr, 0, sTable->tables[index]->currWidth, child->elem->lineNo); 
    //                 insertS(new, sTable->tables[index]->entries); 
    //                 sTable->tables[index]->currWidth += sTable->allTypes[0]->width; 
    //                 sTable->tables[index]->numEntries += 1; 
    //             } 
    //         } 
    //         else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_REAL") == 0) { 
    //             child = child->next; 
                
    //             if (child->next->elem->isLeaf == 1 && child->next->elem->curr == 19) { 
    //                 if (sTable->numEntries == 0) { 
    //                     sTable->entries = initialiseS(); 
    //                 } 

    //                 entry* new = makeEntry(child->elem->lex.lexemeStr, 1, sTable->mainOffset, child->elem->lineNo); 
    //                 insertS(new, sTable->entries); 
    //                 sTable->mainOffset += sTable->allTypes[1]->width; 
    //                 sTable->numEntries += 1; 
    //             } 
    //             else { 
    //                 if (sTable->tables[index]->numEntries == 0) { 
    //                     sTable->tables[index]->entries = initialiseS(); 
    //                 }
            
    //                 entry* new = makeEntry(child->elem->lex.lexemeStr, 1, sTable->tables[index]->currWidth, child->elem->lineNo); 
    //                 insertS(new, sTable->tables[index]->entries); 
    //                 sTable->tables[index]->currWidth += sTable->allTypes[1]->width; 
    //                 sTable->tables[index]->numEntries += 1; 
    //             } 
    //         } 
    //         else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_RECORD") == 0) { 
    //             // printf("done \n"); 
    //             child = child->next; 

    //             int ind = searchTypes(child->elem->lex.lexemeStr, sTable); 
    //             // printf("done \n"); 
    //             if (ind == -1) { // This record type has not yet been defined or used in the parameters 
    //                 typeInfo* newT = (typeInfo*) malloc(sizeof(typeInfo)); 
    //                 int l = strlen(child->elem->lex.lexemeStr); 
    //                 newT->name = (char*) malloc(sizeof(char) * (l + 1)); 
    //                 for(int i = 0; i < l; i++){
    //                     newT->name[i] = child->elem->lex.lexemeStr[i];
    //                 } 
    //                 newT->name[l] = '\0'; 
    //                 newT->typeId = 3; 
    //                 newT->ref = -1; 
    //                 // newT->fInd = -1; 
    //                 insertType(newT, sTable); 
    //                 ind = sTable->numTypes - 1; 
    //             } 

    //             // printf("done \n"); 
    //             child = child->next; 
                
    //             if (child->next->elem->isLeaf == 1 && child->next->elem->curr == 19) { 
    //                 if (sTable->numEntries == 0) { 
    //                     sTable->entries = initialiseS(); 
    //                 } 

    //                 entry* new = makeEntry(child->elem->lex.lexemeStr, ind, sTable->mainOffset, child->elem->lineNo); 
    //                 insertS(new, sTable->entries); 
    //                 sTable->mainOffset += sTable->allTypes[ind]->width; 
    //                 sTable->numEntries += 1; 
    //             } 
    //             else { 
    //                 if (sTable->tables[index]->numEntries == 0) { 
    //                     sTable->tables[index]->entries = initialiseS(); 
    //                 }
            
    //                 entry* new = makeEntry(child->elem->lex.lexemeStr, ind, sTable->tables[index]->currWidth, child->elem->lineNo); 
    //                 insertS(new, sTable->tables[index]->entries); 
    //                 sTable->tables[index]->currWidth += sTable->allTypes[ind]->width; 
    //                 sTable->tables[index]->numEntries += 1; 
    //             } 
                
    //             // if (sTable->tables[index]->numEntries == 0) { 
    //             //     sTable->tables[index]->entries = initialiseS(); 
    //             // }
            
    //             // // printf("done \n"); 
    //             // entry* new = makeEntry(child->elem->lex.lexemeStr, ind, sTable->tables[index]->currWidth, child->elem->lineNo); 
    //             // insertS(new, sTable->tables[index]->entries); 
    //             // sTable->tables[index]->currWidth += sTable->allTypes[ind]->width; 
    //             // sTable->tables[index]->numEntries += 1; 
    //             // // printf("done %d %d %s %s \n", sTable->numTypes, sTable->tables[index]->currWidth, sTable->tables[index]->entries[0]->varName, child->elem->lex.lexemeStr); 
    //         } 
    //         else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_UNION") == 0) { 
    //             child = child->next; 

    //             int ind = searchTypes(child->elem->lex.lexemeStr, sTable); 
    //             if (ind == -1) { // This record type has not yet been defined or used in the parameters 
    //                 typeInfo* newT = (typeInfo*) malloc(sizeof(typeInfo)); 
    //                 int l = strlen(child->elem->lex.lexemeStr); 
    //                 newT->name = (char*) malloc(sizeof(char) * (l + 1)); 
    //                 for(int i = 0; i < l; i++){
    //                     newT->name[i] = child->elem->lex.lexemeStr[i];
    //                 } 
    //                 newT->name[l] = '\0'; 
    //                 newT->typeId = 4; 
    //                 newT->ref = -1; 
    //                 // newT->fInd = -1; 
    //                 insertType(newT, sTable); 
    //                 ind = sTable->numTypes - 1; 
    //             } 

    //             child = child->next; 
                
    //             if (child->next->elem->isLeaf == 1 && child->next->elem->curr == 19) { 
    //                 if (sTable->numEntries == 0) { 
    //                     sTable->entries = initialiseS(); 
    //                 } 

    //                 entry* new = makeEntry(child->elem->lex.lexemeStr, ind, sTable->mainOffset, child->elem->lineNo); 
    //                 insertS(new, sTable->entries); 
    //                 sTable->mainOffset += sTable->allTypes[ind]->width; 
    //                 sTable->numEntries += 1; 
    //             } 
    //             else { 
    //                 if (sTable->tables[index]->numEntries == 0) { 
    //                     sTable->tables[index]->entries = initialiseS(); 
    //                 }
            
    //                 entry* new = makeEntry(child->elem->lex.lexemeStr, ind, sTable->tables[index]->currWidth, child->elem->lineNo); 
    //                 insertS(new, sTable->tables[index]->entries); 
    //                 sTable->tables[index]->currWidth += sTable->allTypes[ind]->width; 
    //                 sTable->tables[index]->numEntries += 1; 
    //             } 
                
    //             // if (sTable->tables[index]->numEntries == 0) { 
    //             //     sTable->tables[index]->entries = initialiseS(); 
    //             // }
            
    //             // entry* new = makeEntry(child->elem->lex.lexemeStr, ind, sTable->tables[index]->currWidth, child->elem->lineNo); 
    //             // insertS(new, sTable->tables[index]->entries); 
    //             // sTable->tables[index]->currWidth += sTable->allTypes[ind]->width; 
    //             // sTable->tables[index]->numEntries += 1; 
    //         } 
    //         else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_RUID") == 0) { 
    //             // child = child->next; 

    //             int ind = searchTypes(child->elem->lex.lexemeStr, sTable); 
    //             if (ind == -1) { // This record type has not yet been defined or used in the parameters 
    //                 typeInfo* newT = (typeInfo*) malloc(sizeof(typeInfo)); 
    //                 int l = strlen(child->elem->lex.lexemeStr); 
    //                 newT->name = (char*) malloc(sizeof(char) * (l + 1)); 
    //                 for(int i = 0; i < l; i++){
    //                     newT->name[i] = child->elem->lex.lexemeStr[i];
    //                 } 
    //                 newT->name[l] = '\0'; 
    //                 newT->ref = -1; 
    //                 // newT->fInd = -1; 
    //                 // newT->typeId; // TypeId not known, could be union or record 
    //                 insertType(newT, sTable); 
    //                 ind = sTable->numTypes - 1; 
    //             } 

    //             child = child->next; 

    //             if (child->next->elem->isLeaf == 1 && child->next->elem->curr == 19) { 
    //                 if (sTable->numEntries == 0) { 
    //                     sTable->entries = initialiseS(); 
    //                 } 

    //                 entry* new = makeEntry(child->elem->lex.lexemeStr, ind, sTable->mainOffset, child->elem->lineNo); 
    //                 insertS(new, sTable->entries); 
    //                 sTable->mainOffset += sTable->allTypes[ind]->width; 
    //                 sTable->numEntries += 1; 
    //             } 
    //             else { 
    //                 if (sTable->tables[index]->numEntries == 0) { 
    //                     sTable->tables[index]->entries = initialiseS(); 
    //                 }
            
    //                 entry* new = makeEntry(child->elem->lex.lexemeStr, ind, sTable->tables[index]->currWidth, child->elem->lineNo); 
    //                 insertS(new, sTable->tables[index]->entries); 
    //                 sTable->tables[index]->currWidth += sTable->allTypes[ind]->width; 
    //                 sTable->tables[index]->numEntries += 1; 
    //             } 
                
    //             // if (sTable->tables[index]->numEntries == 0) { 
    //             //     sTable->tables[index]->entries = initialiseS(); 
    //             // }

    //             // child = child->next; 
    //             // entry* new = makeEntry(child->elem->lex.lexemeStr, ind, sTable->tables[index]->currWidth, child->elem->lineNo); 
    //             // insertS(new, sTable->tables[index]->entries); 
    //             // sTable->tables[index]->currWidth += sTable->allTypes[ind]->width; 
    //             // sTable->tables[index]->numEntries += 1; 
    //         }
    //         child = child->next; 
    //     }
    // } 
    else if (strcmp(G.nonTerminals[root->elem->curr], "typeDefinitions") == 0) { 
        astNode* child = root->child; 
        while (child != NULL) { 
            if (child->elem->isLeaf == 1 && child->elem->curr == 0) { 
                // skip 
            } 
            else { 
                // printf("Handling %s \n", G.nonTerminals[child->elem->curr]); 
                handleNT(child, G, sTable, index, 0); 
            } 
            child = child->next; 
        }
    } 
    else if (strcmp(G.nonTerminals[root->elem->curr], "typeDefinition") == 0) { 
        astNode* child = root->child; 
        while (child != NULL) { 
            if (strcmp(G.terminals[child->elem->curr], "TK_RECORD") == 0) { 
                child = child->next; 
                
                int ind = searchTypes(child->elem->lex.lexemeStr, sTable); 
                // printf("done \n"); 
                if (ind == -1) { // This record type has not yet been defined or used in the parameters 
                    typeInfo* newT = (typeInfo*) malloc(sizeof(typeInfo)); 
                    int l = strlen(child->elem->lex.lexemeStr); 
                    newT->name = (char*) malloc(sizeof(char) * (l + 1)); 
                    for(int i = 0; i < l; i++){
                        newT->name[i] = child->elem->lex.lexemeStr[i];
                    } 
                    newT->name[l] = '\0'; 
                    newT->typeId = 3; 
                    newT->ref = -1; 
                    newT->numFields = 0; 
                    newT->numAl = 0; 
                    // newT->fInd = index; 
                    newT->width = 0; 
                    insertType(newT, sTable); 
                    ind = sTable->numTypes - 1; 
                } 

                child = child->next; 
                // printf("Handling %s \n", G.nonTerminals[child->elem->curr]); 
                handleNT(child, G, sTable, index, ind); 
                // printf("In handleNT for record: %s %d \n", sTable->allTypes[ind]->name, sTable->allTypes[ind]->width); 
                updateWidth(ind, sTable, sTable->allTypes[ind]->width); 
                bool varRec = false; 
                // bool tagIsNotInt = false; 
                // bool incorName = true; 
                bool wrongTag = true; 
                bool moreThanOneUnion = false; 
                for (int i = 0; i < size; i++) { 
                    if (sTable->allTypes[ind]->fields[i]->present == 1 && sTable->allTypes[sTable->allTypes[ind]->fields[i]->type]->typeId == 4) { 
                        if (varRec == true) { 
                            moreThanOneUnion = true; 
                        } 
                        else { 
                            varRec = true; 
                        } 
                    } 
                    else if (sTable->allTypes[ind]->fields[i]->present == 1 && sTable->allTypes[sTable->allTypes[ind]->fields[i]->type]->typeId != 4) { 
                        // printf("%s TypeID %d \n", sTable->allTypes[ind]->name, sTable->allTypes[sTable->allTypes[ind]->fields[i]->type]->typeId); 
                        if (sTable->allTypes[sTable->allTypes[ind]->fields[i]->type]->typeId == 0) { 
                            if (strcmp(sTable->allTypes[ind]->fields[i]->varName, "tagvalue") == 0) { 
                                wrongTag = false; 
                            } 
                        } 
                    }
                } 
                if (varRec && moreThanOneUnion) { 
                    printf("Variant record has more than one union field: %s ERROR \n", sTable->allTypes[ind]->name); 
                }
                
                if (varRec && wrongTag) { 
                    printf("Tag for union in variant record %s missing, not of type int or not named correctly ERROR \n", sTable->allTypes[ind]->name); 
                } 

            } 
            else if (strcmp(G.terminals[child->elem->curr], "TK_UNION") == 0) { 
                child = child->next; 
                
                int ind = searchTypes(child->elem->lex.lexemeStr, sTable); 
                if (ind == -1) { // This record type has not yet been defined or used in the parameters 
                    typeInfo* newT = (typeInfo*) malloc(sizeof(typeInfo)); 
                    int l = strlen(child->elem->lex.lexemeStr); 
                    newT->name = (char*) malloc(sizeof(char) * (l + 1)); 
                    for(int i = 0; i < l; i++){
                        newT->name[i] = child->elem->lex.lexemeStr[i];
                    } 
                    newT->name[l] = '\0'; 
                    newT->typeId = 4; 
                    newT->ref = -1; 
                    newT->numFields = 0; 
                    newT->numAl = 0; 
                    // newT->fInd = index; 
                    newT->width = 0; 
                    insertType(newT, sTable); 
                    ind = sTable->numTypes - 1; 
                } 

                child = child->next; 
                // printf("Handling %s \n", G.nonTerminals[child->elem->curr]); 
                handleNT(child, G, sTable, index, ind); 
                // printf("In handleNT for union: %s %d \n", sTable->allTypes[ind]->name, sTable->allTypes[ind]->width); 
                // updateWidth(ind, sTable, sTable->allTypes[ind]->width); 
                int maxW = 0; 
                for (int i = 0; i < size; i++) { 
                    if (sTable->allTypes[ind]->fields[i]->present == 1) { 
                        sTable->allTypes[ind]->fields[i]->offset = 0; 
                        if (sTable->allTypes[sTable->allTypes[ind]->fields[i]->type]->width > maxW) { 
                            maxW = sTable->allTypes[sTable->allTypes[ind]->fields[i]->type]->width; 
                        } 
                    }
                } 
                sTable->allTypes[ind]->width = maxW; 

            } 
            child = child->next; 
        }
    } 
    else if (strcmp(G.nonTerminals[root->elem->curr], "fieldDefinitions") == 0) { 
        astNode* child = root->child; 
        // int count = 0; 
        while (child != NULL) { 
            if (child->elem->isLeaf == 1 && child->elem->curr == 0) { 
                // skip
            } 
            else { 
                // count += 1; 
                // printf("Handling %s \n", G.nonTerminals[child->elem->curr]); 
                handleNT(child, G, sTable, index, recInd); 
            } 
            child = child->next; 
        } 
        // sTable->allTypes[sTable->numTypes - 1]->numFields = count; 
    } 
    else if (strcmp(G.nonTerminals[root->elem->curr], "moreFields") == 0) { 
        astNode* child = root->child; 
        // int count = 0; 
        while (child != NULL) { 
            if (child->elem->isLeaf == 1 && child->elem->curr == 0) { 
                // skip
            } 
            else { 
                // count += 1; 
                // printf("Handling %s \n", G.nonTerminals[child->elem->curr]); 
                handleNT(child, G, sTable, index, recInd); 
            } 
            child = child->next; 
        } 
        // sTable->allTypes[sTable->numTypes - 1]->numFields = count; 
    } 
    else if (strcmp(G.nonTerminals[root->elem->curr], "fieldDefinition") == 0) { 
        astNode* child = root->child; 
        // printf("FIELD DEFINITION TYPE %s \n", child->elem->lex.lexemeStr); 
        while (child != NULL) { 
            if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_INT") == 0) { 
                int typeInd = recInd; 
                child = child->next; 
                // printf("FIELD DEFINITION NAME %s \n", child->elem->lex.lexemeStr); 
                if (sTable->allTypes[typeInd]->numFields == 0) { 
                    sTable->allTypes[typeInd]->fields = initialiseS(); 
                    sTable->allTypes[typeInd]->fOrder = (int*) malloc(sizeof(int)); 
                } 
                else { 
                    sTable->allTypes[typeInd]->fOrder = realloc(sTable->allTypes[typeInd]->fOrder, (sizeof(int) * (sTable->allTypes[typeInd]->numFields + 1))); 
                }
                sTable->allTypes[typeInd]->numFields += 1; 

                entry* new = makeEntry(child->elem->lex.lexemeStr, 0, sTable->allTypes[typeInd]->width, child->elem->lineNo); 
                int ins = insertS(new, sTable->allTypes[typeInd]->fields); 
                sTable->allTypes[typeInd]->width += sTable->allTypes[0]->width; 
                sTable->allTypes[typeInd]->fOrder[sTable->allTypes[typeInd]->numFields - 1] = ins; 
                // sTable->tables[index]->currWidth += new->width; 
                // sTable->tables[index]->numEntries += 1; 
            } 
            else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_REAL") == 0) { 
                int typeInd = recInd; 
                child = child->next; 
                // printf("FIELD DEFINITION NAME %s \n", child->elem->lex.lexemeStr); 
                if (sTable->allTypes[typeInd]->numFields == 0) { 
                    sTable->allTypes[typeInd]->fields = initialiseS(); 
                    sTable->allTypes[typeInd]->fOrder = (int*) malloc(sizeof(int)); 
                } 
                else { 
                    sTable->allTypes[typeInd]->fOrder = realloc(sTable->allTypes[typeInd]->fOrder, (sizeof(int) * (sTable->allTypes[typeInd]->numFields + 1))); 
                }
                sTable->allTypes[typeInd]->numFields += 1; 

                entry* new = makeEntry(child->elem->lex.lexemeStr, 1, sTable->allTypes[typeInd]->width, child->elem->lineNo); 
                int ins = insertS(new, sTable->allTypes[typeInd]->fields); 
                sTable->allTypes[typeInd]->width += sTable->allTypes[1]->width; 
                sTable->allTypes[typeInd]->fOrder[sTable->allTypes[typeInd]->numFields - 1] = ins; 
            } 
            else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_RUID") == 0) { 
                int typeInd = recInd; 
                
                int ind = searchTypes(child->elem->lex.lexemeStr, sTable); 
                if (ind == -1) { // This record type has not yet been defined or used in the parameters 
                    typeInfo* newT = (typeInfo*) malloc(sizeof(typeInfo)); 
                    int l = strlen(child->elem->lex.lexemeStr); 
                    newT->name = (char*) malloc(sizeof(char) * (l + 1)); 
                    for(int i = 0; i < l; i++){
                        newT->name[i] = child->elem->lex.lexemeStr[i];
                    } 
                    newT->name[l] = '\0'; 
                    newT->ref = -1; 
                    newT->numFields = 0; 
                    newT->numAl = 0; 
                    // newT->fInd = -1; 
                    // newT->typeId; // TypeId not known, could be union or record 
                    insertType(newT, sTable); 
                    ind = sTable->numTypes - 1; 
                } 

                if (sTable->allTypes[typeInd]->numFields == 0) { 
                    sTable->allTypes[typeInd]->fields = initialiseS(); 
                    sTable->allTypes[typeInd]->fOrder = (int*) malloc(sizeof(int)); 
                } 
                else { 
                    sTable->allTypes[typeInd]->fOrder = realloc(sTable->allTypes[typeInd]->fOrder, (sizeof(int) * (sTable->allTypes[typeInd]->numFields + 1))); 
                }
                sTable->allTypes[typeInd]->numFields += 1; 

                child = child->next; 
                entry* new = makeEntry(child->elem->lex.lexemeStr, ind, sTable->allTypes[typeInd]->width, child->elem->lineNo); 
                int ins = insertS(new, sTable->allTypes[typeInd]->fields); 
                // printf("Hash %d \n", hash); 
                sTable->allTypes[typeInd]->width += sTable->allTypes[ind]->width; 
                sTable->allTypes[typeInd]->fOrder[sTable->allTypes[typeInd]->numFields - 1] = ins; 
                // printf("%s %d %s \n", sTable->allTypes[typeInd]->name, typeInd, sTable->allTypes[typeInd]->fields[hash]->varName); 
            } 
            child = child->next; 
        }
    } 
    else if (strcmp(G.nonTerminals[root->elem->curr], "definetypestmt") == 0) { 
        
        // printf("In definetypestmt \n"); 
        astNode* child = root->child; 
        int typeId; 
        int typeInd; 
        int flag = 0; 
        // printf("done \n"); 
        while (child != NULL) { 
            if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_DEFINETYPE") == 0) { 
                // skip 
            } 
            else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_RECORD") == 0) { 
                typeId = 3; 
            } 
            else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_UNION") == 0) { 
                typeId = 4; 
            } 
            else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_AS") == 0) { 
                flag = 1; 
                // skip
            } 
            else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_RUID") == 0) { 
                if (flag == 0) { 
                    // printf("%s \n", child->elem->lex.lexemeStr); 
                    typeInd = searchTypes(child->elem->lex.lexemeStr, sTable); 
                    // printf("done %d \n", typeInd); 
                    if (typeInd == -1) { 
                        
                        typeInfo* newT = (typeInfo*) malloc(sizeof(typeInfo)); 
                        int l = strlen(child->elem->lex.lexemeStr); 
                        newT->name = (char*) malloc(sizeof(char) * (l + 1)); 
                        for(int i = 0; i < l; i++){
                            newT->name[i] = child->elem->lex.lexemeStr[i];
                        } 
                        newT->name[l] = '\0'; 
                        newT->ref = -1; 
                        newT->numFields = 0; 
                        newT->numAl = 0; 
                        // newT->fInd = -1; 
                        // newT->typeId; // TypeId not known, could be union or record 
                        insertType(newT, sTable); 
                        typeInd = sTable->numTypes - 1; 
                        
                        
                        // printf("This record has not been defined %s at line %d ERROR \n", child->elem->lex.lexemeStr, child->elem->lineNo); 
                        // break; 
                    } 
                    else { 
                        if (sTable->allTypes[typeInd]->ref != -1) { 
                            typeInd = sTable->allTypes[typeInd]->ref; 
                        }
                    }
                } 
                else { 
                    if (sTable->allTypes[typeInd]->numAl == 0) { 
                        sTable->allTypes[typeInd]->aliases = (char**) malloc(sizeof(char*)); 
                    } 
                    
                    int oldInd = searchTypes(child->elem->lex.lexemeStr, sTable); 
                    if (oldInd != -1) { // An entry in allTypes has already been made for this 
                        // printf("In definetypestmt: %s %d %d \n", sTable->allTypes[oldInd]->name, oldInd, sTable->allTypes[typeInd]->width); 
                        sTable->allTypes[oldInd]->ref = typeInd; 
                        sTable->allTypes[oldInd]->typeId = sTable->allTypes[typeInd]->typeId; 
                        sTable->allTypes[oldInd]->width = sTable->allTypes[typeInd]->width; 
                        updateWidth(oldInd, sTable, sTable->allTypes[typeInd]->width); 
                    } 
                    else { 
                        typeInfo* newT = (typeInfo*) malloc(sizeof(typeInfo)); 
                        int l = strlen(child->elem->lex.lexemeStr); 
                        newT->name = (char*) malloc(sizeof(char) * (l + 1)); 
                        for(int i = 0; i < l; i++){
                            newT->name[i] = child->elem->lex.lexemeStr[i];
                        } 
                        newT->name[l] = '\0'; 
                        newT->ref = -1; 
                        newT->numFields = 0; 
                        newT->numAl = 0; 
                        // newT->fInd = -1; 
                        // newT->typeId; // TypeId not known, could be union or record 
                        insertType(newT, sTable); 
                        oldInd = sTable->numTypes - 1; 
                        sTable->allTypes[oldInd]->ref = typeInd; 
                        sTable->allTypes[oldInd]->typeId = sTable->allTypes[typeInd]->typeId; 
                        sTable->allTypes[oldInd]->width = sTable->allTypes[typeInd]->width; 
                        updateWidth(oldInd, sTable, sTable->allTypes[typeInd]->width); 
                    }
                    
                    // printf("done \n"); 
                    sTable->allTypes[typeInd]->numAl += 1; 
                    int l = strlen(child->elem->lex.lexemeStr); 
                    sTable->allTypes[typeInd]->aliases[0] = (char*) malloc(sizeof(char) * (l + 1)); 
                    for(int i = 0; i < l; i++){
                        sTable->allTypes[typeInd]->aliases[0][i] = child->elem->lex.lexemeStr[i];
                    } 
                    sTable->allTypes[typeInd]->aliases[0][l] = '\0'; 
                }
            } 
            child = child->next; 
        }
    }
}

/*
    Handles the declaration statements and populates the symbol table
*/
void handleDecl(astNode* root, grammar G, symbolTable* sTable, int index) { 
    astNode* child1 = root->child; 
    while (child1 != NULL) { 
        if (child1->elem->isLeaf == 0 && strcmp(G.nonTerminals[child1->elem->curr], "input_par") == 0) { 
            int inId = sTable->tables[index]->function->inId; 
            for (int i = 0; i < sTable->tables[index]->function->numIn; i++) { 
                int field = sTable->tables[index]->function->inOrder[i]; 
                sTable->allTypes[inId]->fields[field]->offset = sTable->tables[index]->currWidth; 
                int inT = sTable->allTypes[inId]->fields[field]->type; 
                // int tName = sTable->allTypes[inT]->name; 
                // int type = searchTypes(tName, sTable); 
                sTable->tables[index]->currWidth += sTable->allTypes[inT]->width; 
            }
        } 
        else if (child1->elem->isLeaf == 0 && strcmp(G.nonTerminals[child1->elem->curr], "output_par") == 0) { 
            int outId = sTable->tables[index]->function->outId; 
            for (int i = 0; i < sTable->tables[index]->function->numOut; i++) { 
                int field = sTable->tables[index]->function->outOrder[i]; 
                sTable->allTypes[outId]->fields[field]->offset = sTable->tables[index]->currWidth; 
                int outT = sTable->allTypes[outId]->fields[field]->type; 
                // int tName = sTable->allTypes[outT]->name; 
                // int type = searchTypes(tName, sTable); 
                sTable->tables[index]->currWidth += sTable->allTypes[outT]->width; 
            }
        } 
        else if (child1->elem->isLeaf == 0 && strcmp(G.nonTerminals[child1->elem->curr], "declaration") == 0) { 
            // printf("declaration \n"); 
            astNode* child = child1->child; 
            while(child != NULL) { 
                if (child->elem->isLeaf == 1 && child->elem->curr == 0) { 
                    // skip 
                } 
                else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_INT") == 0){ 
                    child = child->next; 
                
                    if (child->next->elem->isLeaf == 1 && child->next->elem->curr == 19) { 
                        if (sTable->numEntries == 0) { 
                            sTable->entries = initialiseS(); 
                        } 

                        entry* new = makeEntry(child->elem->lex.lexemeStr, 0, sTable->mainOffset, child->elem->lineNo); 
                        insertS(new, sTable->entries); 
                        sTable->mainOffset += sTable->allTypes[0]->width; 
                        sTable->numEntries += 1; 
                    } 
                    else { 
                        if (sTable->tables[index]->numEntries == 0) { 
                            sTable->tables[index]->entries = initialiseS(); 
                        }
            
                        entry* new = makeEntry(child->elem->lex.lexemeStr, 0, sTable->tables[index]->currWidth, child->elem->lineNo); 
                        insertS(new, sTable->tables[index]->entries); 
                        sTable->tables[index]->currWidth += sTable->allTypes[0]->width; 
                        sTable->tables[index]->numEntries += 1; 
                    } 
                } 
                else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_REAL") == 0) { 
                    child = child->next; 
                
                    if (child->next->elem->isLeaf == 1 && child->next->elem->curr == 19) { 
                        if (sTable->numEntries == 0) { 
                            sTable->entries = initialiseS(); 
                        } 

                        entry* new = makeEntry(child->elem->lex.lexemeStr, 1, sTable->mainOffset, child->elem->lineNo); 
                        insertS(new, sTable->entries); 
                        sTable->mainOffset += sTable->allTypes[1]->width; 
                        sTable->numEntries += 1; 
                    } 
                    else { 
                        if (sTable->tables[index]->numEntries == 0) { 
                            sTable->tables[index]->entries = initialiseS(); 
                        }
            
                        entry* new = makeEntry(child->elem->lex.lexemeStr, 1, sTable->tables[index]->currWidth, child->elem->lineNo); 
                        insertS(new, sTable->tables[index]->entries); 
                        sTable->tables[index]->currWidth += sTable->allTypes[1]->width; 
                        sTable->tables[index]->numEntries += 1; 
                    } 
                } 
                else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_RECORD") == 0) { 
                    // printf("done \n"); 
                    child = child->next; 

                    int ind = searchTypes(child->elem->lex.lexemeStr, sTable); 
                    // printf("done \n"); 
                    if (ind == -1) { // This record type has not yet been defined or used in the parameters 
                        typeInfo* newT = (typeInfo*) malloc(sizeof(typeInfo)); 
                        int l = strlen(child->elem->lex.lexemeStr); 
                        newT->name = (char*) malloc(sizeof(char) * (l + 1)); 
                        for(int i = 0; i < l; i++){
                            newT->name[i] = child->elem->lex.lexemeStr[i];
                        } 
                        newT->name[l] = '\0'; 
                        newT->typeId = 3; 
                        newT->ref = -1; 
                        newT->numFields = 0; 
                        newT->numAl = 0; 
                        // newT->fInd = -1; 
                        insertType(newT, sTable); 
                        ind = sTable->numTypes - 1; 
                    } 

                    // printf("done \n"); 
                    child = child->next; 
                
                    if (child->next->elem->isLeaf == 1 && child->next->elem->curr == 19) { 
                        if (sTable->numEntries == 0) { 
                            sTable->entries = initialiseS(); 
                        } 

                        entry* new = makeEntry(child->elem->lex.lexemeStr, ind, sTable->mainOffset, child->elem->lineNo); 
                        insertS(new, sTable->entries); 
                        sTable->mainOffset += sTable->allTypes[ind]->width; 
                        sTable->numEntries += 1; 
                    } 
                    else { 
                        if (sTable->tables[index]->numEntries == 0) { 
                            sTable->tables[index]->entries = initialiseS(); 
                        }
            
                        entry* new = makeEntry(child->elem->lex.lexemeStr, ind, sTable->tables[index]->currWidth, child->elem->lineNo); 
                        insertS(new, sTable->tables[index]->entries); 
                        sTable->tables[index]->currWidth += sTable->allTypes[ind]->width; 
                        sTable->tables[index]->numEntries += 1; 
                    } 
                
                    // if (sTable->tables[index]->numEntries == 0) { 
                    //     sTable->tables[index]->entries = initialiseS(); 
                    // }
            
                    // // printf("done \n"); 
                    // entry* new = makeEntry(child->elem->lex.lexemeStr, ind, sTable->tables[index]->currWidth, child->elem->lineNo); 
                    // insertS(new, sTable->tables[index]->entries); 
                    // sTable->tables[index]->currWidth += sTable->allTypes[ind]->width; 
                    // sTable->tables[index]->numEntries += 1; 
                    // // printf("done %d %d %s %s \n", sTable->numTypes, sTable->tables[index]->currWidth, sTable->tables[index]->entries[0]->varName, child->elem->lex.lexemeStr); 
                } 
                else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_UNION") == 0) { 
                    
                    // printf("Variable cannot be of union type at line %d ERROR \n", child->elem->lineNo); 
                    // return; 
                    // child = child->next; 

                    // int ind = searchTypes(child->elem->lex.lexemeStr, sTable); 
                    // if (ind == -1) { // This record type has not yet been defined or used in the parameters 
                    //     typeInfo* newT = (typeInfo*) malloc(sizeof(typeInfo)); 
                    //     int l = strlen(child->elem->lex.lexemeStr); 
                    //     newT->name = (char*) malloc(sizeof(char) * (l + 1)); 
                    //     for(int i = 0; i < l; i++){
                    //         newT->name[i] = child->elem->lex.lexemeStr[i];
                    //     } 
                    //     newT->name[l] = '\0'; 
                    //     newT->typeId = 4; 
                    //     newT->ref = -1; 
                    //     newT->numFields = 0; 
                    //     newT->numAl = 0; 
                    //     // newT->fInd = -1; 
                    //     insertType(newT, sTable); 
                    //     ind = sTable->numTypes - 1; 
                    // } 

                    // child = child->next; 
                
                    // if (child->next->elem->isLeaf == 1 && child->next->elem->curr == 19) { 
                    //     if (sTable->numEntries == 0) { 
                    //         sTable->entries = initialiseS(); 
                    //     } 

                    //     entry* new = makeEntry(child->elem->lex.lexemeStr, ind, sTable->mainOffset, child->elem->lineNo); 
                    //     insertS(new, sTable->entries); 
                    //     sTable->mainOffset += sTable->allTypes[ind]->width; 
                    //     sTable->numEntries += 1; 
                    // } 
                    // else { 
                    //     if (sTable->tables[index]->numEntries == 0) { 
                    //         sTable->tables[index]->entries = initialiseS(); 
                    //     }
            
                    //     entry* new = makeEntry(child->elem->lex.lexemeStr, ind, sTable->tables[index]->currWidth, child->elem->lineNo); 
                    //     insertS(new, sTable->tables[index]->entries); 
                    //     sTable->tables[index]->currWidth += sTable->allTypes[ind]->width; 
                    //     sTable->tables[index]->numEntries += 1; 
                    // } 
                
                    // // if (sTable->tables[index]->numEntries == 0) { 
                    // //     sTable->tables[index]->entries = initialiseS(); 
                    // // }
            
                    // // entry* new = makeEntry(child->elem->lex.lexemeStr, ind, sTable->tables[index]->currWidth, child->elem->lineNo); 
                    // // insertS(new, sTable->tables[index]->entries); 
                    // // sTable->tables[index]->currWidth += sTable->allTypes[ind]->width; 
                    // // sTable->tables[index]->numEntries += 1; 
                } 
                else if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_RUID") == 0) { 
                    // child = child->next; 

                    int ind = searchTypes(child->elem->lex.lexemeStr, sTable); 
                    if (ind == -1) { // This record type has not yet been defined or used in the parameters 
                        typeInfo* newT = (typeInfo*) malloc(sizeof(typeInfo)); 
                        int l = strlen(child->elem->lex.lexemeStr); 
                        newT->name = (char*) malloc(sizeof(char) * (l + 1)); 
                        for(int i = 0; i < l; i++){
                            newT->name[i] = child->elem->lex.lexemeStr[i];
                        } 
                        newT->name[l] = '\0'; 
                        newT->ref = -1; 
                        newT->numFields = 0; 
                        newT->numAl = 0; 
                        // newT->fInd = -1; 
                        // newT->typeId; // TypeId not known, could be union or record 
                        insertType(newT, sTable); 
                        ind = sTable->numTypes - 1; 
                    } 

                    child = child->next; 

                    if (child->next->elem->isLeaf == 1 && child->next->elem->curr == 19) { 
                        if (sTable->numEntries == 0) { 
                            sTable->entries = initialiseS(); 
                        } 

                        entry* new = makeEntry(child->elem->lex.lexemeStr, ind, sTable->mainOffset, child->elem->lineNo); 
                        insertS(new, sTable->entries); 
                        sTable->mainOffset += sTable->allTypes[ind]->width; 
                        sTable->numEntries += 1; 
                    } 
                    else { 
                        if (sTable->tables[index]->numEntries == 0) { 
                            sTable->tables[index]->entries = initialiseS(); 
                        }
            
                        entry* new = makeEntry(child->elem->lex.lexemeStr, ind, sTable->tables[index]->currWidth, child->elem->lineNo); 
                        insertS(new, sTable->tables[index]->entries); 
                        sTable->tables[index]->currWidth += sTable->allTypes[ind]->width; 
                        sTable->tables[index]->numEntries += 1; 
                    } 
                
                    // if (sTable->tables[index]->numEntries == 0) { 
                    //     sTable->tables[index]->entries = initialiseS(); 
                    // }

                    // child = child->next; 
                    // entry* new = makeEntry(child->elem->lex.lexemeStr, ind, sTable->tables[index]->currWidth, child->elem->lineNo); 
                    // insertS(new, sTable->tables[index]->entries); 
                    // sTable->tables[index]->currWidth += sTable->allTypes[ind]->width; 
                    // sTable->tables[index]->numEntries += 1; 
                }
                child = child->next; 
            }
        } 
        child1 = child1->next; 
    }
}

void handleF(astNode* root, grammar G, symbolTable* sTable, int index) { 
    
    astNode* child = root->child; 
    
    sTable->tables[index]->function = (fInfo*) malloc(sizeof(fInfo)); 

    while (child != NULL) { 
        if (child->elem->isLeaf == 1 && strcmp(G.terminals[child->elem->curr], "TK_FUNID") == 0) { 
            // printf("done funid \n"); 
            int l = strlen(child->elem->lex.lexemeStr); 
            // printf("done \n"); 
            sTable->tables[index]->function->fId = (char*) malloc(sizeof(char) * (l + 1)); 
            sTable->tables[index]->function->line = child->elem->lineNo;
            // printf("done \n");
            for(int i = 0; i < l; i++){
                sTable->tables[index]->function->fId[i] = child->elem->lex.lexemeStr[i];
            } 
            sTable->tables[index]->function->fId[l] = '\0'; 
            // printf("done '%s' \n", sTable->tables[index]->function->fId); 
            sTable->tables[index]->currWidth = 0; 
            sTable->tables[index]->function->numIn = 0; 
            sTable->tables[index]->function->numOut = 0; 
            sTable->tables[index]->function->inId = -1; 
            sTable->tables[index]->function->outId = -1; 
        } 
        else if (child->elem->isLeaf == 1 && child->elem->curr == 0) { 
            // skip 
            // printf("Epsilon encountered handleF\n"); 
        } 
        else {   
            // if (strcmp(G.nonTerminals[child->elem->curr], "typeDefinitions") == 0) { 
            //     printf("typeDefinitions skipped \n"); 
            //     child = child->next; 
            //     continue;
            // } 
            // if (strcmp(G.nonTerminals[child->elem->curr], "assignmentStmt") == 0) { 
            //     break; 
            // }
            // printf("Handling %s \n", G.nonTerminals[child->elem->curr]); 
            if (child->elem->isLeaf == 0 && strcmp(G.nonTerminals[child->elem->curr], "returnStmt") == 0) { 
                sTable->tables[index]->function->lastLine = child->child->elem->lineNo; 
            }
            handleNT(child, G, sTable, index, 0); 
            // if (strcmp(G.nonTerminals[child->elem->curr], "output_par") == 0) { 
            //     break; 
            // } 
        } 
        child = child->next; 
    }
} 

void handleM(astNode* root, grammar G, symbolTable* sTable, int index) { 

    // printf("In handleM \n"); 
    astNode* child = root->child; 
    sTable->tables[index]->function = (fInfo*) malloc(sizeof(fInfo)); 
    sTable->tables[index]->function->fId = "main"; 
    sTable->tables[index]->function->inId = -1; 
    sTable->tables[index]->function->outId = -1; 
    sTable->tables[index]->function->numIn = 0; 
    sTable->tables[index]->function->numOut = 0; 
    // printf("done \n"); 
    while (child != NULL) { 
        if (child->elem->isLeaf == 1 && child->elem->curr == 0) { 
            // skip 
            // printf("Epsilon encountered handleM\n"); 
        } 
        else { 
            // printf("Handling %s \n", G.nonTerminals[child->elem->curr]); 
            if (child->elem->isLeaf == 0 && strcmp(G.nonTerminals[child->elem->curr], "returnStmt") == 0) { 
                sTable->tables[index]->function->lastLine = child->child->elem->lineNo; 
            }
            handleNT(child, G, sTable, index, 0); 
            // if (strcmp(G.nonTerminals[child->elem->curr], "definetypestmt") == 0) { 
            //     break; 
            // }
        } 
        child = child->next; 
    }
} 

/*
    Constructs the symbol table by traversing the AST
*/
symbolTable* constructST(astNode* root, grammar G) { 

    if (root == NULL) { 
        printf("Empty AST ERROR \n"); 
        return NULL; 
    } 
    symbolTable* sTable; 
    // printf("done %d %d \n", root->elem->curr, findIndex(G.nonTerminals, G.numNonTerminals, "program")); 
    if (root->elem->isLeaf == 0 && root->elem->curr == findIndex(G.nonTerminals, G.numNonTerminals, "program")) { 
        sTable = (symbolTable*)malloc(sizeof(symbolTable)); 
        // printf("done \n");
        sTable->numF = 0; 
        sTable->numTypes = 0; 
        sTable->numEntries = 0; 
        sTable->allTypes = (typeInfo**) malloc(sizeof(typeInfo*)); 
        // printf("done \n");
        typeInfo* tint = (typeInfo*) malloc(sizeof(typeInfo)); 
        tint->isParam = 0; 
        tint->isPrim = 1; 
        tint->numFields = 0; 
        tint->typeId = 0; 
        tint->name = "int"; 
        tint->width = 2; 
        tint->ref = -1; 
        tint->numAl = 0; 
        insertType(tint, sTable); 
        typeInfo* treal = (typeInfo*) malloc(sizeof(typeInfo)); 
        treal->name = "real"; 
        treal->isParam = 0; 
        treal->isPrim = 1; 
        treal->numFields = 0; 
        treal->typeId = 1; 
        treal->width = 4; 
        treal->ref = -1; 
        treal->numAl = 0; 
        insertType(treal, sTable); 
        // sTable->allTypes[0]->isPrim = 1; 
        // sTable->allTypes[0]->typeId = 0; // Int 
        // sTable->allTypes[0]->isPrim = 1; 
        // sTable->allTypes[0]->typeId = 1; // Real 
        // printf("done \n");
    } 
    else { 
        return NULL; 
    } 
    // printf("done \n"); 
    int varInd = findIndex(G.terminals, G.numTerminals, "TK_ID"); 
    int ruInd = findIndex(G.terminals, G.numTerminals, "TK_RUID"); 
    int fieldInd = findIndex(G.terminals, G.numTerminals, "TK_FIELDID"); 
    astNode* child = root->child; 

    while (child != NULL) { 
        if (child->elem->isLeaf == 1 && child->elem->curr == 0) { 
            // printf("Epsilon encountered cst\n"); 
        } 
        else if (child->elem->isLeaf == 1 && (child->elem->curr == varInd || child->elem->curr == ruInd || child->elem->curr == fieldInd)) { 
            printf("Cannot have variable declarations outside functions ERROR \n"); 
            break; 
        } 
        else if (child->elem->isLeaf == 0) { 

            if (strcmp(G.nonTerminals[child->elem->curr], "function") == 0) { 
                // printf("function \n"); 
                sTable->numF += 1; 
                if (sTable->numF == 1) { 
                    sTable->tables = (table**) malloc(sizeof(table*)); 
                } 
                else { 
                    sTable->tables = realloc(sTable->tables, ((sizeof(table*)) * sTable->numF)); 
                } 
                // printf("%d \n", sTable->numF -  1); 
                sTable->tables[sTable->numF - 1] = (table*) malloc(sizeof(table)); 
                sTable->tables[sTable->numF - 1]->currWidth = 0; 
                // printf("done %d \n", sTable->tables[sTable->numF - 1]->currWidth); 
                handleF(child, G, sTable, sTable->numF - 1); 
                // printf("done \n"); 
                // break; 
            }
            else { 
                sTable->numF += 1; 
                if (sTable->numF == 1) { 
                    sTable->tables = (table**) malloc(sizeof(table*)); 
                } 
                else { 
                    sTable->tables = realloc(sTable->tables, ((sizeof(table*)) * sTable->numF)); 
                } 
                // printf("%d \n", sTable->numF -  1); 
                sTable->tables[sTable->numF - 1] = (table*) malloc(sizeof(table)); 
                sTable->tables[sTable->numF - 1]->currWidth = 0; 
                // printf("done %d \n", sTable->tables[sTable->numF - 1]->currWidth); 
                handleM(child, G, sTable, sTable->numF - 1); 
                // printf("done \n"); 
                // break; 
            } 
        } 
        child = child->next; 
    } 

    child = root->child; 
    int count = 0; 
    while (child != NULL) { 
        if (child->elem->isLeaf == 1 && child->elem->curr == 0) { 
            // printf("Epsilon encountered cst\n"); 
        } 
        else if (child->elem->isLeaf == 1 && (child->elem->curr == varInd || child->elem->curr == ruInd || child->elem->curr == fieldInd)) { 
            // printf("Cannot have variable declarations outside ERROR \n"); 
            break; 
        } 
        else if (child->elem->isLeaf == 0) { 

            if (strcmp(G.nonTerminals[child->elem->curr], "function") == 0) { 
                handleDecl(child, G, sTable, count); 
                count += 1; 
            }
            else { 
                handleDecl(child, G, sTable, count); 
                count += 1; 
            } 
        } 
        child = child->next; 
    }
    return sTable; 

} 

// void main() { 

//     // int numEntries = 0; 
//     // int hashSize = 0; 
//     // entry** hashT = initialiseS(); 
//     // hashSize = size; 
//     // entry* new1 = (entry*) malloc(sizeof(entry)); 
//     // // printf("done \n"); 
//     // // entry* new1 = makeEntry("c453", 0, 0, 1); 
    
//     // new1->varName = "c453"; 
//     // new1->offset = 4; 
//     // entry* new2 = (entry*) malloc(sizeof(entry)); 
//     // new2->varName = "kfjuw"; 
//     // new2->offset = 8; 
//     // printf("done \n"); 
//     // initialiseS(hashT); 
//     // printf("done \n"); 
//     // int ins1 = insertSRez(new1, hashT, numEntries, hashSize); 
//     // if (ins1 >= hashSize) { 
//     //     printf("Insert 1 (Doubled): %d \n", ins1 - hashSize); 
//     //     hashSize *= 2; 
//     // } 
//     // else { 
//     //     printf("Insert 1 (Not doubled): %d \n", ins1); 
//     // } 
//     // int ins2 = insertSRez(new2, hashT, numEntries, hashSize); 
//     // if (ins2 >= hashSize) { 
//     //     printf("Insert 1 (Doubled): %d \n", ins2 - hashSize); 
//     //     hashSize *= 2; 
//     // } 
//     // else { 
//     //     printf("Insert 1 (Not doubled): %d \n", ins2); 
//     // } 

//     // printf("%d \n", insertS(new1, hashT)); 
//     // printf("done \n"); 
//     // printf("%d \n", insertS(new2, hashT)); 
//     // int ind = searchS(new1->varName, hashT); 
//     // printf("%d \n", ind); 
//     // printf("%s %d %d %d %d %d \n", hashT[ind]->varName, hashT[ind]->offset, hashT[ind]->present, hashT[ind]->type, hashT[ind]->lineNo, hashT[ind]->width); 

//     char* file; 
//     file = "grammar.txt"; 
//     grammar C; 
//     C = readGrammar(file); 
//     printf("Grammar read and saved \n"); 
//     printf("%d %d %d %d \n", C.totalNumRules, C.allRules[36].numOrs, C.allRules[31].epsilon, C.allRules[28].epsilon); 
    
//     // printf("'%s' %d %d \n", C.nonTerminals[23], C.allRules[23].numOrs, C.allRules[23].epsilon); 
//     // printf("%d %d '%s' '%s' \n", C.allRules[23].RHS[0].symbols[0].symbol, C.allRules[23].RHS[1].symbols[0].symbol, C.nonTerminals[C.allRules[23].RHS[0].symbols[0].symbol], C.terminals[C.allRules[23].RHS[1].symbols[0].symbol]); 

//     // prettyPrintGrammar(C);
    
//     C.ff = ComputeFirstAndFollowSets(C); 
//     printf("First and Follow computed \n"); 
//     printf("\n***** \n"); 
//     // int n = 48; 
//     // printf("C.ff[n].numFirst %d  C.ff[n].numFollow %d \n", C.ff[n].numFirst[0], C.ff[n].numFollow); 
//     // for (int i = 0; i < C.ff[n].numFollow; i++) { 
//     //     printf("%d '%s' \n", C.ff[n].follow[i], C.terminals[C.ff[n].follow[i]]); 
//     // } 
    
//     printf("\n***** \n"); 

//     // int trial = 10; 
//     // // printf("%d %d %d %d %d %d \n", C.allRules[23].numOrs, C.ff[24].numFirst[0], C.ff[24].numFirst[1], C.ff[24].numFirst[2], C.ff[24].numFirst[3], C.ff[24].numFirst[4]); 
//     // printf("C.allRules[trial].numOrs %d ", C.allRules[trial].numOrs); 
//     // for (int i = 0; i < C.allRules[trial].numOrs; i++) { 
//     //     printf("C.ff[trial].numFirst[i] %d ", C.ff[trial].numFirst[i]); 
//     // } 

//     // printRule(C,trial,-1);
    
//     parseTable* T = intializeParseTable(C.numNonTerminals,C.numTerminals);
//     createParseTable(C,C.ff,T);
//     // printParseTable(C,T);
//     printf("Parse Table created \n"); 

//     // printf("%d %d %d '%s' '%s' \n", C.allRules[2].numOrs, C.ff[2].numFirst[0], C.ff[2].numFirst[1], C.terminals[C.ff[2].first[0][0]], C.terminals[C.ff[2].first[1][0]]); 
//     // printf("%d %d %d \n", C.allRules[0].numOrs, C.allRules[0].RHS[0].numSyms, C.allRules[0].RHS[0].symbols[1].type); 
//     // printf("%d %d %d %d %d %d '%s' '%s' '%s' '%s' \n", C.ff[23].numFirst, C.ff[23].numFollow, C.ff[23].follow[0], C.ff[23].follow[1], C.ff[23].follow[2], C.ff[23].follow[3], C.terminals[C.ff[23].follow[0]], C.terminals[C.ff[23].follow[1]], C.terminals[C.ff[23].follow[2]], C.terminals[C.ff[23].follow[3]]); 

//     char* testCaseFile = "./stage 2 (typechecking testcases for expressions)/p3.txt"; 
//     // FILE *fp = fopen("./testcases_stage1/t2.txt","r"); 
//     // initialize();
//     // fp = getStream(fp, 0);
//     // tokenInfo currToken = getNextToken(fp); 
//     // printf("%d '%s' \n", findIndex(C.terminals, C.numTerminals, enumToStringP[currToken.tkn_name]), enumToStringP[currToken.tkn_name]); 
//     treeN rootNode; 
//     int* errorFlag (int*) malloc(sizeof(int)); 
//     *errorFlag = 0; 
//     rootNode = parseInputSourceCode(testCaseFile, C, T, errorFlag); 
//     // printf("%u %d %d %d %d %d \n", &rootNode, rootNode.elem.curr, rootNode.numChild, rootNode.elem.lineNo, rootNode.elem.isLeaf, rootNode.elem.parentNodeSymbolID); 
//     // printf("%u %u \n", rootNode.children[0], rootNode.children[1]); 
//     // printf("%u %u \n", rootNode.children[1]->children[1], rootNode.children[1]->children[1]->children[5]); 
//     // printf("%d '%s' %d '%s' \n", rootNode.children[1]->children[1], C.nonTerminals[rootNode.children[1]->children[1]->elem.curr], rootNode.children[1]->children[1]->children[5], C.terminals[rootNode.children[1]->children[1]->children[5]->elem.curr]); 
//     // printParseTree(&rootNode,"op.txt",C);
//     int *insertPrev = (int *)malloc(sizeof(int));
//     *insertPrev = 0;
//     int *count1 = (int *)malloc(sizeof(int));
//     *count1 = 0;
//     astNode *astroot = mknode(rootNode.elem,C);
//     constructAst(astroot, &rootNode,C,insertPrev,astroot);
//     printf("*************************************************************************************************\n\n");
//     printf("Printing Abstract Syntax Tree\n");
//     printAST(astroot,C,count1);
//     printf("*************************************************************************************************\n\n");
//     // printf("Level 1 printing\n");
//     // printf("Root : isLeaf: %d curr: %d name: %s Line: %d \n",astroot->elem->isLeaf,astroot->elem->curr,astroot->elem->isLeaf?C.terminals[astroot->elem->curr]:C.nonTerminals[astroot->elem->curr],astroot->elem->lineNo);
//     // astNode *curr = astroot->child;
//     // while(curr!=NULL){
//     //     printf("isLeaf: %d curr: %d name:%s Line:%d\n",curr->elem->isLeaf,curr->elem->curr,curr->elem->isLeaf?C.terminals[curr->elem->curr]:C.nonTerminals[curr->elem->curr],curr->elem->lineNo);
//     //     curr = curr->next;
//     // }
//     printf("Root : %d %d %s \n", astroot->elem->isLeaf, astroot->elem->curr, C.nonTerminals[astroot->elem->curr]); 
//     astNode* curr = astroot->child->child; 
//     // astNode* curr = astroot->child->child; 
//     while (curr != NULL) { 
//         printf("%d %d ", curr->elem->isLeaf, curr->elem->curr); 
//         if (curr->elem->isLeaf == 1) { 
//             printf("'%s' \n", C.terminals[curr->elem->curr]); 
//         } 
//         else { 
//             printf("'%s' \n", C.nonTerminals[curr->elem->curr]); 
//         } 
//         curr = curr->next; 
//     } 

//     // printf("%d \n", findIndex(C.terminals, C.numTerminals, "TK_GLOBAL")); 
//     symbolTable* sTable = constructST(astroot, C); 
    
//     // printf("%d \n", sTable->tables[0]->numEntries); 
//     // printf("%d %d \n", sTable->numTypes, sTable->numEntries); 
//     // int ind1 = searchS("b3b444", sTable->entries); 
//     // printf("%d %d %d %s \n", sTable->entries[ind1]->lineNo, sTable->entries[ind1]->offset, sTable->entries[ind1]->type, sTable->entries[ind1]->varName); 
//     // printf("%d %d %d \n", sTable->tables[0]->currWidth, sTable->tables[1]->currWidth, sTable->mainOffset); 
    
//     // printf("%d %d %d %d %d \n", sTable->numF, sTable->numTypes, sTable->tables[0]->currWidth, sTable->tables[0]->numEntries, sTable->tables[0]->function->inId); 
//     // printf("%d %d \n", sTable->allTypes[0]->typeId, sTable->allTypes[1]->typeId); 
//     // printf("%s %s \n", sTable->allTypes[2]->name, sTable->allTypes[3]->name); 
//     // int ind1 = searchS("d5cc34", sTable->allTypes[4]->fields); 
//     // int ind2 = searchS("b5c6", sTable->allTypes[4]->fields); 
//     // printf("%d %d %d \n", sTable->allTypes[4]->numFields, sTable->allTypes[4]->fields[ind1]->type, sTable->allTypes[4]->fields[ind2]->type); 
//     // int ind3 = searchTypes("#two", sTable); 
//     // printf("%s \n", sTable->allTypes[ind3]->name); 
//     // ind1 = searchS("b5b567", sTable->tables[0]->entries); 
//     // ind2 = searchS("b3", sTable->tables[0]->entries); 
//     // ind3 = searchS("d5", sTable->tables[0]->entries); 
//     // printf("%d %d %d %d \n", sTable->tables[0]->numEntries, sTable->tables[0]->entries[ind1]->type, sTable->tables[0]->entries[ind2]->type, sTable->tables[0]->entries[ind3]->type); 
//     // ind1 = searchS("beginpoint", sTable->allTypes[2]->fields); 
//     // ind2 = searchS("endpoint", sTable->allTypes[2]->fields); 
//     // printf("%d \n", sTable->allTypes[3]->width); 
//     // printf("%s %d %s %d %d \n", sTable->allTypes[2]->name, sTable->allTypes[2]->numFields, sTable->allTypes[2]->fields[ind2]->varName, sTable->allTypes[2]->fields[ind2]->type, sTable->allTypes[2]->fields[ind2]->offset); 
    
//     // int ind4 = searchTypes("#two", sTable); 
//     // printf("%d %s \n", sTable->allTypes[ind4]->numAl, sTable->allTypes[ind4]->aliases[0]); 
//     // int ind5 = searchTypes("#point", sTable); 
//     // printf("%d %d \n", ind4, sTable->allTypes[ind5]->ref); 
//     // int ind6 = searchTypes("#variantrecord", sTable); 
//     // printf("%d %d \n", sTable->allTypes[ind6]->width, ind6); 
//     // int ind7 = searchS("b5c6", sTable->allTypes[4]->fields); 
//     // printf("%s %d \n", sTable->allTypes[4]->fields[ind7]->varName, sTable->allTypes[4]->fields[ind7]->type); 
//     // ind7 = searchS("beginpoint", sTable->allTypes[ind4]->fields); 
//     // printf("%s \n", sTable->allTypes[ind4]->fields[ind7]->varName); 
//     // printf("Number of functions: %d \n", sTable->numF); 
//     // int ind8 = searchS("b3b444", sTable->entries); 
//     // printf("%s %d %d \n", sTable->entries[ind8]->varName, sTable->entries[ind8]->type, sTable->entries[ind8]->offset); 
//     // printf("%d %d %d \n", sTable->tables[0]->function->inOrder[0], sTable->tables[0]->function->inOrder[1], sTable->tables[0]->function->inOrder[2]); 
//     // printf("%s %s %s \n", sTable->allTypes[sTable->tables[0]->function->inId]->fields[sTable->tables[0]->function->inOrder[0]]->varName, sTable->allTypes[sTable->tables[0]->function->inId]->fields[sTable->tables[0]->function->inOrder[1]]->varName, sTable->allTypes[sTable->tables[0]->function->inId]->fields[sTable->tables[0]->function->inOrder[2]]->varName); 
//     // printf("First child of function : %d %d %s %s \n", astroot->child->child->child->elem->isLeaf, astroot->child->child->child->elem->curr, C.terminals[astroot->child->child->child->elem->curr], astroot->child->child->child->elem->lex.lexemeStr); 
//     // printf("rule number %d LHS %s RHS %s\n",getRuleNumber(52,1,C),C.nonTerminals[C.allRules[52].LHS],C.terminals[C.allRules[52].RHS[1].symbols[0].symbol]);
// } 