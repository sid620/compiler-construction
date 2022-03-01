#include "utils.h"

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


int isInArr(int* arr, int num, int lim) { 
    for (int i = 0; i < lim; i++) { 
        if (num == arr[i]) { 
            return 1; 
        }
    } 
    return 0; 
} 


void printRule(grammar G, int ruleNum, int rhsInd){

    // print entire rule
    printf("\n\nprinting ruleNumber: %d\n",G.allRules[ruleNum].ruleNumber);
    printf("LHS: %s\n",G.nonTerminals[G.allRules[ruleNum].LHS]);
    printf("RHS: ");
    if (rhsInd==-1){
        for(int i=0; i<G.allRules[ruleNum].numOrs;i++){
            for(int k=0; k<G.allRules[ruleNum].RHS[i].numSyms;k++){
                if (G.allRules[ruleNum].RHS[i].symbols[k].type==0)
                    printf("%s ",G.nonTerminals[G.allRules[ruleNum].RHS[i].symbols[k].symbol]);
                else
                    printf("%s ",G.terminals[G.allRules[ruleNum].RHS[i].symbols[k].symbol]);
            }
            if (i<G.allRules[ruleNum].numOrs-1)
                printf("| ");
        }
        printf("\n\n\n");
    }
    // print specific RHS prod rule
    else{
        for(int k=0; k<G.allRules[ruleNum].RHS[rhsInd].numSyms;k++){
            if (G.allRules[ruleNum].RHS[rhsInd].symbols[k].type==0)
                printf("%s ",G.nonTerminals[G.allRules[ruleNum].RHS[rhsInd].symbols[k].symbol]);
            else
                printf("%s ",G.terminals[G.allRules[ruleNum].RHS[rhsInd].symbols[k].symbol]);
        }
        printf("\n\n\n");
    }
}


void prettyPrintGrammar(grammar G){
    for (int i=0;i<G.totalNumRules;i++){
        printf("Rule Num: %d ",i);
        for(int j=0;j<G.allRules[i].numOrs;j++){
            printf("\n\tRHS symbol index: %d ",j);
            for(int k=0;k<G.allRules[i].RHS[j].numSyms;k++){
                printf("\n\t\ttype of symbol: %d, symbol(id): %d",G.allRules[i].RHS[j].symbols->type , G.allRules[i].RHS[j].symbols[k].symbol);
            }
        }
        printf("\n\n");
    }
}