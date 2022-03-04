#pragma once
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "parserDef.h" 
#include "utils.h"
#include "lexer.h"
#include "tree.h"

grammar readGrammar(char* file);
FirstAndFollow* ComputeFirstAndFollowSets (grammar G);
void IndividualFirst(grammar G, FirstAndFollow* ff, int ind);
void IndividualFollow(grammar G, FirstAndFollow* ff, int ind, int prev);
void printParseTable(grammar G,parseTable* T);
parseTable* intializeParseTable( int numNonTerminals, int numTerminals); 
void createParseTable(grammar G, FirstAndFollow* ff, parseTable* T);
treeN parseInputSourceCode(char* testCaseFile, grammar G, parseTable* T);
void inorder(treeN* n,FILE *f, grammar G);
void printParseTree(treeN* rootNode, char *outfile,grammar G);
