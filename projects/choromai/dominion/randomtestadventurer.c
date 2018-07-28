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

int randNum(int size)
{
    int number = rand() % size;
    return number; 
}

int getRandomCard()
{
  //total number of different card types
  int random = rand() % 26;

  //account for 0;
  return random + 1;
}

void getPlayerDeckSizes(int array[])
{

  //plausible total cards
  const int total = 100;
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
        discardSize = total;
      }
    }
    //deck NOT empty 
    else
    {
      if(!discardEmpty)
      {
        deckSize = randNum(total);
        discardSize = total - deckSize; 
      }
      else
      {
        deckSize = total;
      }
    }
  }
  //hand
  else
  {
    //hand && discard
    if(!discardEmpty)
    {
      handSize = randNum(total); 
      //hand && deck && discard
      if(!deckEmpty)
      {
        deckSize = randNum(total - handSize); 
        discardSize = total - deckSize - handSize;
      }
      else
      {
        discardSize = total - handSize;
      }
    }
    else
    {
      if(!deckEmpty)
      {
        handSize = randNum(total); 
        deckSize = total - handSize;
      }
    }
  }

  array[0] = handSize;
  array[1] = deckSize;
  array[2] = discardSize;

}

//Deck has at least 3 cards, three coppers added, 7 cards total 
int randomTest(int k[])
{
  

  struct gameState state, before;
  state.numPlayers = 2 + randNum(3);

  initializeGame(2, k, 2, &state);

  int treasures[MAX_PLAYERS][2] = {0};
  int revealed[MAX_PLAYERS][100] = {0};
  int treasureCount[MAX_PLAYERS] = {0};
  int revealCount[MAX_PLAYERS] = {0};

  int j;

  for(j=0; j < state.numPlayers; j++)
  {

    int decks[3];
    getPlayerDeckSizes(decks);

    
    state.handCount[j] = decks[0];
    state.deckCount[j] = decks[1];
    state.discardCount[j] = decks[2];
    
    int sum = decks[0] + decks[1] + decks[2];
    printf("%d + %d + %d = %d\n", decks[0], decks[1], decks[2], sum);

    int i;

    //fill in reverse for easier counting of treasures
    for(i=state.handCount[j] - 1; i >= 0; i--)
    {
      state.hand[j][i] = getRandomCard();
    }

    for(i=state.deckCount[j] - 1; i >= 0; i--)
    {
      state.deck[j][i] = getRandomCard();

      if(treasures == 2)
      {
        continue;
      }

      if(state.deck[j][i] == gold || state.deck[j][i] == silver || state.deck[j][i])
      {
        treasures[j][treasureCount] = state.deck[j][i];
        treasureCount[j]++;
      }
      else
      {
        reveals[j][revealCount] = state.deck[j][i];
        revealCount[j]++;
      }
    }

    for(i=state.discardCount[j] - 1; i >= 0; i--)
    {
      state.discard[j][i] = getRandomCard();
    }

  }

  int player = randNum(3);
  int card = adventurer;
  int choice1 = 0; 
  int choice2 = 0;
  int choice3 = 0;
  int handPos = randNum(state.handCount[player]);
  int a = 0;
  int *bonus = &a;

  //record state before function
  memcpy[&before,&state, sizeof(struct gameState)];

  cardEffect(card, choice1, choice2, choice3, state, handPos, bonus);

  int i=0;
  int cardsInHand = state->handCount[player];
  int treasures = 0;
  int nextCard = 0;
  int coppersAdded = 0;

  int treasuresAfter = 0;
  //in hand before + after
  for(i=0; i < state->handCount[player]; i++)
  {
      nextCard = state->hand[player][i];
      if(nextCard == silver || nextCard == gold || nextCard == copper)
      {
        treasuresAfter++;
      }
  }

  int totalDiscards = state->discardCount[player];

  //THIS NEEDS TO COMPARE EVERY DISCARDED CARD, one by one, two arrays side by size
  //
  /*
  int k;
  int discartedVillageCount = 0;
  for(k=1; k <= 2; k++)
  {
    if(state->discard[player][totalDiscards-k] == village)
    {
        discartedVillageCount++;
    }
  }
  */

  int success = 1;

  /*
  //did we add 2 treasure card and discard 2 cards from the deck in the process
  success &= AssertCondition("Don't have 2 treasures", treasures == 2);
  //are the added treasures really coppers
  success &= AssertCondition("Don't have 2 coppers", coppersAdded == 2);
  //were 2 cards discarded
  success &= AssertCondition("Number of discarded cards is not 2", totalDiscards == 2);
  //are the discarded cards really village
  success &= AssertCondition("Number of discarded village cards in not 2", discartedVillageCount == 2);
  */

  /*
  Can write a very stellar and detailed reporting mechanism
  Player = 1;
  Player Hand Count = 37;
  ..etc
  */

  //return success;
  free(handPile);
  free(deckPile);
  free(discardPile);

  return 1;
}


int main (int argc, char** argv) {

  srand(time(NULL));

  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};

  int runs = 10;
  int i;
  for(i = 0; i < runs; i++)
  {
    randomTest(k);
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
