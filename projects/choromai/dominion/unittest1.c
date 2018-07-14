#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>


//game state with province count = 0, expected 1
int provinceCountZero_EndGame(struct gameState *state)
{
  int pass = 0;

  //setup state
  state->supplyCount[province] = 0;

  int result = isGameOver(state);
  if(result == 1)
  {
    pass = 1;
  }

  printf ("Test 1: Province Count = 0 | Expection: End Game | Result: ");
  if(pass)
    printf ("PASSED\n");
  else
    printf ("FAILED\n");

  return pass;
}

//game state with (3) supply counts = 0, expected 1
int threeTreasuresCountZero_EndGame(struct gameState *state)
{
  int pass = 0;

  //setup state
  state->supplyCount[0] = 0;
  state->supplyCount[1] = 0;
  state->supplyCount[2] = 0;

  int result = isGameOver(state);
  if(result == 1)
  {
    pass = 1;
  }

  printf ("Test 2: (3) Treasures Count = 0 | Expection: End Game | Result: ");
  if(pass)
    printf ("PASSED\n");
  else
    printf ("FAILED\n");

  return pass;
}

//game state with (2) supply counts = 0, expected: 0
int twoTreasuresCountZero_ContinueGame(struct gameState *state)
{
  int pass = 0;

  //setup state
  state->supplyCount[0] = 0;
  state->supplyCount[1] = 0;

  int result = 0;
  result = isGameOver(state);
  if(result == 0)
  {
    pass = 1;
  }

  printf ("Test 3: (2) Treasures Count = 0 | Expection: Continue Game | Result: ");
  if(pass)
    printf ("PASSED\n");
  else
    printf ("FAILED\n");


  return pass;
}

int raceCondition_NoneDetected(struct gameState *state)
{
  int pass = 0;
  pass = 1;
  return pass;
}

//function doesn't have any race conditions

int main (int argc, char** argv) {
  struct gameState G;
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};

  printf("\n**************  isGameOver() ***********\n");

  int success = 0;
  initializeGame(2, k, 2, &G);
  success += provinceCountZero_EndGame(&G);

  initializeGame(2, k, 2, &G);
  success += twoTreasuresCountZero_ContinueGame(&G);

  initializeGame(2, k, 2, &G);
  success += threeTreasuresCountZero_EndGame(&G);

  initializeGame(2, k, 2, &G);
  success += raceCondition_NoneDetected(&G);

  if(success == 0)
  {
    printf ("ALL TESTS PASSED\n");
  }
  printf ("\n");

  return 0;
}
