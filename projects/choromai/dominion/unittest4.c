#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include "utils.h"

//TESTING: gainCard()

//Test successful add of Province to deck
int addToDeckProvince_Success(struct gameState *state)
{
  int player = 0;
  state->supplyCount[province] = 1;

  int deckCountBefore = state->deckCount[player];

  gainCard(province,state, 1,player);

  int deckCountAfter = state->deckCount[player];

  int success = 1;

  success &= AssertCondition("Deck count incorrect.", 
  deckCountBefore + 1 == deckCountAfter);

  success &= AssertCondition("Card added is not province.", 
    state->deck[player][state->deckCount[player]-1] == province);

  success &= AssertCondition("Supply count incorrect.", 
    state->supplyCount[province] == 0);

  return success;
}

//Test try add Province to deck when pile empty
int addToDeckProvince_Fail(struct gameState *state)
{
  int result = 0; 
  int player = 0;

  state->supplyCount[province] = 0;

  int deckCountBefore = state->deckCount[player];
  
  result = gainCard(province,state,0,player);

  int deckCountAfter = state->deckCount[player];

  int success = 1;

  success &= AssertCondition("Deck count changed.", 
  deckCountBefore == deckCountAfter);

  success &= AssertCondition("Province pile count changed.", 
    state->supplyCount[province] == 0);

  success &= AssertCondition("Failed to detect empty province pile.", 
    result);

  return success;
}

//Add province to hand
int addToHandProvince_Success(struct gameState *state)
{
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

  int success = 1;

  success &= AssertCondition("Hand count incorrect.", 
    handCountBefore + 1 == handCountAfter);

  success &= AssertCondition("Actual card added not province",
    state->hand[player][state->handCount[player]-1] == province);

  success &= AssertCondition("Province supply count incorrect.",
    state->supplyCount[province] == 0);

  return success;
}

//Add province to discard pile 
int discardProvince_Success(struct gameState *state)
{
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

  int success = 1;

  success &= AssertCondition("Discard count incorrect.",
    discardCountBefore + 1 == discardCountAfter);

  success &= AssertCondition("Actual card not province",
    state->discard[player][state->discardCount[player]-1] == province);

  success &= AssertCondition("Province supply count incorrect.",
    state->supplyCount[province] == 0);

  return success;
}

int main (int argc, char** argv) {
  struct gameState G;
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};

  printf("\n**************  gainCard() ***********\n");

  int success = 1;

  initializeGame(2, k, 2, &G);
  success &= AssertTest("Test 1: Add Province To Deck | Expected: Deck+1, Supply-1, Card=Province",
    addToDeckProvince_Success(&G));

  initializeGame(2, k, 2, &G);
  success &= AssertTest("Test 2: Try Add Province when none available | Expected: Deck & Supply unchanged, Gain returns -1",
    addToDeckProvince_Fail(&G));

  initializeGame(2, k, 2, &G);
  success &= AssertTest("Test 3: Add Province To Hand | Expection: Hand Count + 1, Supply-1, Card=Province",
    addToHandProvince_Success(&G));

  initializeGame(2, k, 2, &G);
  success &= AssertTest("Test 4: Add Province To Discard Pile | Expected: Discard + 1, Supply-1; Card=Province",
    discardProvince_Success(&G));

  if(success)
  {
    printf ("ALL TESTS PASSED\n");
  }
  printf ("\n");

  return 0;
}
