#ifndef LOOKUPTABLEH
#define LOOKUPTABLEH
#include "lookuptableDef.h"
#include "lexerDef.h"
#include<stdio.h>
// polynomial rolling function to be used
int binExpo(int a, int b);
int kmult(int a, int b);
int hash(char *lexeme, int i);
void initialize();
int h1(char *lexeme);
int h2(int h1_hashvalue);
int search(char *lexeme);
int insert(char *lexeme);



#endif