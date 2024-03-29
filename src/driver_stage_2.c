/*
ID  2019A7PS0064P					Name Siddharth Sharma
ID  2019A7PS0062P					Name Atharva Chandak
ID  2019A7PS0133P					Name Archit Bhatnagar 
ID  2019A7PS0554P					Name Suchismita Tripathy
ID  2019A7PS1139P 					Name Srujan Deolasee
*/

#include "parser.h"
#include <time.h>
#include "ast.h"
#include "symbolTable.h"
#include "typechecker.h"
#include "symbolTable.h"
#include "semantics.h"

int main(int argc, char *argv[]){
    
	int choice=0;
	printf("LEVEL 4: Symbol table, AST, Semantic Rules, Type Checking modules work.\n");
	char *file;
	file = "grammar.txt";
    while(1){
		printf("\n\nEnter 0 to exit.\n");
		printf("Enter 1 to print the token list generated by the lexer.\n");
		printf("Enter 2 to verify the syntactic correctness of the input source code and to produce parse tree.\n");
		printf("Enter 3 for printing Abstract Syntax Tree in Inorder Traversal.\n");
		printf("Enter 4 for for displaying the amount of allocated memory and number of nodes to each of parse tree and abstract syntax tree for the test case used.\n");
		printf("Enter 5 for printing Symbol Table.\n");
		printf("Enter 6 for printing Global Variables.\n");   
		printf("Enter 7 for printing Activation record sizes.\n");
		printf("Enter 8 for printing Record types and sizes.\n");
		printf("Enter 9 for Type checking and Semantic Analysis.\n");
		printf("Enter 10 for Code generation and dynamic type checking.\n");

        printf("\nENTER CHOICE:\n");
        scanf("%d", &choice);
		int*errorFlag = (int *)malloc(sizeof(int));
        switch(choice){
            case 0:{
                printf("\nExiting.\n");
                exit(1);
            }
            break;

            case 1:{
                // printing the token list generated by the lexer
                printf("\nPrinting the token list generated by the lexer.\n");
                initialize();
                prettyPrint(argv[1]);
            }
            break;

            case 2:{
                // parsing to verify the syntactic correctness of the input source code and to produce parse tree
                printf("parsing to verify the syntactic correctness of the input source code and to produce parse tree.\n");
                initialize();
                
                grammar C;
                C = readGrammar(file);
                C.ff = ComputeFirstAndFollowSets(C);
                parseTable* T = intializeParseTable(C.numNonTerminals,C.numTerminals);
                createParseTable(C,C.ff,T);
                treeN rootNode; 
				*errorFlag=0;
                rootNode = parseInputSourceCode(argv[1], C, T, errorFlag);
                int *count1 = (int *)malloc(sizeof(int));
    			*count1 = 0;
    			printParseTree(&rootNode, C,count1);
            }
            break;

            case 3:{
                // printing the Abstract Syntax Tree

    			printf("Printing Abstract Syntax Tree in Inorder Traversal\n\n");
                initialize();

				grammar C;
				C = readGrammar(file);
				C.ff = ComputeFirstAndFollowSets(C);
				parseTable* T = intializeParseTable(C.numNonTerminals, C.numTerminals);
				createParseTable(C, C.ff, T);
				char *testCaseFile = argv[1];
				treeN rootNode;

				*errorFlag=0;
				rootNode = parseInputSourceCode(testCaseFile, C, T, errorFlag);
				if(!(*errorFlag)){

					int *count1 = (int *)malloc(sizeof(int));
					*count1 = 0;
					// printParseTree(&rootNode,C,count1);
					int *insertPrev = (int *)malloc(sizeof(int));
					*insertPrev = 0;
					int *count = (int *)malloc(sizeof(int));
					*count = 0;
					astNode *astroot = mknode(rootNode.elem,C);
					printf("Constructing AST...\n");
					constructAst(astroot, &rootNode,C,insertPrev,astroot);
					printf("AST Constructed.\n");
					printf("\nPrinting AST (INORDER):\n");
					printAST(astroot,C, count);
				}
				else{
					printf("Error occured in parsing. Cant print AST.");
				}

            }
            break;

            case 4:{
				// displaying the amount of allocated memory and number of nodes to each of parse tree and abstract syntax tree for the test case used
                printf("\ndisplaying the amount of allocated memory and number of nodes to each of parse tree and abstract syntax tree for the test case used\n");
				initialize();

				grammar C;
				C = readGrammar(file);
				C.ff = ComputeFirstAndFollowSets(C);
				parseTable* T = intializeParseTable(C.numNonTerminals, C.numTerminals);
				createParseTable(C, C.ff, T);
				char *testCaseFile = argv[1];
				treeN rootNode;

				*errorFlag=0;
				rootNode = parseInputSourceCode(testCaseFile, C, T, errorFlag);
				
				if(!(*errorFlag)){
					int *count1 = (int *)malloc(sizeof(int));
					*count1 = 0;
					printParseTree(&rootNode, C, count1);
					int *insertPrev = (int *)malloc(sizeof(int));
					*insertPrev = 0;
					int *count = (int *)malloc(sizeof(int));
					*count = 0;
					astNode *astroot = mknode(rootNode.elem,C);
					printf("Constructing AST...\n");
					constructAst(astroot, &rootNode,C,insertPrev,astroot);
					printf("AST Constructed.\n");
					printf("\nPrinting AST (INORDER):\n");
					printAST(astroot,C, count);

					printf("Parse Tree Number of nodes = %d. Allocated Memory = %lu bytes\n",*count1,sizeof(treeN)*(*count1));
					printf("AST Number of nodes = %d. Allocated Memory = %lu bytes\n\n",*count,sizeof(astNode)*(*count));

					float p1 = sizeof(treeN)*(*count1);
					float p2 = sizeof(astNode)*(*count);
					float compressionPercentage = ((p1 - p2)/p1)*100;
					printf("Compression percentage = ((%f - %f)/%f)*100 = %f\n", p1, p2, p1, compressionPercentage);
				}
				else{
					printf("Error occured in parsing. Cant print AST.\n");
				}
            }
            break;

			case 5:{
				// printing symbol table
				printf("\nPrint Symbol Table\n");
                initialize();

				grammar C;
				C = readGrammar(file);
				C.ff = ComputeFirstAndFollowSets(C);
				parseTable* T = intializeParseTable(C.numNonTerminals, C.numTerminals);
				createParseTable(C, C.ff, T);
				char *testCaseFile = argv[1];
				treeN rootNode;

				*errorFlag=0;
				rootNode = parseInputSourceCode(testCaseFile, C, T, errorFlag);
				
				if(!(*errorFlag)){
					int *count1 = (int *)malloc(sizeof(int));
					*count1 = 0;
					// printParseTree(&rootNode,C,count1);
					int *insertPrev = (int *)malloc(sizeof(int));
					*insertPrev = 0;
					int *count = (int *)malloc(sizeof(int));
					*count = 0;
					astNode *astroot = mknode(rootNode.elem,C);
				    printf("Constructing AST...\n");
					constructAst(astroot, &rootNode,C,insertPrev,astroot);
				    symbolTable* sTable = constructST(astroot, C); 
					// int ind1 = searchTypes("#coordinate", sTable); 
					// printf("%d %d %d %s \n", ind1, sTable->allTypes[ind1]->numFields, sTable->allTypes[ind1]->numAl, sTable->allTypes[ind1]->name); 
					// int ind2 = searchTypes("#point", sTable); 
					// printf("%s %d  %s \n", sTable->allTypes[ind1]->aliases[0], ind2, sTable->allTypes[ind2]->name); 
					printf("\nPrinting Symbol Table:\n");
					printSymbolTable(sTable);
				}
				else{
					printf("Error occured in parsing. Cant print symbol Table.\n");
				}
			}
			break;

			case 6:{
				// Print Global Variables
				printf("\nPrinting Global Variables\n");
                initialize();

				grammar C;
				C = readGrammar(file);
				C.ff = ComputeFirstAndFollowSets(C);
				parseTable* T = intializeParseTable(C.numNonTerminals, C.numTerminals);
				createParseTable(C, C.ff, T);
				char *testCaseFile = argv[1];
				treeN rootNode;

				*errorFlag=0;
				rootNode = parseInputSourceCode(testCaseFile, C, T, errorFlag);
				if(!(*errorFlag)){
					int *count1 = (int *)malloc(sizeof(int));
					*count1 = 0;
					// printParseTree(&rootNode,C,count1);
					int *insertPrev = (int *)malloc(sizeof(int));
					*insertPrev = 0;
					int *count = (int *)malloc(sizeof(int));
					*count = 0;
					astNode *astroot = mknode(rootNode.elem,C);
					printf("Constructing AST...\n");
					constructAst(astroot, &rootNode,C,insertPrev,astroot);
					printf("AST Constructed.\n");
					
					symbolTable *sTable = constructST(astroot, C);
					printf("\nPrinting Global Variables:\n");
					printGlobalVars(sTable);
				}
				else{
					printf("Error occured in parsing. Cant print Global Variables.\n");
				}

			}
			break;

			case 7:{
				// Print Activation record sizes
				printf("\nPrinting Activation record sizes\n");
                initialize();

				grammar C;
				C = readGrammar(file);
				C.ff = ComputeFirstAndFollowSets(C);
				parseTable* T = intializeParseTable(C.numNonTerminals, C.numTerminals);
				createParseTable(C, C.ff, T);
				char *testCaseFile = argv[1];
				treeN rootNode;

				*errorFlag=0;
				rootNode = parseInputSourceCode(testCaseFile, C, T, errorFlag);
				
				if(!(*errorFlag)){

					int *count1 = (int *)malloc(sizeof(int));
					*count1 = 0;
					// printParseTree(&rootNode,C,count1);
					int *insertPrev = (int *)malloc(sizeof(int));
					*insertPrev = 0;
					int *count = (int *)malloc(sizeof(int));
					*count = 0;
					astNode *astroot = mknode(rootNode.elem,C);
					printf("Constructing AST...\n");
					constructAst(astroot, &rootNode,C,insertPrev,astroot);
					printf("AST Constructed.\n");
					
					symbolTable *sTable = constructST(astroot, C);
					printf("\nActivation record sizes are:\n");
					printARSizes(sTable);
				}
				else{
					printf("Error occured in parsing. Cant print activation records.\n");
				}	
			}
			break;

			case 8:{
				// Record types and sizes
				printf("\nPrinting record types and sizes\n");
                initialize();

				grammar C;
				C = readGrammar(file);
				C.ff = ComputeFirstAndFollowSets(C);
				parseTable* T = intializeParseTable(C.numNonTerminals, C.numTerminals);
				createParseTable(C, C.ff, T);
				char *testCaseFile = argv[1];
				treeN rootNode;

				*errorFlag=0;
				rootNode = parseInputSourceCode(testCaseFile, C, T, errorFlag);

				if(!(*errorFlag)){

					int *count1 = (int *)malloc(sizeof(int));
					*count1 = 0;
					// printParseTree(&rootNode,C,count1);
					int *insertPrev = (int *)malloc(sizeof(int));
					*insertPrev = 0;
					int *count = (int *)malloc(sizeof(int));
					*count = 0;
					astNode *astroot = mknode(rootNode.elem,C);
					printf("Constructing AST...\n");
					constructAst(astroot, &rootNode,C,insertPrev,astroot);
					printf("AST Constructed.\n");
					
					symbolTable *sTable = constructST(astroot, C);
					printf("\nPrinting record types and sizes:\n");
					printRUInfo(sTable);
				}
				else{
					printf("Error occured in parsing. Cant print record types and sizes.\n");
				}

			}
			break;

			case 9:{
				// Type checking and semantic analysis
				printf("\nType checking and semantic analysis\n");
				
				clock_t start_time, end_time;
                double total_CPU_time, total_CPU_time_in_seconds;
                start_time = clock();

				// print syntactic errors if any
				initialize();
                
                grammar C;
                C = readGrammar(file);
                C.ff = ComputeFirstAndFollowSets(C);
                parseTable* T = intializeParseTable(C.numNonTerminals,C.numTerminals);
                createParseTable(C,C.ff,T);
                treeN rootNode; 

				*errorFlag=0;
                rootNode = parseInputSourceCode(argv[1], C, T, errorFlag);

				if(!(*errorFlag)){

					// type checking and semantic analysis
					int *insertPrev = (int *)malloc(sizeof(int));
					*insertPrev = 0;
					int *count = (int *)malloc(sizeof(int));
					*count = 0;
					astNode *astroot = mknode(rootNode.elem,C);
					printf("Constructing AST...\n");
					constructAst(astroot, &rootNode,C,insertPrev,astroot);
					printf("AST Constructed.\n");
					
					symbolTable* sTable = constructST(astroot, C);
					printf("Type Checking... \n"); 
					int tc = typeCheck(astroot, C, sTable, -1);
					semanticCheck(astroot,sTable,C);
				}
				else{
					printf("Error occured in parsing. Cant perform semantic analysis and typechecking.\n");
				}
				end_time = clock();
                total_CPU_time = (double) (end_time - start_time);
                total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

				// Print both total_CPU_time and total_CPU_time_in_seconds
                printf("\ntotal_CPU_time: %-15f total_CPU_time_in_seconds %-15f\n", total_CPU_time, total_CPU_time_in_seconds);
			}
			break;

			case 10:{
				// Code generation and dynamic type checking
				printf("\nCode generation and dynamic type checking\n");
                printf("<Not Implemented>\n");
				
			}			
			break;

            default:
                printf("ENTER CORRECT CHOICE\n");
            break;
        }
    }
}