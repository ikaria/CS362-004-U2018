#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include "race.h"


//Test Gold count
int twoGoldCoins_Plus6(struct gameState *state)
{
  int pass = 0;

  //state->coins = 0;
  int player = 1;
  state->handCount[player] = 5;

  //setup state
  state->hand[player][0] = village;
  state->hand[player][1] = village;
  state->hand[player][2] = village;
  state->hand[player][3] = gold;
  state->hand[player][4] = gold;

  updateCoins(player,state,0);
   
  if(state->coins == 6)
  {
    pass = 1;
  }

  printf ("Test 1: Gold Coins Added = 2 | Expection: Coins = 6 | Result: ");
  if(pass)
    printf ("PASSED\n");
  else
    printf ("FAILED\n");

  return pass;
}


//Test Silver count
int oneSilverCoin_Plus2(struct gameState *state)
{
  int pass = 0;

  //state->coins = 0;
  int player = 1;
  state->handCount[player] = 5;

  //setup state
  state->hand[player][0] = village;
  state->hand[player][1] = village;
  state->hand[player][2] = village;
  state->hand[player][3] = village;
  state->hand[player][4] = silver;

  updateCoins(player,state,0);
   
  if(state->coins == 2)
  {
    pass = 1;
  }

  printf ("Test 2: Silver Coins Added = 1 | Expection: Coins = 2 | Result: ");
  if(pass)
    printf ("PASSED\n");
  else
    printf ("FAILED\n");

  return pass;
}

//Test Copper count
int threeCopperCoins_Plus3(struct gameState *state)
{
  int pass = 0;

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
   
  if(state->coins == 3)
  {
    pass = 1;
  }

  printf ("Test 3: Copper Coins Added = 3 | Expection: Coins = 3 | Result: ");
  if(pass)
    printf ("PASSED\n");
  else
    printf ("FAILED\n");

  return pass;
}

//Test Bonus Count
int fiveBonusAdded_plus5(struct gameState *state)
{
  int pass = 0;

  //state->coins = 0;
  int player = 1;

  updateCoins(player,state,5);
   
  if(state->coins == 5)
  {
    pass = 1;
  }

  printf ("Test 4: Bonus Added = 5 | Expection: Coins = 5 | Result: ");
  if(pass)
    printf ("PASSED\n");
  else
    printf ("FAILED\n");

  return pass;
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

  int success = 0;

  success += TestRace(&raceCondition_NoneDetected);

  printf("Test 0: Race Condition | Expectation: None Detected | Result: ");
  if(success == 1)
    printf("PASSED\n");
  else
    printf("FAILED\n");

  initializeGame(2, k, 2, &G);
  success += twoGoldCoins_Plus6(&G);

  initializeGame(2, k, 2, &G);
  success += oneSilverCoin_Plus2(&G);

  initializeGame(2, k, 2, &G);
  success += threeCopperCoins_Plus3(&G);

  initializeGame(2, k, 2, &G);
  success += fiveBonusAdded_plus5(&G);

  if(success == 5)
  {
    printf ("ALL TESTS PASSED\n");
  }
  printf ("\n");

  return 0;
}
