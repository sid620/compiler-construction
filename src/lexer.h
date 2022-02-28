#pragma once
#include "lookuptable.h"
#include "lexerDef.h"
#include <string.h>
#include <stdio.h>

int dfa_state = 0;

FILE *getStream(FILE *fp, int bufferNo);
tokenInfo getNextToken(FILE *fp);
void removeComments(char *testcaseFile, char *cleanFile);