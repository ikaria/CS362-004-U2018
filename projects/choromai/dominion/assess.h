#ifndef _ASSESS_H
#define _ASSESS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int AssertCondition(char* failMsg, int pass);
int AssertTest(char* resultMsg, int pass);

#endif