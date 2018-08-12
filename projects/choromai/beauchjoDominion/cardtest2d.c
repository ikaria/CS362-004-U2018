#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "dominion_helpers.h"

//TESTING: ADVENTURER CARD 

//Deck has at least 3 cards, three coppers added, 7 cards total 
int testGold(struct gameState *state)
{

  int player = 0;
  state->handCount[player] = 5;

  //setup state
  state->hand[player][0] = smithy;
  state->hand[player][1] = village;
  state->hand[player][2] = village;
  state->hand[player][3] = village;
  state->hand[player][4] = adventurer;

  int card = adventurer;
  int choice1 = 0; 
  int choice2 = 0;
  int choice3 = 0;
  int handPos = 4;
  int a = 0;
  int *bonus = &a;

  state->deck[player][0] = village; 
  state->deck[player][1] = copper; 
  state->deck[player][2] = village; 
  state->deck[player][3] = gold; 
  state->deck[player][4] = village; 

  cardEffect(card, choice1, choice2, choice3, state, handPos, bonus);

  int i=0;
  int cardsInHand = state->handCount[player];
  int treasures = 0;
  int nextCard = 0;
  int coppersAdded = 0;
  int goldAdded = 0;

  for(i=0; i < cardsInHand; i++)
  {
      nextCard = state->hand[player][i];
      if(nextCard == silver || nextCard == gold || nextCard == copper)
      {
        treasures++;
        if(nextCard == copper)
        {
          coppersAdded++;
        }
        if(nextCard == gold)
        {
            goldAdded++;
        }
      }
  }

  int totalDiscards = state->discardCount[player];

  int j;
  int discartedVillageCount = 0;
  for(j=1; j <= 2; j++)
  {
    if(state->discard[player][totalDiscards-j] == village)
    {
        discartedVillageCount++;
    }
  }

  int success = 1;

  //did we add 2 treasure card and discard 2 cards from the deck in the process
  success &= AssertCondition("Don't have 2 treasures", treasures == 2);
  //are the added treasures really coppers
  success &= AssertCondition("Don't have 1 copper, 1 gold", coppersAdded == 1 && goldAdded == 1);

  return success;
}


int main (int argc, char** argv) {
  struct gameState G;
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};

  printf("\n**************  cardtest2: Adventurer ***********\n");

  int success = 1;

  initializeGame(2, k, 2, &G);
  success &= AssertTest("Gold Treasure Test: Expected: 1 Gold and 1 Copper ",
    testGold(&G));

  if(success)
  {
    printf ("ALL TESTS PASSED\n");
  }
  printf ("\n");

  return 0;
}
