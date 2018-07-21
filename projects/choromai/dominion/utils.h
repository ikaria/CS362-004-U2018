#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *Timeout();
void *CheckRace(void *args);
int TestRace(void (*userFunction)());
int AssertCondition(char* failMsg, int pass);
int AssertTest(char* resultMsg, int pass);

#endif