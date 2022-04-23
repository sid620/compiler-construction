/*
ID  2019A7PS0064P					Name Siddharth Sharma
ID  2019A7PS0062P					Name Atharva Chandak
ID  2019A7PS0133P					Name Archit Bhatnagar 
ID  2019A7PS0554P					Name Suchismita Tripathy
ID  2019A7PS1139P 					Name Srujan Deolasee
*/

#pragma once
#include <stdio.h> 
#include "parserDef.h"
#include <string.h> 

void prettyPrintGrammar(grammar G);
int findIndex(char** arr, int lim, char* str);
void printArr(char** arr, int lim);
int isInArr(int* arr, int num, int lim);
void printRule(grammar G, int ruleNum, int rhsInd);