#include "parser.h"
// #include "lookuptable.c" 
// #include "lexerDef.h"
// #include "lexer.h"
// #include "lexer.c" 
// #include "tree.c"

// char* nonTerminals[] = { 
//     "program", "mainFunction", "otherFunctions", "function", "input_par", "output_par", "parameter_list", "dataType", 
//     "primitiveDatatype", "constructedDatatype", "remaining_list", "stmts", "typeDefinitions", "actualOrRedefined", 
//     "typeDefinition", "fieldDefinitions", "fieldDefinition", "fieldtype", "moreFields", "declarations", "declaration", 
//     "global_or_not", "otherStmts", "stmt", "assignmentStmt", "singleOrRecId", "constructedVariable", "oneExpansion", 
//     "moreExpansions", "funCallStmt", "outputParameters", "inputParameters", "iterativeStmt", "conditionalStmt", "elsePart", 
//     "ioStmt", "arithmeticExpression", "expPrime", "term", "termPrime", "factor", "highPrecedenceOperator", 
//     "lowPrecedenceOperators", "booleanExpression", "var", "logicalOp", "relationalOp", "returnStmt", "optionalReturn", 
//     "idList", "more_ids", "definetypestmt", "A"}; 

char* terminals[] = { 
    "eps", "TK_ASSIGNOP","TK_COMMENT","TK_FIELDID","TK_ID","TK_NUM","TK_RNUM","TK_FUNID","TK_RUID","TK_WITH","TK_PARAMETERS","TK_END",
"TK_WHILE","TK_UNION","TK_ENDUNION","TK_DEFINETYPE","TK_AS","TK_TYPE","TK_MAIN","TK_GLOBAL","TK_PARAMETER","TK_LIST","TK_SQL","TK_SQR","TK_INPUT","TK_OUTPUT","TK_INT","TK_REAL",
"TK_COMMA","TK_SEM","TK_COLON","TK_DOT","TK_ENDWHILE","TK_OP","TK_CL","TK_IF","TK_THEN","TK_ENDIF","TK_READ","TK_WRITE","TK_RETURN","TK_PLUS","TK_MINUS","TK_MUL","TK_DIV",
"TK_CALL","TK_RECORD","TK_ENDRECORD","TK_ELSE","TK_AND","TK_OR","TK_NOT","TK_LT","TK_LE","TK_EQ","TK_GT","TK_GE","TK_NE","$"}; 


int numTerminals = 59; 

char* enumToStringP[]={"TK_ASSIGNOP","TK_COMMENT","TK_FIELDID","TK_ID","TK_NUM","TK_RNUM","TK_FUNID","TK_RUID","TK_WITH","TK_PARAMETERS","TK_END",
"TK_WHILE","TK_UNION","TK_ENDUNION","TK_DEFINETYPE","TK_AS","TK_TYPE","TK_MAIN","TK_GLOBAL","TK_PARAMETER","TK_LIST","TK_SQL","TK_SQR","TK_INPUT","TK_OUTPUT","TK_INT","TK_REAL",
"TK_COMMA","TK_SEM","TK_COLON","TK_DOT","TK_ENDWHILE","TK_OP","TK_CL","TK_IF","TK_THEN","TK_ENDIF","TK_READ","TK_WRITE","TK_RETURN","TK_PLUS","TK_MINUS","TK_MUL","TK_DIV",
"TK_CALL","TK_RECORD","TK_ENDRECORD","TK_ELSE","TK_AND","TK_OR","TK_NOT","TK_LT","TK_LE","TK_EQ","TK_GT","TK_GE","TK_NE","ERROR","TK_EOF"};


grammar readGrammar(char* file) { 
    
    grammar currGrammar; 

    char** nonTerminals = (char**) malloc(sizeof(char*) * 1); 
    nonTerminals[0] = "program"; 
    int numNonTerminals = 1; 

    
    FILE* f = fopen(file, "r");
    int ruleID = 0; 

    while (!feof(f)) { 
        char s[300]; 
        // Read rule
        fgets(s, 300, f); 
        // printf("%d %s %d ", ruleID, s, strlen(s)); 
        for (int i = 0; s[i] != '\0'; i++) { 
            if (s[i] == 13 || s[i] == 10) { 
                s[i] = '\0'; 
            }
        } 
        // printf("%s \n", s); 
        
        char* point = strtok(s, " "); 
        
       
        if (ruleID != 0) { 
            // printf("%s ", point); 
            int l = strlen(point); 
            char* lhs = (char*)malloc(sizeof(char) * (l - 1)); 
            strncpy(lhs, point + 1, l - 2); 
            lhs[l - 2] = '\0'; 
            nonTerminals = realloc(nonTerminals, ((sizeof(char*)) * (numNonTerminals + 1))); 
            nonTerminals[numNonTerminals] = lhs; 
            numNonTerminals += 1; 
        } 
        // printf("%d \n", numNonTerminals); 
        ruleID += 1; 
    } 

    currGrammar.terminals = terminals; 
    currGrammar.nonTerminals = nonTerminals; 
    currGrammar.allRules = (rule*)malloc(sizeof(rule) * (ruleID)); 
    currGrammar.totalNumRules = ruleID; 
    // printf("numNonTerminals %d, numTerminals %d, ruleID %d \n*** \n", numNonTerminals, numTerminals, ruleID); 
    currGrammar.numNonTerminals = numNonTerminals; 
    currGrammar.numTerminals = numTerminals; 

    fclose(f); 
    ruleID = 0; 
    FILE* fN = fopen(file, "r"); 

    while(!feof(fN)) { 
        char s[300]; 
        fgets(s, 300, fN); 

        int countOr = 0; 
        for (int i = 0; s[i] != '\0'; i++) { 
            if (s[i] == '|') { 
                countOr += 1; 
            } 
            if (s[i] == 13 || s[i] == 10) { 
                s[i] = '\0'; 
            }
        } 
        // printf("%s \n", s); 

        char* point = strtok(s, " "); 
        
        int l = strlen(point); 
        char* lhs = (char*)malloc(sizeof(char) * (l - 1)); 
        strncpy(lhs, point + 1, l - 2); 
        lhs[l - 2] = '\0'; 
        // printf("%s ", lhs); 
        
        rule currRule; 
        currRule.LHS = findIndex(nonTerminals, numNonTerminals, lhs); 
        // printf("%d\n", currRule.LHS); 
        point = strtok(NULL, " "); 
        point = strtok(NULL, " ");  
        // printf("%d\n", currRule.LHS); 
        
        currRule.numOrs = countOr + 1; 
        int rhsInd = 0; 
        currRule.RHS = (rhsSingular*) malloc(sizeof(rhsSingular) * (countOr + 1)); 
        currRule.epsilon = 0; 
        int currLength = 0; 
        varSymbol* currSymbols;  
        while (point != NULL) { 
            
            // printf("%s ", point); 

            if (point[0] == '|') { 
                currRule.RHS[rhsInd].numSyms = currLength; 
                currRule.RHS[rhsInd].symbols = currSymbols; 
                currLength = 0; 
                point = strtok(NULL, " "); 
                rhsInd += 1; 
                continue; 
            } 
            if (point[0] == '<' || point[0] == 'T' || strcmp(point, "∈") == 0) { 
                currLength += 1; 
                if (currLength == 1) { 
                    currSymbols = (varSymbol*)malloc(sizeof(varSymbol) * 1); 
                } 
                else { 
                    currSymbols = realloc(currSymbols, ((sizeof(varSymbol)) * currLength));
                }
                if (point[0] == '<') { 
                    int l = strlen(point); 
                    char* sym = (char*)malloc(sizeof(char) * (l - 1)); 
                    strncpy(sym, point + 1, l - 2); 
                    sym[l - 2] = '\0'; 
                    // printf("%d %c %c %s ", l, point[0], point[l - 1], sym); 
                    int id = findIndex(nonTerminals, numNonTerminals, sym); 
                    currSymbols[currLength - 1].type = 0; 
                    currSymbols[currLength - 1].symbol = id; 
                } 
                else if (point[0] == 'T') { 
                    int id = findIndex(terminals, numTerminals, point); 
                    currSymbols[currLength - 1].type = 1; 
                    currSymbols[currLength - 1].symbol = id; 
                } 
                else if (strcmp(point, "∈") == 0) { 
                    int id = findIndex(terminals, numTerminals, "eps"); 
                    currSymbols[currLength - 1].type = 1; 
                    currSymbols[currLength - 1].symbol = id; 
                    currRule.epsilon = 1; 
                }
            } 
            point = strtok(NULL, " "); 
        } 
        currRule.RHS[rhsInd].numSyms = currLength; 
        currRule.RHS[rhsInd].symbols = currSymbols; 
        currRule.ruleNumber = ruleID + 1; 
        // printf("*\n"); 
        currGrammar.allRules[ruleID] = currRule; 
        ruleID += 1; 
    } 
    // printf("%d \n", ruleID); 
    // printf("%d %d %d \n", currGrammar.allRules[0].LHS, currGrammar.allRules[0].numOrs, currGrammar.allRules[0].ruleNumber); 
    // printf("%d %d %d %d %d", currGrammar.allRules[0].RHS[0].numSyms, currGrammar.allRules[0].RHS[0].symbols[0].type, currGrammar.allRules[0].RHS[0].symbols[1].type, currGrammar.allRules[0].RHS[0].symbols[0].symbol, currGrammar.allRules[0].RHS[0].symbols[1].symbol); 
    return currGrammar; 
} 

void IndividualFirst(grammar G, FirstAndFollow* ff, int ind) { 

    // For each non-terminal 
    // 1. 
    // Add all first terminals on the RHS to 'first', imcluding epsilon 
    // 2. 
    // If the first symbol is a non-terminal, recursively find 'first' of this terminal and add its 
    // elements to the required 'first' array 
    // 3. 
    // If the first symbol is a non-terminal and it yields epsilon, find 'first' of this terminal 
    // and add all elements except epsilon to the required 'first' array. Then find 'first' of 
    // the next symbol if it is a non-terminal, if it is a terminal, just add this terminal to 
    // the required 'first' array 


    int numOrs = G.allRules[ind].numOrs; // Number of rules in RHS 
    // printf("%d ", numOrs); 
    for (int i = 0; i < numOrs; i++) { // This block adds only all the terminals to the 'first' array
        if (G.allRules[ind].RHS[i].symbols[0].type == 1) { 
            if (ff[ind].numFirst[i] == 0) { 
                ff[ind].first[i] = (int*) malloc(sizeof(int) * 1); 
            } 
            else { 
                ff[ind].first[i] = (int*) realloc(ff[ind].first[i], sizeof(int) * (ff[ind].numFirst[i] + 1)); 
            }
            ff[ind].first[i][ff[ind].numFirst[i]] = G.allRules[ind].RHS[i].symbols[0].symbol; 
            ff[ind].numFirst[i] += 1; 
        } 
    } 
    // printf("%d ", ff[ind].numFirst); 

    // Okay! 

    for (int i = 0; i < numOrs; i++) { // This block deals with all the non-terminals on the RHS 
        if (G.allRules[ind].RHS[i].symbols[0].type == 0) { 
            int ind1 = G.allRules[ind].RHS[i].symbols[0].symbol; 
            // printf("%d ", ind1); 
            if (ff[ind1].numFirst[0] == 0) { // If 'first' of this non-terminal has not already been computed 
                IndividualFirst(G, ff, ind1); 
            } 
            for (int m = 0; m < G.allRules[ind1].numOrs; m++) { 
                for (int j = 0; j < ff[ind1].numFirst[m]; j++) { 
                    if (ff[ind1].first[m][j] != 0) { // Epsilon 
                        if (isInArr(ff[ind].first[i], ff[ind1].first[m][j], ff[ind].numFirst[i]) != 1) { // If the elements of 'first' of this non-terminal are not already in required array 
                            ff[ind].first[i] = (int*) realloc(ff[ind].first[i], sizeof(int) * (ff[ind].numFirst[i] + 1)); 
                            ff[ind].first[i][ff[ind].numFirst[i]] = ff[ind1].first[m][j]; 
                            ff[ind].numFirst[i] += 1; 
                        }
                    }
                } 
            }
            
            if (G.allRules[ind1].epsilon == 1) { // If this non-terminal yields epsilon 
                int k = 0; 
                while (k < G.allRules[ind].RHS[i].numSyms) { 
                    int ind2 = G.allRules[ind].RHS[i].symbols[k].symbol; // Next symbol in current rule 
                    if (ind2 == ind) { 
                        k += 1; 
                        continue; 
                    }
                    if (G.allRules[ind].RHS[i].symbols[k].type == 1) { // Next symbol is a terminal 
                        if (isInArr(ff[ind].first[i], ind2, ff[ind].numFirst[i]) != 1) { 
                            ff[ind].first[i] = (int*) realloc(ff[ind].first[i], sizeof(int) * (ff[ind].numFirst[i] + 1)); 
                            ff[ind].first[i][ff[ind].numFirst[i]] = ind2; 
                            ff[ind].numFirst[i] += 1; 
                        } 
                        break; 
                    } 
                    else { // Next symbol is a non-terminal 
                        if (ff[ind2].numFirst[0] == 0) { // If the next symbol's 'first' has not been computed 
                            IndividualFirst(G, ff, ind2); 
                        } 
                        for (int m = 0; m < G.allRules[ind2].numOrs; m++) { 
                            for (int j = 0; j < ff[ind2].numFirst[m]; j++) { 
                                if (ff[ind2].first[m][j] != 0) { // Epsilon 
                                    if (isInArr(ff[ind].first[i], ff[ind2].first[m][j], ff[ind].numFirst[i]) != 1) { // If the elements of 'first' of this non-terminal are not already in required array 
                                        ff[ind].first[i] = (int*) realloc(ff[ind].first[i], sizeof(int) * (ff[ind].numFirst[i] + 1)); 
                                        ff[ind].first[i][ff[ind].numFirst[i]] = ff[ind2].first[m][j]; 
                                        ff[ind].numFirst[i] += 1; 
                                    }
                                }
                            } 
                        }
                        

                        if (G.allRules[ind2].epsilon != 1) { // If this non-terminal does not yield epsilon, end the process 
                            break;                           
                        } 
                    } 
                    k += 1; 
                } 
                // if (k == G.allRules[ind].RHS[i].numSyms) { // Last symbol in the rule is a non-terminal and it also yields epsilon, add espilon to the required 'first' list 
                    
                //     int ind2 = G.allRules[ind].RHS[i].symbols[k - 1].symbol; 
                //     if (ff[ind2].numFollow == 0) { 
                //         IndividualFollow(G, ff, ind2); 
                //     } 
                //     for (int j = 0; j < ff[ind2].numFollow; j++) { 
                //         if (isInArr(ff[ind].first, ff[ind2].follow[j], ff[ind].numFirst) != 1) { 
                //             ff[ind].first = (int*) realloc(ff[ind].first, sizeof(int) * (ff[ind].numFirst + 1)); 
                //             ff[ind].first[ff[ind].numFirst] = ff[ind2].follow[j]; 
                //             ff[ind].numFirst += 1; 
                //         } 
                //     } 
                // } 
            }
        }
    } 
} 

void IndividualFollow(grammar G, FirstAndFollow* ff, int ind, int prev) { 

    // Check all rules and subrules where this non-terminal occurs (in reverse order) 
    // 1. 
    // If the current non-terminal is the start symbol i.e. <program> with nonTerminal index 0, 
    // add $ to its follow 
    // 2. 
    // If the next symbol is a terminal, add to 'follow' list 
    // Eg: For <A> : <definetypestmt> ===> TK_DEFINETYPE <A> TK_RUID TK_AS TK_RUID
    // 3. 
    // If the next symbol is a non-terminal, add its first to the required 'follow' list. If this 'first' 
    // has epsilon, add 'first' of next symbol in the rule and so on 
    // Eg: For <otherFunctions> : <program> ===> <otherFunctions> <mainFunction>
    // 4. 
    // If there is no next symbol i.e. it is the last symbol in the rule, find rule's corresponding 
    // non-terminal's 'follow' and add that to the required 'follow' 
    // Eg: For <expPrime> : <arithmeticExpression> ===> <term> <expPrime> 
    // 5. 
    // If there is no next symbol i.e. it is the last symbol in the rule, find rule's corresponding 
    // non-terminal's 'follow'. While finding this next 'follow', if this non-terminal is the last 
    // symbol in a rule for the first non-terminal, skip the rule 
    // Eg: For <more_ids> : <idList> ===> TK_ID <more_ids> 
    // and <more_ids> ===> TK_COMMA <idList> | ∈ 
    // 6. 
    // If there is no next symbol i.e. it is the last symbol in the rule, and the current rule's 
    // corresponding non-terminal is the same (right recursion case), do nothing 
    // Eg: For <otherStmts> : <otherStmts> ===> <stmt> <otherStmts> | ∈ 

    if (ind == 0) { 
        ff[ind].follow = (int*) malloc(sizeof(int) * 1); 
        ff[ind].follow[0] = G.numTerminals - 1; 
        ff[ind].numFollow = 1; 
    }
    for (int i = G.totalNumRules - 1; i >= 0; i--) { 
        // printf("%d \n", i); 
        for (int j = 0; j < G.allRules[i].numOrs; j++) { 
            for (int k = 0; k < G.allRules[i].RHS[j].numSyms; k++) { 
                if (k == G.allRules[i].RHS[j].numSyms - 1) { 
                    if (G.allRules[i].RHS[j].symbols[k].type == 0 && G.allRules[i].RHS[j].symbols[k].symbol == ind) { 
                        // printf("'%s' %d %d %d \n", G.nonTerminals[i], i, ind, k); 
                        if (i != ind) { 
                            // printf("i and ind are diff "); 
                            if (prev == i) { 
                                break; 
                            }
                            if (ff[i].numFollow == 0) { 
                                IndividualFollow(G, ff, i, ind); 
                            } 
                            for (int j = 0; j < ff[i].numFollow; j++) { 
                                if (isInArr(ff[ind].follow, ff[i].follow[j], ff[ind].numFollow) != 1) { // If the elements of 'first' of this non-terminal are not already in required array 
                                    ff[ind].follow = (int*) realloc(ff[ind].follow, sizeof(int) * (ff[ind].numFollow + 1)); 
                                    ff[ind].follow[ff[ind].numFollow] = ff[i].follow[j]; 
                                    ff[ind].numFollow += 1; 
                                } 
                            } 
                        }
                    } 
                    continue; 
                } 
                if (G.allRules[i].RHS[j].symbols[k].type == 0 && G.allRules[i].RHS[j].symbols[k].symbol == ind) { 
                    // printf("'%s'\n ", G.nonTerminals[i]); 
                    if (G.allRules[i].RHS[j].symbols[k + 1].type == 1) { // Next symbol is a terminal i.e. add to 'follow' and leave this for loop 
                        // printf("'%s' \n", G.terminals[G.allRules[i].RHS[j].symbols[k + 1].symbol]); 
                        if (ff[ind].numFollow == 0) { 
                            ff[ind].follow = (int*) malloc(sizeof(int) * 1); 
                            ff[ind].follow[ff[ind].numFollow] = G.allRules[i].RHS[j].symbols[k + 1].symbol; 
                            ff[ind].numFollow += 1; 
                        } 
                        else { 
                            if (isInArr(ff[ind].follow, G.allRules[i].RHS[j].symbols[k + 1].symbol, ff[ind].numFollow) != 1) { // If the elements of 'first' of this non-terminal are not already in required array 
                                // printf("%d \n", ff[ind].numFollow); 
                                ff[ind].follow = (int*) realloc(ff[ind].follow, sizeof(int) * (ff[ind].numFollow + 1)); 
                                ff[ind].follow[ff[ind].numFollow] = G.allRules[i].RHS[j].symbols[k + 1].symbol; 
                                ff[ind].numFollow += 1; 
                            } 
                        } 
                        continue; 
                    } 
                    else { 
                        int ind1 = G.allRules[i].RHS[j].symbols[k + 1].symbol; 
                        // printf("'%s' %d \n", G.nonTerminals[ind1], ff[ind1].numFirst); 
                        if (ff[ind1].numFirst[0] == 0) { 
                            IndividualFirst(G, ff, ind1); 
                        } 
                        for (int m = 0; m < G.allRules[ind1].numOrs; m++) { 
                            for (int j = 0; j < ff[ind1].numFirst[m]; j++) { 
                                // printf("%d \n", ff[ind1].first[j]); 
                                if (ff[ind1].first[m][j] != 0) { // Epsilon 
                                    if (ff[ind].numFollow == 0) { 
                                        ff[ind].follow = (int*) malloc(sizeof(int) * 1); 
                                        ff[ind].follow[ff[ind].numFollow] = ff[ind1].first[m][j]; 
                                        ff[ind].numFollow += 1; 
                                    } 
                                    else { 
                                        if (isInArr(ff[ind].follow, ff[ind1].first[m][j], ff[ind].numFollow) != 1) { // If the elements of 'first' of this non-terminal are not already in required array 
                                            ff[ind].follow = (int*) realloc(ff[ind].follow, sizeof(int) * (ff[ind].numFollow + 1)); 
                                            ff[ind].follow[ff[ind].numFollow] = ff[ind1].first[m][j]; 
                                            ff[ind].numFollow += 1; 
                                        }
                                    } 
                                }
                            }
                        }
                        
                         
                        // printf("%d %d %d \n", ff[ind].numFollow, G.allRules[ind1].epsilon, k + 2); 
                        if (G.allRules[ind1].epsilon != 1) { 
                            continue; 
                        } 
                        else { 
                            int m = k + 2; 
                            while (m < G.allRules[i].RHS[j].numSyms) { 
                                int ind2 = G.allRules[i].RHS[j].symbols[m].symbol; 
                                // printf("%d %d '%s' %d %d \n", m, ind2, G.terminals[ind2], ff[ind].numFollow, G.allRules[i].RHS[j].symbols[m].type); 
                                if (G.allRules[i].RHS[j].symbols[m].type == 1) { // Next symbol is a terminal 
                                    if (isInArr(ff[ind].follow, ind2, ff[ind].numFollow) != 1) { 
                                        ff[ind].follow = (int*) realloc(ff[ind].follow, sizeof(int) * (ff[ind].numFollow + 1)); 
                                        ff[ind].follow[ff[ind].numFollow] = ind2; 
                                        ff[ind].numFollow++; 
                                        // printf("%d \n", ff[ind].numFollow); 
                                    } 
                                    break; 
                                } 
                                else { // Next symbol is a non-terminal 
                                    if (ff[ind2].numFirst[0] == 0) { // If the next symbol's 'first' has not been computed 
                                        IndividualFirst(G, ff, ind2); 
                                    } 
                                    for (int m = 0; m < G.allRules[ind2].numOrs; m++) { 
                                        for (int j = 0; j < ff[ind2].numFirst[m]; j++) { 
                                            if (ff[ind2].first[m][j] != 0) { // Epsilon 
                                                if (isInArr(ff[ind].follow, ff[ind2].first[m][j], ff[ind].numFollow) != 1) { // If the elements of 'first' of this non-terminal are not already in required array 
                                                    ff[ind].follow = (int*) realloc(ff[ind].follow, sizeof(int) * (ff[ind].numFollow + 1)); 
                                                    ff[ind].follow[ff[ind].numFollow] = ff[ind2].first[m][j]; 
                                                    ff[ind].numFollow += 1; 
                                                }
                                            }
                                        } 
                                    }
                                    
                                    
                                    if (G.allRules[ind2].epsilon != 1) { // If this non-terminal does not yield epsilon, end the process 
                                        break;                           
                                    } 
                                } 
                                m += 1; 
                            } 
                            if (m == G.allRules[i].RHS[j].numSyms) { 
                                if (i != ind) { 
                                    if (ff[i].numFollow == 0) { 
                                        IndividualFollow(G, ff, i, -1); 
                                    } 
                                    for (int j = 0; j < ff[i].numFollow; j++) { 
                                        if (isInArr(ff[ind].follow, ff[i].follow[j], ff[ind].numFollow) != 1) { // If the elements of 'first' of this non-terminal are not already in required array 
                                            ff[ind].follow = (int*) realloc(ff[ind].follow, sizeof(int) * (ff[ind].numFollow + 1)); 
                                            ff[ind].follow[ff[ind].numFollow] = ff[i].follow[j]; 
                                            ff[ind].numFollow += 1; 
                                        } 
                                    } 
                                }
                            } 
                        }
                    }
                }
            }
        }
    }
}

FirstAndFollow* ComputeFirstAndFollowSets (grammar G) { 

    FirstAndFollow* ff = (FirstAndFollow*) malloc(sizeof(FirstAndFollow) * G.numNonTerminals); 
    for (int i = 0; i < G.numNonTerminals; i++) { 
        int numOrs = G.allRules[i].numOrs; 
        ff[i].numFirst = (int*) malloc(sizeof(int) * numOrs); 
        for (int j = 0; j < numOrs; j++) { 
            ff[i].numFirst[j] = 0; 
        }
        ff[i].first = (int**) malloc(sizeof(int*) * numOrs); 
        ff[i].numFollow = 0; 
    } 

    // IndividualFollow(G, ff, 51, -1); 

    for (int i = G.numNonTerminals - 1; i >= 0; i--) { 
        if (ff[i].numFirst[0] == 0) { 
            IndividualFirst(G, ff, i); 
        } 
        if (ff[i].numFollow == 0) { 
            IndividualFollow(G, ff, i, -1); 
        }
    }

    return ff; 
} 

// print the parse table cells which have error = 0
void printParseTable(grammar G,parseTable* T){
    int tot=0;
    
    for(int i=0; i<G.numNonTerminals;i++){
        for (int j = 0; j < G.numTerminals; j++){
            if (T->cells[i][j].error==0){
                // printf("terminal: %s",G.terminals[j]);
                tot++;
                printRule(G, T->cells[i][j].ruleInd, T->cells[i][j].rhsInd);
            }
        }
    }
}

// Make a parse table, do proper initializations and return pointer to the table
parseTable* intializeParseTable( int numNonTerminals, int numTerminals ){
    
    parseTableCell cells[numNonTerminals][numTerminals];

    parseTable* T = (parseTable*)malloc((sizeof(parseTable)));

    T->cells = (parseTableCell **)malloc(sizeof(parseTableCell *) * numNonTerminals);
    
    for (int i = 0; i < numNonTerminals; i++) {
        T->cells[i] = (parseTableCell *)malloc(sizeof(parseTableCell) * numTerminals);
    }

    for( int i=0; i < numNonTerminals; i++ ){
        for(int j = 0; j < numTerminals; j++){
            T->cells[i][j].error = 1;
            T->cells[i][j].ruleInd = -1;
            T->cells[i][j].rhsInd = -1;
        }
    }

    return T;
}

void createParseTable(grammar G, FirstAndFollow* ff, parseTable* T){
    // For each production A -> RHS of the grammar
    //   1. For each terminal a in First(RHS), add A -> RHS to Table[A][a]
    //   2. If eps is in First(RHS), then for each terminal b ( and "$" if applicable) in Follow(A), add A -> RHS to Table[A][b].
    
    int numNonTerminals = G.numNonTerminals, numTerminals = G.numTerminals;
    int index;

    // for each grammar rule
    for( int i=0; i < G.totalNumRules; i++ ){
        // for each indivisual production rule
        for(int j = 0; j < G.allRules[i].numOrs; j++){
            // for each terminal in first(alpha)
            for( int terminal_num = 0; terminal_num < G.ff[i].numFirst[j]; terminal_num++ ){
                // assign the production rule details
                index = G.ff[i].first[j][terminal_num];
                if ( index != 0 ){   
                    T->cells[i][index].error = 0;
                    T->cells[i][index].ruleInd = i;
                    T->cells[i][index].rhsInd = j;
                }else{
                    // for each follow terminal, add the prod rule
                    for( int follow_terminal_num = 0; follow_terminal_num < G.ff[i].numFollow; follow_terminal_num++ ){
                        index = G.ff[i].follow[follow_terminal_num]; 
                        T->cells[i][index].error = 0;
                        T->cells[i][index].ruleInd = i;
                        T->cells[i][index].rhsInd = j;
                    }
                }
            }
        }
    }
// for each grammar rule
    for( int i=0; i < G.totalNumRules; i++ ){
        // for each indivisual production rule
        for(int j = 0; j < G.allRules[i].numOrs; j++){
            // for each terminal in first(alpha)
            for( int follow_terminal_num = 0; follow_terminal_num < G.ff[i].numFollow; follow_terminal_num++ ){
                // assign the production rule details
                index = G.ff[i].follow[follow_terminal_num]; 
                if ( T->cells[i][index].error == 1 ){   
                    T->cells[i][index].error = 2;
                }
            }
        }
    }
} 


treeN parseInputSourceCode(char* testCaseFile, grammar G, parseTable* T, int*errorFlag) { 
    
    printf("Parsing input source code...\n");
    FILE *fp = fopen(testCaseFile,"r");
    initialize();
    initializeTwinBuffer();
    fp = getStream(fp, 0);
    
    varSymbol* stack; 
    stack = (varSymbol*) malloc(sizeof(varSymbol) * 2); 
    stack[0].type = 1; 
    stack[0].symbol = G.numTerminals - 1; // $
    stack[1].type = 0; 
    stack[1].symbol = 0; // Start symbol 

    treeN** pointers = (treeN**) malloc(sizeof(treeN*) * 2); 
    pointers[0] = NULL; 
    node rootNode = createEl(-1, -1, 0, 0,  0); 
    treeN root = createNode(rootNode,G); 
    pointers[1] = &root; 
    // printf("root %u %u %u %d %d \n", &root, pointers[1], pointers[0], rootNode.curr, pointers[1]->elem.curr); 

    int stackLen = 2; 
    int stackPointer = 1; 
    tokenInfo currToken = getNextToken(fp); 
    // printf("curr token %s\n",currToken.value.str);
    // printf("before while %u %u %u %d %d \n", &root, pointers[1], pointers[0], rootNode.curr, pointers[1]->elem.curr); 

    // printf("%d %d \n", root.numChild, pointers[1]->numChild); 
    // node child1 = createEl(-1, pointers[1]->elem.curr, 1, 0); 
    // node child2 = createEl(-1, pointers[1]->elem.curr, 2, 0); 
    // treeN new1 = createNode(child1); 
    // treeN new2 = createNode(child2); 
    // printf("first \n"); 
    // addChild(pointers[1], &new1); 
    // printf("second \n"); 
    // addChild(pointers[1], &new2); 
    // printf("%d %u %d \n", pointers[1]->numChild, pointers[1]->children[0], pointers[1]->children[0]->elem.curr); 
    // printf("%u %u %d \n", pointers[1]->children[0], pointers[1]->children[1], pointers[1]->children[1]->elem.curr); 
    // printf("%u %u %d \n", root.children[0], root.children[1], root.children[1]->elem.curr); 

    while(1) { 
        // printf("'%s' \n", enumToStringP[currToken.tkn_name]); 
        if (currToken.tkn_name == ERROR) { 
            printf("Line No: %d - ERROR: Lexical error at %s \n",currToken.line,currToken.value.str); 
            *errorFlag = 1;
            currToken = getNextToken(fp);
            // printf("curr token %s\n",enumToString[currToken.tkn_name]);
           
            
            stackPointer--;
            continue;
            // currToken = getNextToken(fp);
            // continue;
            // break; 
            // Lexical error
        } 
        if (currToken.tkn_name == TK_EOF) { 
            if (stackPointer == 0) { 
                printf("Parsing completed.\n\n");
                break; 
            } 
            else { 
                int flag = 0; 
                while (stackPointer != 0) { 
                    if (stack[stackPointer].type == 1) { 
                        printf("ERROR: Top element of stack is a terminal but EOF has been reached\n"); 
                        *errorFlag = 1;
                        flag = 1; 
                        break; 
                    } 
                    if(T->cells[stack[stackPointer].symbol][G.numTerminals - 1].error == 1) { 
                        printf("ERROR: ParseTable[X, a] is blank\n"); 
                        *errorFlag = 1;
                        // printf("'%s' 'TK_EOF' \n", G.nonTerminals[stack[stackPointer].symbol]); 
                        flag = 1; 
                        break;
                    } 
                    else { 
                        // printf("'%s' 'TK_EOF' \n", G.nonTerminals[stack[stackPointer].symbol]); 
                        int ruleInd = T->cells[stack[stackPointer].symbol][G.numTerminals - 1].ruleInd; 
                        int rhsInd = T->cells[stack[stackPointer].symbol][G.numTerminals - 1].rhsInd; 
                        // stackPointer += 1; 
                        // printf("%d %d %d %d \n", ruleInd, rhsInd, stackPointer, G.allRules[ruleInd].RHS[rhsInd].numSyms); 
                        if (G.allRules[ruleInd].RHS[rhsInd].symbols[0].type == 1 && G.allRules[ruleInd].RHS[rhsInd].symbols[0].symbol == 0) { 
                            node currNode = createEl(-1, stack[stackPointer].symbol, 0, 1, 0); 
                            treeN* eps = (treeN*) malloc(sizeof(treeN)); 
                            *eps = createNode(currNode,G); 
                            addChild(pointers[stackPointer], eps); 
                            printf("Rule is epsilon, pop last non-terminal off the stack \n"); 
                            stackPointer -= 1; 
                        } 
                        else { 
                            int parentSymbol = pointers[stackPointer]->elem.curr; 
                            treeN* parent = pointers[stackPointer]; 
                            for (int i = G.allRules[ruleInd].RHS[rhsInd].numSyms - 1; i >= 0; i--) { 
                                if(stackPointer == stackLen) { 
                                    stack = (varSymbol*) realloc(stack, sizeof(varSymbol) * (stackLen + 1)); 
                                    pointers = (treeN**) realloc(pointers, sizeof(treeN*) * (stackLen + 1)); 
                                    stackLen += 1; 
                                }
                                stack[stackPointer].type = G.allRules[ruleInd].RHS[rhsInd].symbols[i].type; 
                                stack[stackPointer].symbol = G.allRules[ruleInd].RHS[rhsInd].symbols[i].symbol; 
                    
                                node currNode; 
                                if (stack[stackPointer].type == 1) { 
                                    currNode = createEl(-1, parentSymbol, stack[stackPointer].symbol, 1, 0); 
                                } 
                                else { 
                                    currNode = createEl(-1, parentSymbol, stack[stackPointer].symbol, 0, 0); 
                                } 
                                treeN* new = (treeN*) malloc(sizeof(treeN)); 
                                *new = createNode(currNode,G); 
                                addChild(parent, new); 
                                pointers[stackPointer] = new; 
                    
                                if (i != 0) { 
                                    stackPointer += 1; 
                                } 
                            } 
                        } 
                        // if (stack[stackPointer].type == 0) { 
                        //     printf("'%s' \n", G.nonTerminals[stack[stackPointer].symbol]); 
                        // } 
                        // else { 
                        //     printf("'%s' \n", G.terminals[stack[stackPointer].symbol]); 
                        // }
                    } 
                } 
                if (flag == 1) { 
                    break; 
                } 
                else { 
                    printf("Parsing completed.\n\n"); 
                    break; 
                }
            }
        }
        
        
        if (stackPointer == 0 && currToken.tkn_name != TK_EOF) { 
            printf("Line No: %d - ERROR: Start symbol popped from stack but end of file not reached \n",currToken.line); 
            *errorFlag = 1;
            break; 
        } 

        int tokenID = findIndex(G.terminals, G.numTerminals, enumToStringP[currToken.tkn_name]); 
        if (stack[stackPointer].type == 1 && stack[stackPointer].symbol == tokenID) { 
            printf("Terminals on top of stack and at lookahead pointer (%s) match at line number %d\n",G.terminals[tokenID],currToken.line); 
            // printf("%d '%s' '%s' \n", stackPointer, G.terminals[stack[stackPointer].symbol], G.terminals[tokenID]); 
            
            pointers[stackPointer]->elem.lineNo = currToken.line; 
            if (currToken.tkn_name == TK_NUM) { 
                pointers[stackPointer]->elem.lex.numVal = currToken.value.num; 
            } 
            else if (currToken.tkn_name == TK_RNUM) { 
                pointers[stackPointer]->elem.lex.rVal = currToken.value.rnum.v; 
            } 
            else { 
                pointers[stackPointer]->elem.lex.lexemeStr = currToken.value.str; 
            }
            
            stackPointer -= 1; 
            currToken = getNextToken(fp);
            // printf("curr token %s\n",enumToString[currToken.tkn_name]);
        } 
        else if(stack[stackPointer].type == 1 && stack[stackPointer].symbol != tokenID) { 
            printf("Line No: %d - ERROR: Top of stack is %s and lookahead is %s : Terminals do not match\n",currToken.line,G.terminals[stack[stackPointer].symbol], G.terminals[tokenID]); 
            // printf("%d '%s' '%s' \n", stackPointer, G.terminals[stack[stackPointer].symbol], G.terminals[tokenID]); 
            *errorFlag = 1;
            
            // Ignore the terminal at lookahead
            currToken = getNextToken(fp);
            // printf("curr token %s\n",enumToString[currToken.tkn_name]);
            // break; 
            // Terminals do not match ERROR 
            // Add Panic Mode recovery code 
        } 
        else if(stack[stackPointer].type == 0 && T->cells[stack[stackPointer].symbol][tokenID].error != 0) { 
            printf("Line No: %d - ERROR: Top of stack is %s(X) and lookahead is %s(a) : ParseTable[X, a] is blank ERROR \n", currToken.line,G.nonTerminals[stack[stackPointer].symbol], G.terminals[tokenID]);      // printf("'%s' '%s' \n", G.nonTerminals[stack[stackPointer].symbol], G.terminals[tokenID]); 
            // printf("'%s' '%s' \n", G.nonTerminals[stack[stackPointer].symbol], G.terminals[tokenID]); 
            *errorFlag = 1;

            // move lookahead till 
            if(!isInArr(G.ff[stack[stackPointer].symbol].follow,tokenID,G.ff[stack[stackPointer].symbol].numFollow)){
                currToken=getNextToken(fp);
                // printf("curr token %s\n",enumToString[currToken.tkn_name]);
                tokenID = currToken.tkn_name;
            }
            else{
            stackPointer--;
                
            }
            
            
        } 
        else if (stack[stackPointer].type == 0 && T->cells[stack[stackPointer].symbol][tokenID].error ==0) { 
            // printf("'%s' '%s' \n", G.nonTerminals[stack[stackPointer].symbol], G.terminals[tokenID]); 
            int ruleInd = T->cells[stack[stackPointer].symbol][tokenID].ruleInd; 
            int rhsInd = T->cells[stack[stackPointer].symbol][tokenID].rhsInd; 
            int expansionRule = getRuleNumber(ruleInd, rhsInd, G);
            pointers[stackPointer]->elem.ruleNumber = expansionRule;    // Assign the rule number used by parent
            // stackPointer += 1; 
            // printf("%d %d %d %d \n", ruleInd, rhsInd, stackPointer, G.allRules[ruleInd].RHS[rhsInd].numSyms); 
            if (G.allRules[ruleInd].RHS[rhsInd].symbols[0].type == 1 && G.allRules[ruleInd].RHS[rhsInd].symbols[0].symbol == 0) { 
                node currNode = createEl(-1, stack[stackPointer].symbol, 0, 1, 0); 
                treeN* eps = (treeN*) malloc(sizeof(treeN)); 
                *eps = createNode(currNode,G); 
                addChild(pointers[stackPointer], eps); 
                // printf("Rule is epsilon, pop last non-terminal off the stack \n"); 
                stackPointer -= 1; 
            } 
            else { 
                int parentSymbol = pointers[stackPointer]->elem.curr; 
                treeN* parent = pointers[stackPointer]; 
                for (int i = G.allRules[ruleInd].RHS[rhsInd].numSyms - 1; i >= 0; i--) { 
                    if(stackPointer == stackLen) { 
                        stack = (varSymbol*) realloc(stack, sizeof(varSymbol) * (stackLen + 1)); 
                        pointers = (treeN**) realloc(pointers, sizeof(treeN*) * (stackLen + 1)); 
                        stackLen += 1; 
                    }
                    stack[stackPointer].type = G.allRules[ruleInd].RHS[rhsInd].symbols[i].type; 
                    stack[stackPointer].symbol = G.allRules[ruleInd].RHS[rhsInd].symbols[i].symbol; 
                    
                    node currNode; 
                    if (stack[stackPointer].type == 1) { 
                        currNode = createEl(-1, parentSymbol, stack[stackPointer].symbol, 1, 0); 
                    } 
                    else { 
                        currNode = createEl(-1, parentSymbol, stack[stackPointer].symbol, 0, 0); 
                    } 
                    treeN* new = (treeN*) malloc(sizeof(treeN)); 
                    *new = createNode(currNode,G); 
                    addChild(parent, new); 
                    // printf("new added : %u \n", new); 
                    pointers[stackPointer] = new; 
                    
                    if (i != 0) { 
                        stackPointer += 1; 
                    } 
                } 
            } 
            // if (stack[stackPointer].type == 0) { 
            //     printf("'%s' \n", G.nonTerminals[stack[stackPointer].symbol]); 
            // } 
            // else { 
            //     printf("'%s' \n", G.terminals[stack[stackPointer].symbol]); 
            // }
        }
    } 
    clearTwinBuffer(twin_buffer);
    return root; 
} 

void inorder(treeN* n, grammar G, int *count)
{   
    char *str;
    char *nt;
    char *terminal_tok;
    // printf("%d ", n->elem.curr);
    if(n->elem.isLeaf==1){
        str="yes";
        nt="NOT_NON_TERMINAL";
        terminal_tok = G.terminals[n->elem.curr];
    }
    else{
        str="no";
        nt=G.nonTerminals[n->elem.curr];
        terminal_tok = "NOT_TERMINAL";
    }
    char *eps_lex;
    if(strcmp(G.terminals[n->elem.curr], "eps") == 0){
        eps_lex = "eps";
    }
    else{
        eps_lex = n->elem.lex.lexemeStr;
    }
    if(n->numChild == 0){
        if(strcmp(G.terminals[n->elem.curr],"TK_NUM")==0)
            printf("%d\t%d\t%s\t%d\t%s\t%s\t%s\n",n->elem.lex.numVal,n->elem.lineNo,terminal_tok,n->elem.lex.numVal,G.nonTerminals[n->elem.parentNodeSymbolID],str,nt);
        else if(strcmp(G.terminals[n->elem.curr],"TK_RNUM")==0)
            printf("%f\t%d\t%s\t%f\t%s\t%s\t%s\n",n->elem.lex.rVal,n->elem.lineNo,terminal_tok,n->elem.lex.rVal,G.nonTerminals[n->elem.parentNodeSymbolID],str,nt);
        else {
            if(n->elem.parentNodeSymbolID != -1){
                printf("%s\t%d\t%s\tNOT_NUM\t%s\t%s\t%s\n",eps_lex,n->elem.lineNo,terminal_tok,G.nonTerminals[n->elem.parentNodeSymbolID],str,nt);
            }
            else{
                printf("%s\t%d\t%s\tNOT_NUM\t%s\tROOT\t%s\n",eps_lex,n->elem.lineNo,terminal_tok,str,nt);
            }
        } 
        (*count)++;     
        return;  
    }
    if(n->numChild==1){
        inorder(n->children[0],G,count);
            if(strcmp(G.terminals[n->elem.curr],"TK_NUM")==0)
                printf("%d\t%d\t%s\t%d\t%s\t%s\t%s\n",n->elem.lex.numVal,n->elem.lineNo,terminal_tok,n->elem.lex.numVal,G.nonTerminals[n->elem.parentNodeSymbolID],str,nt);
            else if(strcmp(G.terminals[n->elem.curr],"TK_RNUM")==0)
                printf("%f\t%d\t%s\t%f\t%s\t%s\t%s\n",n->elem.lex.rVal,n->elem.lineNo,terminal_tok,n->elem.lex.rVal,G.nonTerminals[n->elem.parentNodeSymbolID],str,nt);
            else{
                if(n->elem.parentNodeSymbolID != -1){
                    printf("%s\t%d\t%s\tNOT_NUM\t%s\t%s\t%s\n",eps_lex,n->elem.lineNo,terminal_tok,G.nonTerminals[n->elem.parentNodeSymbolID],str,nt);
                }
                else{
                    printf("%s\t%d\t%s\tNOT_NUM\t%s\tROOT\t%s\n",eps_lex,n->elem.lineNo,terminal_tok,str,nt);
                }
            }
       (*count)++;
       return;  
    }

    inorder(n->children[n->numChild-1],G,count);

    if(strcmp(G.terminals[n->elem.curr],"TK_NUM")==0)
        printf("%d\t%d\t%s\t%d\t%s\t%s\t%s\n",n->elem.lex.numVal,n->elem.lineNo,terminal_tok,n->elem.lex.numVal,G.nonTerminals[n->elem.parentNodeSymbolID],str,nt);
    else if(strcmp(G.terminals[n->elem.curr],"TK_RNUM")==0)
        printf("%f\t%d\t%s\t%f\t%s\t%s\t%s\n",n->elem.lex.rVal,n->elem.lineNo,terminal_tok,n->elem.lex.rVal,G.nonTerminals[n->elem.parentNodeSymbolID],str,nt);
    else{
        if(n->elem.parentNodeSymbolID != -1){
            printf("%s\t%d\t%s\tNOT_NUM\t%s\t%s\t%s\n",eps_lex,n->elem.lineNo,terminal_tok,G.nonTerminals[n->elem.parentNodeSymbolID],str,nt);
        }
        else{
            printf("%s\t%d\t%s\tNOT_NUM\t%s\tROOT\t%s\n",eps_lex,n->elem.lineNo,terminal_tok,str,nt);
        }
    }

    for(int i=n->numChild-2;i>=0;i--){
        inorder(n->children[i],G,count);
    }
    (*count)++;
    return;
}

// void inorder(treeN* n,FILE *f, grammar G, int *count)
// {   
//     char *str;
//     char *nt;
//     char *terminal_tok;
//     // printf("%d ", n->elem.curr);
//     if(n->elem.isLeaf==1){
//         str="yes";
//         nt="NOT_NON_TERMINAL";
//         terminal_tok = G.terminals[n->elem.curr];
//     }
//     else{
//         str="no";
//         nt=G.nonTerminals[n->elem.curr];
//         terminal_tok = "NOT_TERMINAL";
//     }
//     char *eps_lex;
//     if(strcmp(G.terminals[n->elem.curr], "eps") == 0){
//         eps_lex = "eps";
//     }
//     else{
//         eps_lex = n->elem.lex.lexemeStr;
//     }
//     if(n->numChild == 0){
//         if(strcmp(G.terminals[n->elem.curr],"TK_NUM")==0)
//             fprintf(f,"%d\t%d\t%s\t%d\t%s\t%s\t%s\n",n->elem.lex.numVal,n->elem.lineNo,terminal_tok,n->elem.lex.numVal,G.nonTerminals[n->elem.parentNodeSymbolID],str,nt);
//         else if(strcmp(G.terminals[n->elem.curr],"TK_RNUM")==0)
//             fprintf(f,"%f\t%d\t%s\t%f\t%s\t%s\t%s\n",n->elem.lex.rVal,n->elem.lineNo,terminal_tok,n->elem.lex.rVal,G.nonTerminals[n->elem.parentNodeSymbolID],str,nt);
//         else {
//             if(n->elem.parentNodeSymbolID != -1){
//                 fprintf(f,"%s\t%d\t%s\tNOT_NUM\t%s\t%s\t%s\n",eps_lex,n->elem.lineNo,terminal_tok,G.nonTerminals[n->elem.parentNodeSymbolID],str,nt);
//             }
//             else{
//                 fprintf(f,"%s\t%d\t%s\tNOT_NUM\t%s\tROOT\t%s\n",eps_lex,n->elem.lineNo,terminal_tok,str,nt);
//             }
//         } 
//         (*count)++;     
//         return;  
//     }
//     if(n->numChild==1){
//         inorder(n->children[0],f,G,count);
//             if(strcmp(G.terminals[n->elem.curr],"TK_NUM")==0)
//                 fprintf(f,"%d\t%d\t%s\t%d\t%s\t%s\t%s\n",n->elem.lex.numVal,n->elem.lineNo,terminal_tok,n->elem.lex.numVal,G.nonTerminals[n->elem.parentNodeSymbolID],str,nt);
//             else if(strcmp(G.terminals[n->elem.curr],"TK_RNUM")==0)
//                 fprintf(f,"%f\t%d\t%s\t%f\t%s\t%s\t%s\n",n->elem.lex.rVal,n->elem.lineNo,terminal_tok,n->elem.lex.rVal,G.nonTerminals[n->elem.parentNodeSymbolID],str,nt);
//             else{
//                 if(n->elem.parentNodeSymbolID != -1){
//                     fprintf(f,"%s\t%d\t%s\tNOT_NUM\t%s\t%s\t%s\n",eps_lex,n->elem.lineNo,terminal_tok,G.nonTerminals[n->elem.parentNodeSymbolID],str,nt);
//                 }
//                 else{
//                     fprintf(f,"%s\t%d\t%s\tNOT_NUM\t%s\tROOT\t%s\n",eps_lex,n->elem.lineNo,terminal_tok,str,nt);
//                 }
//             }
//        (*count)++;
//        return;  
//     }

//     inorder(n->children[n->numChild-1],f,G,count);

//     if(strcmp(G.terminals[n->elem.curr],"TK_NUM")==0)
//         fprintf(f,"%d\t%d\t%s\t%d\t%s\t%s\t%s\n",n->elem.lex.numVal,n->elem.lineNo,terminal_tok,n->elem.lex.numVal,G.nonTerminals[n->elem.parentNodeSymbolID],str,nt);
//     else if(strcmp(G.terminals[n->elem.curr],"TK_RNUM")==0)
//         fprintf(f,"%f\t%d\t%s\t%f\t%s\t%s\t%s\n",n->elem.lex.rVal,n->elem.lineNo,terminal_tok,n->elem.lex.rVal,G.nonTerminals[n->elem.parentNodeSymbolID],str,nt);
//     else{
//         if(n->elem.parentNodeSymbolID != -1){
//             fprintf(f,"%s\t%d\t%s\tNOT_NUM\t%s\t%s\t%s\n",eps_lex,n->elem.lineNo,terminal_tok,G.nonTerminals[n->elem.parentNodeSymbolID],str,nt);
//         }
//         else{
//             fprintf(f,"%s\t%d\t%s\tNOT_NUM\t%s\tROOT\t%s\n",eps_lex,n->elem.lineNo,terminal_tok,str,nt);
//         }
//     }

//     for(int i=n->numChild-2;i>=0;i--){
//         inorder(n->children[i],f,G,count);
//     }
//     (*count)++;
//     return;


// }

// void printParseTree(treeN* rootNode, char *outfile,grammar G, int *count){
//     printf("Printing the parse tree inorder------\n");
//     if(rootNode==NULL){
//     printf("ERROR: Parse Tree root is null");
//     return;
//     }
//     FILE* f;

//     f = fopen(outfile,"wb");

//     if(f == NULL) {
//         printf("Error opening the output file\n");
//         return;
//     }

//     inorder(rootNode,f,G,count);    
//     fclose(f);
// }

void printParseTree(treeN* rootNode,grammar G, int *count){
    printf("Printing the parse tree inorder:\n");
    if(rootNode==NULL){
         printf("ERROR: Parse Tree root is null");
    return;
    }
    
    inorder(rootNode,G,count);    
}

int getRuleNumber(int ruleId, int rhsId, grammar G){
    int c = 0;
    // printf("LHS of rule: %s\n",G.nonTerminals[G.allRules[ruleId].LHS]);
    for(int i=0;i<G.totalNumRules;i++){
        for(int j = 0;j<G.allRules[i].numOrs;j++){
            if(i<ruleId || (i==ruleId && j<=rhsId))c++;
            // printf("LHS of rule %d ors %d: %s\n",c,G.allRules[i].numOrs,G.nonTerminals[G.allRules[i].LHS]);
        }
    }
    // printf("LHS of rule %d ors %d: %s\n",c,G.allRules[ruleId].numOrs,G.nonTerminals[G.allRules[ruleId].LHS]);
    return c;
}
// void main() { 
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

//     char* testCaseFile = "./testcases_stage1/t6.txt"; 
//     // FILE *fp = fopen("./testcases_stage1/t2.txt","r"); 
//     // initialize();
//     // fp = getStream(fp, 0);
//     // tokenInfo currToken = getNextToken(fp); 
//     // printf("%d '%s' \n", findIndex(C.terminals, C.numTerminals, enumToStringP[currToken.tkn_name]), enumToStringP[currToken.tkn_name]); 
//     treeN rootNode; 
//     rootNode = parseInputSourceCode(testCaseFile, C, T); 
//     // printf("%u %d %d %d %d %d \n", &rootNode, rootNode.elem.curr, rootNode.numChild, rootNode.elem.lineNo, rootNode.elem.isLeaf, rootNode.elem.parentNodeSymbolID); 
//     // printf("%u %u \n", rootNode.children[0], rootNode.children[1]); 
//     // printf("%u %u \n", rootNode.children[1]->children[1], rootNode.children[1]->children[1]->children[5]); 
//     // printf("%d '%s' %d '%s' \n", rootNode.children[1]->children[1], C.nonTerminals[rootNode.children[1]->children[1]->elem.curr], rootNode.children[1]->children[1]->children[5], C.terminals[rootNode.children[1]->children[1]->children[5]->elem.curr]); 
//     printParseTree(&rootNode,"op.txt",C);
//     // printf("%d %d '%s' '%s' \n", C.ff[5].numFirst[0], C.ff[5].first[0][0], C.nonTerminals[5], C.terminals[C.ff[5].first[0][0]]); 
// } 