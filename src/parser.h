/*
ID  2019A7PS0064P					Name Siddharth Sharma
ID  2019A7PS0062P					Name Atharva Chandak
ID  2019A7PS0133P					Name Archit Bhatnagar 
ID  2019A7PS0554P					Name Suchismita Tripathy
ID  2019A7PS1139P 					Name Srujan Deolasee
*/

#pragma once
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
treeN parseInputSourceCode(char* testCaseFile, grammar G, parseTable* T, int*errorFlag);
void inorder(treeN* n, grammar G, int *count);
void printParseTree(treeN* rootNode, grammar G, int *count);
int getRuleNumber(int ruleInd, int rhsInd, grammar G);