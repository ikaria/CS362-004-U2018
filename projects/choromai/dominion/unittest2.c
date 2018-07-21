#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include "utils.h"


//Test Gold count
int twoGoldCoins_Plus6(struct gameState *state)
{

  //setup state
  int player = 1;
  state->handCount[player] = 5;

  state->hand[player][0] = village;
  state->hand[player][1] = village;
  state->hand[player][2] = village;
  state->hand[player][3] = gold;
  state->hand[player][4] = gold;

  //run function
  updateCoins(player,state,0);

  //return results, add fail msg if 0 
  return AssertCondition("Gold coins failed.", state->coins == 6);
}


//Test Silver count
int oneSilverCoin_Plus2(struct gameState *state)
{
 
  //setup state
  int player = 1;
  state->handCount[player] = 5;

  state->hand[player][0] = village;
  state->hand[player][1] = village;
  state->hand[player][2] = village;
  state->hand[player][3] = village;
  state->hand[player][4] = silver;

  //test function
  updateCoins(player,state,0);
   
   //return result
  return AssertCondition("Silver coin value incorrect. ", state->coins == 2);
}

//Test Copper count
int threeCopperCoins_Plus3(struct gameState *state)
{

  //state->coins = 0;
  int player = 1;
  state->handCount[player] = 5;

  //setup state
  state->hand[player][0] = village;
  state->hand[player][1] = village;
  state->hand[player][2] = copper;
  state->hand[player][3] = copper;
  state->hand[player][4] = copper;

  updateCoins(player,state,0);
   

  return AssertCondition("Copper coin value incorrect. ", state->coins == 3);
}

//Test Bonus Count
int fiveBonusAdded_plus5(struct gameState *state)
{

  int player = 1;

  updateCoins(player,state,5);
   

  return AssertCondition("Bonus value incorrect. ", state->coins == 5);
}

//function has race condition, expected: none 
void raceCondition_NoneDetected()
{
  struct gameState G;
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};
  initializeGame(2, k, 2, &G);

  updateCoins(1, &G, 0);
}


int main (int argc, char** argv) {
  struct gameState G;
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};

  printf("\n**************  updateCoins() ***********\n");

  int success = 1;

  success &= AssertTest ("Test 0: Race Condition | Expected: None Detected ",
    TestRace(&raceCondition_NoneDetected));

  initializeGame(2, k, 2, &G);
  success &= AssertTest ("Test 1: Gold Coins Added = 2 | Expected: Coins = 6",
    twoGoldCoins_Plus6(&G));

  initializeGame(2, k, 2, &G);
  success &= AssertTest("Test 2: Silver Coins Added = 1 | Expected: Coins = 2", 
    oneSilverCoin_Plus2(&G));

  initializeGame(2, k, 2, &G);
  success &= AssertTest("Test 3: Copper Coins Added = 3 | Expected: Coins = 3", 
    threeCopperCoins_Plus3(&G));

  initializeGame(2, k, 2, &G);
  success &= AssertTest("Test 4: Bonus Added = 5 | Expected: Coins = 5",
    fiveBonusAdded_plus5(&G));

  if(success)
  {
    printf ("ALL TESTS PASSED\n");
  }
  printf ("\n");

  return 0;
}
