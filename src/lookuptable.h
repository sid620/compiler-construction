#ifndef LOOKUPTABLEH
#define LOOKUPTABLEH
#include "lookuptableDef.h"

int hash(char *lexeme);
void initialize(lookupTable s);
int search(lookupTable s, char *lexeme);
int insert(lookupTable s, char *lexeme, int hashvalue);



#endif