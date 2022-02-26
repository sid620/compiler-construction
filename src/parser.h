#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "parserDef.h" 

grammar readGrammar(char* file); 
FirstAndFollow* ComputeFirstAndFollowSets (grammar G); 
void IndividualFirst(grammar G, FirstAndFollow* ff, int ind); 
void IndividualFollow(grammar G, FirstAndFollow* ff, int ind, int prev); 
// createParseTable(FirstAndFollow F, parseTable T); 
// parseInputSourceCode(char *testcaseFile, parseTable T); 
// printParseTree(parseTree PT, char *outfile); 
