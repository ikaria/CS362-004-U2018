#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include "race.h"

//TESTING: gainCard()

//test if discarded
//test if added to deck
//test if added to hand
//check if supply pile reduced

//Test successful add of Province to deck
int addToDeckProvince_Success(struct gameState *state)
{
  int pass = 0;
  int player = 0;
  state->supplyCount[province] = 1;

  int deckCountBefore = state->deckCount[player];

  gainCard(province,state, 1,player);

  int deckCountAfter = state->deckCount[player];

  if (deckCountBefore + 1 == deckCountAfter)
  {
      if(state->deck[player][state->deckCount[player]-1] == province)
      {
          if(state->supplyCount[province] == 0)
          {
            pass = 1;
          }
      }
  }

  printf ("Test 1: Add Province To Deck | Expection: Success | Result: ");
  if(pass)
    printf ("PASSED\n");
  else
    printf ("FAILED\n");

  return pass;
}

//Test failed add of Province to deck when pile empty
int addToDeckProvince_Fail(struct gameState *state)
{
  int pass = 0;
  int result = 0; 
  int player = 0;

  state->supplyCount[province] = 0;

  result = gainCard(province,state,0,player);

  if (result == -1)
  {
      pass = 1;
  }

  printf ("Test 2: Try Add Province Pile Empty | Expection: Failure | Result: ");
  if(pass)
    printf ("PASSED\n");
  else
    printf ("FAILED\n");

  return pass;
}

//Add province to hand
int addToHandProvince_Success(struct gameState *state)
{
  int pass = 0;
  int result = 0; 
  int player = 0;

  state->hand[player][0] = village;
  state->hand[player][1] = copper;
  state->hand[player][2] = copper;
  state->hand[player][3] = copper;
  state->hand[player][4] = smithy;

  state->supplyCount[province] = 1;

  int handCountBefore = state->handCount[player];

  gainCard(province,state, 2,player);

  int handCountAfter = state->handCount[player];

  if (handCountBefore + 1 == handCountAfter)
  {
      if(state->hand[player][state->handCount[player]-1] == province)
      {
          if(state->supplyCount[province] == 0)
          {
            pass = 1;
          }
      }
  }

  printf ("Test 2: Add Province To Hand | Expection: Success | Result: ");
  if(pass)
    printf ("PASSED\n");
  else
    printf ("FAILED\n");

  return pass;
}

//Add province to discard pile 
int discardProvince_Success(struct gameState *state)
{
  int pass = 0;
  int result = 0; 
  int player = 0;

  state->hand[player][0] = province;
  state->hand[player][1] = copper;
  state->hand[player][2] = copper;
  state->hand[player][3] = copper;
  state->hand[player][4] = smithy;

  state->supplyCount[province] = 1;

  int discardCountBefore = state->discardCount[player];

  gainCard(province,state, 0,player);

  int discardCountAfter = state->discardCount[player];

  if (discardCountBefore + 1 == discardCountAfter)
  {
      if(state->discard[player][state->discardCount[player]-1] == province)
      {
          if(state->supplyCount[province] == 0)
          {
            pass = 1;
          }
      }
  }

  printf ("Test 2: Add Province To Discard Pile | Expection: Success | Result: ");
  if(pass)
    printf ("PASSED\n");
  else
    printf ("FAILED\n");

  return pass;
}

int main (int argc, char** argv) {
  struct gameState G;
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};

  printf("\n**************  gainCard() ***********\n");

  int success = 0;

  initializeGame(2, k, 2, &G);
  success += addToDeckProvince_Success(&G);

  initializeGame(2, k, 2, &G);
  success += addToDeckProvince_Fail(&G);

  initializeGame(2, k, 2, &G);
  success += addToHandProvince_Success(&G);

  initializeGame(2, k, 2, &G);
  success += discardProvince_Success(&G);

  if(success == 4)
  {
    printf ("ALL TESTS PASSED\n");
  }
  printf ("\n");

  return 0;
}
