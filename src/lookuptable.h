#ifndef LOOKUPTABLEH
#define LOOKUPTABLEH
#include "lookuptableDef.h"
#include "lexerDef.h"
#include<stdio.h>
// polynomial rolling function to be used
int binExpo(int a, int b);
int kmult(int a, int b);
int hash(char *lexeme, int i);
void initialize(lookupTable s);
int h1(char *lexeme);
int h2(int h1_hashvalue);
int search(lookupTable s, char *lexeme);
int insert(lookupTable s, char *lexeme);



#endif