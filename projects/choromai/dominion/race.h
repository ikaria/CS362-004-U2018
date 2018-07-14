#ifndef _RACE_H
#define _RACE_H

void *Timeout();
void *CheckRace(void *args);
int TestRace(void (*userFunction)());

#endif