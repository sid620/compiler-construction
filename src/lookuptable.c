#include "lookuptable.h"
#include <string.h>


int hash(char *lexeme);
void initialize(lookupTable s);
int search(lookupTable s, char *lexeme);
int insert(lookupTable s, char *lexeme, int hashvalue);