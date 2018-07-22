#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include "utils.h"

//TESTING: handCard()

//Test retriving first card, expected: Village
int getFirstCard_village(int handPos, struct gameState *state)
{
  int card = handCard(handPos, state);
  return AssertCondition("Incorrect card at pos 0", card == village);
}


//Test retriving last card, expected: Smithy 
int getLastCard_smithy(int handPos, struct gameState *state)
{
  int card = handCard(handPos, state);
  return AssertCondition("Incorrect card at pos 4", card == smithy);
}

//Test attempt to get card at position larger than hand size
int getOutOfRange_None(int handPos, struct gameState *state)
{
  int card = handCard(handPos, state);
  return AssertCondition("Out of range card has value, but shouldn't", card < 0 || card > 26);
}

int main (int argc, char** argv) {
  struct gameState G;
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};

  printf("\n**************  handCard() ***********\n");

  int success = 1;

  initializeGame(2, k, 2, &G);
  int player = whoseTurn(&G);

  //setup
  G.hand[player][0] = village;
  G.hand[player][1] = copper;
  G.hand[player][2] = copper;
  G.hand[player][3] = copper;
  G.hand[player][4] = smithy;

  G.handCount[player] = 5;

  success &= AssertTest("Test 1: Get card in position: 1 | Expected: Village",
    getFirstCard_village(0,&G));

  success &= AssertTest("Test 2: Get card in position: 5 | Expected: Smithy",
   getLastCard_smithy(4,&G));

  success &= AssertTest("Test 3: Try card out of range | Expected: invalid enum",
   getOutOfRange_None(7,&G));

  if(success)
  {
    printf ("ALL TESTS PASSED\n");
  }
  printf ("\n");

  return 0;
}
