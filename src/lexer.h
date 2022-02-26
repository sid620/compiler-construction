#pragma once

#include "lexerDef.h"
#include <string.h>
#include <stdio.h>

int dfa_state = 0;

FILE *getStream(FILE *fp);
tokenInfo getNextToken(twinBuffer B);
void removeComments(char *testcaseFile, char *cleanFile);