#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <string.h>
#include "race.h"

//TESTING: ADVENTURER CARD 

//2 treasure cards added to hand / for i to cards in hand treature +2
//revealed cards discarded / 2 other cards, followed by 2 treasure cards

//Deck has at least 3 cards, three coppers added, 7 cards total 
int twoTreasureCardsInDeck_twoCardsDiscardedtwoTreasuresFound(struct gameState *state)
{
  int pass = 0;

  //state->coins = 0;
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
  int *bonus;

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

  for(i=0; i < cardsInHand; i++)
  {
      nextCard = state->hand[player][i];
      if(nextCard == silver || nextCard == gold || nextCard == copper)
      {
        treasures++;
      }
  }

  int totalDiscards = state->discardCount[player];

  //did we add 1 treasure card and discard 4 cards from the deck in the process
  if(treasures == 2 && totalDiscards == 2)
  {
      int j;
      int discartedVillageCount = 0;
      for(j=1; j <= 2; j++)
      {
        //are the discarded cards really village
        if(state->discard[player][totalDiscards-j] == village)
        {
            discartedVillageCount++;
        }
      }
      if(discartedVillageCount == 2)
      {
          pass = 1;
      }
  }

  printf ("Test 1: 2 Treasures in Deck | Expection: 2 coppers added to hand, 2 village cards discarded | Result: ");
  if(pass)
    printf ("PASSED\n");
  else
    printf ("FAILED\n");

  return pass;
}

int oneTreasureCardInDeck_fourCardsDiscardedoneTreasureFound(struct gameState *state)
{
  int pass = 0;

  //state->coins = 0;
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
  int *bonus;

  //add 1 treasures and 2 other cards to deck
  state->supplyCount[copper] = 1;
  state->supplyCount[village] = 2;

  state->deck[player][0] = copper; 
  state->deck[player][1] = village; 
  state->deck[player][2] = village; 
  state->deck[player][3] = village; 
  state->deck[player][4] = village; 

  cardEffect(card, choice1, choice2, choice3, state, handPos, bonus);

  int i=0;
  int cardsInHand = state->handCount[player];
  int treasures = 0;
  int nextCard = 0;

  for(i=0; i < cardsInHand; i++)
  {
      nextCard = state->hand[player][i];
      if(nextCard == silver || nextCard == gold || nextCard == copper)
      {
        treasures++;
      }
  }

  int totalDiscards = state->discardCount[player];

  //did we add 1 treasure card and discard 4 cards from the deck in the process
  if(treasures == 1 && totalDiscards == 4)
  {
      int j;
      int discartedVillageCount = 0;
      for(j=1; j <= 4; j++)
      {
        //are the discarded cards really village
        if(state->discard[player][totalDiscards-j] == village)
        {
            discartedVillageCount++;
        }
      }
      if(discartedVillageCount == 4)
      {
          pass = 1;
      }
  }

  printf ("Test 1: 1 Treasures in Deck | Expection: 1 copper added to hand, 4 village cards discarded | Result: ");
  if(pass)
    printf ("PASSED\n");
  else
    printf ("FAILED\n");

  return pass;
}

int main (int argc, char** argv) {
  struct gameState G, emptyG;
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};

  printf("\n**************  cardtest2: Adventurer ***********\n");

  int success = 0;

  initializeGame(2, k, 2, &G);
  success += twoTreasureCardsInDeck_twoCardsDiscardedtwoTreasuresFound(&G);

  initializeGame(2, k, 2, &G);
  memcpy(&emptyG, &G, sizeof(struct gameState));
  success += oneTreasureCardInDeck_fourCardsDiscardedoneTreasureFound(&G);

  if(success == 2)
  {
    printf ("ALL TESTS PASSED\n");
  }
  printf ("\n");

  return 0;
}
