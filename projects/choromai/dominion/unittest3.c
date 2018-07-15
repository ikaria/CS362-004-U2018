#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include "race.h"

//TESTING: handCard()

//Test retriving first card, expected: Village
int getFirstCard_village(int handPos, struct gameState *state)
{
  int pass = 0;

  //setup state

  int card = handCard(handPos, state);

  if (card == village)
  {
      pass = 1;
  }

  printf ("Test 1: Get card in position: 1 | Expection: Village | Result: ");
  if(pass)
    printf ("PASSED\n");
  else
    printf ("FAILED\n");

  return pass;
}


//Test retriving last card, expected: Smithy 
int getLastCard_smithy(int handPos, struct gameState *state)
{
  int pass = 0;

  //setup state

  int card = handCard(handPos, state);

  if (card == smithy)
  {
      pass = 1;
  }

  printf ("Test 2: Get card in position: 5 | Expection: Smithy | Result: ");
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

  printf("\n**************  handCard() ***********\n");

  int success = 0;

  initializeGame(2, k, 2, &G);
  int player = whoseTurn(&G);

  G.hand[player][0] = village;
  G.hand[player][1] = copper;
  G.hand[player][2] = copper;
  G.hand[player][3] = copper;
  G.hand[player][4] = smithy;

  success += getFirstCard_village(0,&G);
  success += getLastCard_smithy(4,&G);

  if(success == 2)
  {
    printf ("ALL TESTS PASSED\n");
  }
  printf ("\n");

  return 0;
}
