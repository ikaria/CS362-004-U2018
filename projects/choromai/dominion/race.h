#ifndef _RACE_H
#define _RACE_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *Timeout();
void *CheckRace(void *args);
int TestRace(void (*userFunction)());

#endif