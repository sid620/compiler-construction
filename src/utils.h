#pragma once
#include <stdio.h> 
#include "parserDef.h"
#include <string.h> 

void prettyPrintGrammar(grammar G);
int findIndex(char** arr, int lim, char* str);
void printArr(char** arr, int lim);
int isInArr(int* arr, int num, int lim);