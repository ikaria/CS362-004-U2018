#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <string.h>
#include "utils.h"

//RANDOM TESTING: ADVENTURER CARD 

//1 in pool chance of it being empty
int isEmpty(int pool)
{
    int empty = rand() % pool;
    return !empty; 
}

int pileSize(int size)
{
    int pile = rand() % size;
    return pile; 
}

void buildPiles(int array[])
{

  int handEmpty = isEmpty(5);
  int deckEmpty = isEmpty(5);
  int discardEmpty = isEmpty(5);

  int handSize = 0;
  int deckSize = 0;
  int discardSize = 0;

  if(handEmpty)
  {
    if(deckEmpty)
    {
      if(!discardEmpty)
      {
        discardSize = MAX_HAND;
      }
    }
    //deck NOT empty 
    else
    {
      if(!discardEmpty)
      {
        deckSize = pileSize(MAX_HAND);
        discardSize = MAX_HAND - deckSize; 
      }
      else
      {
        deckSize = MAX_HAND;
      }
    }
  }
  //hand
  else
  {
    //hand && discard
    if(!discardEmpty)
    {
      handSize = pileSize(MAX_HAND); 
      //hand && deck && discard
      if(!deckEmpty)
      {
        deckSize = pileSize(MAX_HAND - handSize); 
        discardSize = MAX_HAND - deckSize - handSize;
      }
      else
      {
        discardSize = MAX_HAND - handSize;
      }
    }
    else
    {
      if(!deckEmpty)
      {
        handSize = pileSize(MAX_HAND); 
        deckSize = MAX_HAND - handSize;
      }
    }
  }

  array[0] = handSize;
  array[1] = deckSize;
  array[2] = discardSize;

}

//Deck has at least 3 cards, three coppers added, 7 cards total 
int randomTest(struct gameState *state)
{
  
  int piles[3];
  buildPiles(piles);
  int sum = piles[0] + piles[1] + piles[2];
  printf("%d + %d + %d = %d\n", piles[0], piles[1], piles[2], sum);

  /*
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

  //add 2 treasures and 2 other cards to deck
  state->supplyCount[copper] = 2;
  state->supplyCount[village] = 2;

  state->deck[player][0] = copper; 
  state->deck[player][1] = copper; 
  state->deck[player][2] = village; 
  state->deck[player][3] = copper; 
  state->deck[player][4] = village; 

  cardEffect(card, choice1, choice2, choice3, state, handPos, bonus);

  int i=0;
  int cardsInHand = state->handCount[player];
  int treasures = 0;
  int nextCard = 0;
  int coppersAdded = 0;

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
  success &= AssertCondition("Don't have 2 coppers", coppersAdded == 2);
  //were 2 cards discarded
  success &= AssertCondition("Number of discarded cards is not 2", totalDiscards == 2);
  //are the discarded cards really village
  success &= AssertCondition("Number of discarded village cards in not 2", discartedVillageCount == 2);
  */

  //return success;
  return 1;
}


int main (int argc, char** argv) {

  srand(time(NULL));

  struct gameState G, emptyG;
  int runs = 10;
  int i;
  for(i = 0; i < runs; i++)
  {
    randomTest(&G);
  }

  /*
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};

  printf("\n**************  cardtest2: Adventurer ***********\n");

  int success = 1;

  initializeGame(2, k, 2, &G);
  success &= AssertTest("Test 1: 2 Treasures in Deck | Expected: 2 coppers added to hand, 2 village cards discarded",
    twoTreasureCardsInDeck_twoCardsDiscardedtwoTreasuresFound(&G));

  if(success)
  {
    printf ("ALL TESTS PASSED\n");
  }
  printf ("\n");
  */

  return 0;
}
