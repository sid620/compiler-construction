#include "parser.h"
#include <time.h>

int main(int argc, char *argv[]){
    int choice=0;
    while(1){
        printf("\nENTER CHOICE:\n");
        scanf("%d", &choice);

        switch(choice){
            case 0:{
                exit(1);
            }
            break;

            case 1:{
                // printf("\nChoice  1: COMMENT REMOVAL CALLED\n");
                removeComments(argv[1], "commentremoval.txt");
                FILE *comment_fptr = fopen("commentremoval.txt", "r");
                printf("\nChoice  1: COMMENT REMOVAL CALLED\n");
                if (comment_fptr == NULL){
                    printf("Cannot open file \n");
                }
                char c;
                // Read contents from file

                c = fgetc(comment_fptr);
                while (c != EOF){
                    printf ("%c", c);
                    c = fgetc(comment_fptr);
                }
                fclose(comment_fptr);
            }
            break;

            case 2:{
                initialize();
                prettyPrint(argv[1]);
            }
            break;

            case 3:{
                initialize();
                char* file; 
                file = "grammar.txt"; 
                grammar C; 
                C = readGrammar(file);
                C.ff = ComputeFirstAndFollowSets(C);
                parseTable* T = intializeParseTable(C.numNonTerminals,C.numTerminals);
                createParseTable(C,C.ff,T);
                treeN rootNode; 
                rootNode = parseInputSourceCode(argv[1], C, T); 
                printParseTree(&rootNode,argv[2],C);

                // C = readGrammar(file);
                // C.ff = ComputeFirstAndFollowSets(C);
                // parseTable* T = intializeParseTable(C.numNonTerminals,C.numTerminals);
                // createParseTable(C,C.ff,T);
                // treeN rootNode;
                // rootNode = parseInputSourceCode(argv[1], C, T);
                // printParseTree(&rootNode, argv[2],C);
            }
            break;

            case 4:{
                clock_t start_time, end_time;
                double total_CPU_time, total_CPU_time_in_seconds;
                start_time = clock();
                // invoke your lexer and parser here
                initialize();
                char *file;
                file = "grammar.txt";
                grammar C;
                C = readGrammar(file);
                printf("%d %d %d %d \n", C.totalNumRules, C.allRules[36].numOrs, C.allRules[31].epsilon, C.allRules[28].epsilon); 
                C.ff = ComputeFirstAndFollowSets(C);
                parseTable* T = intializeParseTable(C.numNonTerminals,C.numTerminals);
                createParseTable(C,C.ff,T);
                
                printf("%d %d %d '%s' '%s' \n", C.allRules[2].numOrs, C.ff[2].numFirst[0], C.ff[2].numFirst[1], C.terminals[C.ff[2].first[0][0]], C.terminals[C.ff[2].first[1][0]]); 
                printf("%d %d %d \n", C.allRules[0].numOrs, C.allRules[0].RHS[0].numSyms, C.allRules[0].RHS[0].symbols[1].type); 

                parseInputSourceCode(argv[1], C, T); 

                end_time = clock();
                total_CPU_time = (double) (end_time - start_time);
                total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

                // Print both total_CPU_time and total_CPU_time_in_seconds
                printf("total_CPU_time: %-15f total_CPU_time_in_seconds %-15f\n", total_CPU_time, total_CPU_time_in_seconds);
            }
            break;

            default:
                printf("ENTER CORRECT CHOICE\n");
            break;
        }
    }
}
