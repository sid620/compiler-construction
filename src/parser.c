#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "parserDef.h" 

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
    "eps", "TK_MAIN", "TK_END", "TK_FUNID", "TK_SEM", "TK_INPUT", "TK_PARAMETER", "TK_LIST", "TK_SQL", "TK_SQR", "TK_OUTPUT", "TK_ID", 
    "TK_INT", "TK_REAL", "TK_RECORD", "TK_RUID", "TK_UNION", "TK_COMMA", "TK_ENDRECORD", "TK_ENDUNION", "TK_TYPE", "TK_COLON", 
    "TK_FIELDID", "TK_GLOBAL", "TK_ASSIGNOP", "TK_DOT", "TK_CALL", "TK_WITH", "TK_WHILE", "TK_OP", "TK_CL", "TK_ENDWHILE", 
    "TK_IF", "TK_THEN", "TK_ENDIF", "TK_ELSE", "TK_READ", "TK_WRITE", "TK_MUL", "TK_DIV", "TK_PLUS", "TK_MINUS", "TK_OP", "TK_NUM", 
    "TK_RNUM", "TK_AND", "TK_OR", "TK_LT", "TK_GT", "TK_LE", "TK_GE", "TK_NE", "TK_EQ", "TK_RETURN", "TK_DEFINTEYPE", "TK_AS"}; 

// #define NUM_NON_TERMINALS 53 
// #define NUM_TERMINALS 56 

int numTerminals = 56; 

int findIndex(char** arr, int lim, char* str) { 
    for (int i = 0; i < lim; i++) { 
        if (strcmp(str, arr[i]) == 0) { 
            return i; 
        }
    } 
    return -1; 
} 

void printArr(char** arr, int lim) { 
    for (int i = 0; i < lim; i++) { 
        printf("%s \n", arr[i]); 
    }
}

grammar readGrammar(char* file) { 
    
    grammar currGrammar; 

    char** nonTerminals = (char**) malloc(sizeof(char*) * 1); 
    nonTerminals[0] = "program"; 
    int numNonTerminals = 1; 

    
    FILE* f = fopen(file, "r");
    int ruleID = 0; 

    while (!feof(f)) { 
        char s[300]; 
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
    printf("%d %d %d \n*** \n", numNonTerminals, numTerminals, ruleID); 
    currGrammar.numNonTerminals = numNonTerminals; 
    currGrammar.numTerminals = numTerminals; 

    fclose(f); 
    ruleID = 0; 
    FILE* fN = fopen(file, "r"); 

    while(!feof(fN)) { 
        char s[300]; 
        fgets(s, 300, fN); 
        // fscanf(f, "%[^\"]\n", s); 

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
    printf("%d \n", ruleID); 
    // printf("%d %d %d \n", currGrammar.allRules[0].LHS, currGrammar.allRules[0].numOrs, currGrammar.allRules[0].ruleNumber); 
    // printf("%d %d %d %d %d", currGrammar.allRules[0].RHS[0].numSyms, currGrammar.allRules[0].RHS[0].symbols[0].type, currGrammar.allRules[0].RHS[0].symbols[1].type, currGrammar.allRules[0].RHS[0].symbols[0].symbol, currGrammar.allRules[0].RHS[0].symbols[1].symbol); 
} 

// void main() { 
//     char* file; 
//     file = "grammar.txt"; 
//     grammar C; 
//     C = readGrammar(file); 
// }

// createParseTable(FirstAndFollow F, table T)

// parseInputSourceCode(char *testcaseFile, table T)

// printParseTree(parseTree PT, char *outfile)
