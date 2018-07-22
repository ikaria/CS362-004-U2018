#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include "utils.h"

//TESTING: isGameOver()

//game state with province count = 0, expected 1
int provinceCountZero_EndGame(struct gameState *state)
{
  //setup state
  state->supplyCount[province] = 0;

  int result = isGameOver(state);


  return AssertCondition("Failed to dected 0 provinces.", result);
}

//game state with (3) supply counts = 0, expected 1
int threeTreasuresCountZero_EndGame(struct gameState *state)
{

  //setup state
  state->supplyCount[0] = 0;
  state->supplyCount[1] = 0;
  state->supplyCount[2] = 0;

  int result = isGameOver(state);


  return AssertCondition("Failed to dected 0 provinces.", result);
}

//game state with (2) supply counts = 0, expected: 0
int twoTreasuresCountZero_ContinueGame(struct gameState *state)
{
  //setup state
  state->supplyCount[0] = 0;
  state->supplyCount[1] = 0;

  int result = 0;
  result = !isGameOver(state);


  return AssertCondition("False positive. Game should not end", result); 
}

//function has race condition, expected: none 
void raceCondition_NoneDetected()
{
  struct gameState G;
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};
  initializeGame(2, k, 2, &G);

  isGameOver(&G);
}


int main (int argc, char** argv) {
  struct gameState G;
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};

  printf("\n**************  isGameOver() ***********\n");

  int success = 1;

  initializeGame(2, k, 2, &G);
  success &= AssertTest ("Test 0: Race Condition | Expected: None Detected ",
    TestRace(&raceCondition_NoneDetected));

  initializeGame(2, k, 2, &G);
  success &= AssertTest ("Test 1: Province Count = 0 | Expected: End Game",
    provinceCountZero_EndGame(&G));

  initializeGame(2, k, 2, &G);
  success &= AssertTest("Test 2: (2) Treasures Count = 0 | Expected: Continue Game",
    twoTreasuresCountZero_ContinueGame(&G));

  initializeGame(2, k, 2, &G);
  success &= AssertTest("Test 3: (3) Treasures Count = 0 | Expected: End Game",
    threeTreasuresCountZero_EndGame(&G));

  if(success)
  {
    printf ("ALL TESTS PASSED\n");
  }
  printf ("\n");

  return 0;
}
