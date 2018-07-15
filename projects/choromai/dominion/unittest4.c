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

//test if province pile empty 
int provincePileIsEmpty_True(struct gameState *state)
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

  printf ("Test 1:  Province Pile is Empty  | Expection: True | Result: ");
  if(pass)
    printf ("PASSED\n");
  else
    printf ("FAILED\n");

  return pass;
}

//test if province pile empty 
int provincePileIsEmpty_False(struct gameState *state)
{
  int pass = 0;
  int result = 0; 
  int player = 0;

  result = gainCard(province,state,0,player);

  if (result == -1)
  {
      pass = 1;
  }

  printf ("Test 1:  Province Pile is Empty  | Expection: False | Result: ");
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
  int player = whoseTurn(&G);

  G.hand[player][0] = village;
  G.hand[player][1] = copper;
  G.hand[player][2] = copper;
  G.hand[player][3] = copper;
  G.hand[player][4] = smithy;

  success += provincePileIsEmpty_True(&G);
  success += provincePileIsEmpty_False(&G);

  if(success == 2)
  {
    printf ("ALL TESTS PASSED\n");
  }
  printf ("\n");

  return 0;
}
