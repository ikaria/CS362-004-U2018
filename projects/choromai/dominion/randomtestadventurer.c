#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <string.h>
#include "utils.h"
//#include "randomtestadventure.h"

//RANDOM TESTING: ADVENTURER CARD 
int const MAX_TREASURES = 2;
int const MAX_CARDS = 10;
int const STATS = 1;
int const RUNS = 1;

struct testRun
{
  int treasures[MAX_PLAYERS][2];
  int revealed[MAX_PLAYERS][10];
  int treasureCount[MAX_PLAYERS];
  int revealCount[MAX_PLAYERS];
  int handPos;
  int player;
};

void initRun(struct testRun *run)
{
  int i,j;

  for(int=0; i < MAX_PLAYERS; i++)
  {
    for(int=0; i < MAX_TREASURES)
    {
      run->treasures[i][j] = 0;
    }
  }

  for(int=0; i < MAX_PLAYERS; i++)
  {
    for(int=0; i < MAX_TREASURES)
    {
      run->revealed[i][j] = 0;
    }
  }

  for(int=0; i < MAX_PLAYERS; i++)
  {
    run->treasureCount[i] = 0;
  }

  for(int=0; i < MAX_PLAYERS; i++)
  {
    run->revealCount[i] = 0;
  }

}

//1 in pool chance of it being empty
int isEmpty(int pool)
{
    int empty = rand() % pool;
    return !empty; 
}

//generated random number within specified range
int randNum(int size)
{
    int number = rand() % size;
    return number; 
}

//picks card from available enums
int getRandomCard()
{
  //MAX_CARDSnumber of different card types
  int random = rand() % 26;

  //account for enums starting with 1;
  return random;
}

//generates random sizes of hand, deck and discard piles totalling 100
void getPlayerDeckSizes(int array[])
{

  //plausible MAX_CARDS
  int deckEmpty = isEmpty(5);
  int discardEmpty = isEmpty(5);

  int handSize = 0;
  int deckSize = 0;
  int discardSize = 0;

  //at least 1, to account for card to be played
  handSize = randNum(MAX_CARDS-1)+1; 

  if(!discardEmpty)
  {
    //hand && deck && discard
    if(!deckEmpty)
    {
      if(MAX_CARDS - handSize > 0)
      {
        deckSize = randNum(MAX_CARDS - handSize); 
      }
      else
      {
        deckSize = 0;
      }
      discardSize = MAX_CARDS - deckSize - handSize;
    }
    else
    {
      discardSize = MAX_CARDS - handSize;
    }
  }
  else
  {
    if(!deckEmpty)
    {
      deckSize = MAX_CARDS - handSize;
    }
  }
  
  array[0] = handSize;
  array[1] = deckSize;
  array[2] = discardSize;

}

int fillDecks(int treasures[][2], int revealed[][10], int treasureCount[], int revealCount[], struct gameState *state)
{
  //random 2-4 players
  state->numPlayers = 2 + randNum(3);
  int player = randNum(state->numPlayers);

  int handPos = 0;
  //state->hand[player][handPos] = adventurer;

  int i,j;
  for(j=0; j < state->numPlayers; j++)
  {

    int decks[3];
    getPlayerDeckSizes(decks);

    if(STATS)
    {
      int sum = decks[0] + decks[1] + decks[2];
      printf("Player: %d -- %d + %d + %d = %d\n", j, decks[0], decks[1], decks[2], sum);
    }
    
    state->handCount[j] = decks[0];
    state->deckCount[j] = decks[1];
    state->discardCount[j] = decks[2];

    //set position of played card
    if(j == player)
    {
      if(state->handCount[player] > 1)
      {
        handPos = randNum(state->handCount[player]);
      }
    }

    //fill in reverse for easier counting of treasures amidst reveals
    for(i=state->handCount[j] - 1; i >= 0; i--)
    {
      state->hand[j][i] = getRandomCard();
    }

    for(i=state->deckCount[j] - 1; i >= 0; i--)
    {
      state->deck[j][i] = getRandomCard();

      if(treasureCount[j] == 2)
        continue;

      //add particular card to treasures, augment count
      if(state->deck[j][i] == gold || state->deck[j][i] == silver || state->deck[j][i] == copper)
      {
        treasures[j][treasureCount[j]] = state->deck[j][i];
        treasureCount[j]++;
      }
      //if not a treasure, then a reveal
      else
      {
        revealed[j][revealCount[j]] = state->deck[j][i];
        revealCount[j]++;
      }
    }

    //fill up discards
    for(i=state->discardCount[j] - 1; i >= 0; i--)
    {
      state->discard[j][i] = getRandomCard();
    }
  }

  printf("reveal count: %d\n", revealCount[player]);
  printf("treasure count: %d\n", treasureCount[player]);


  if(!STATS)
    return player;

  for(j=0; j<state->numPlayers; j++)
  {
    printf("\n****************************\n");
    printf("Player: %d\n", j);

    printf("Hand --\n");
    for(i=0; i < state->handCount[j]; i++)
    {
      printf("%d: %d\n", i, state->hand[j][i]);
    }

    printf("Deck --\n");
    for(i=0; i < state->deckCount[j]; i++)
    {
      printf("%d: %d\n", i, state->deck[j][i]);
    }

    printf("Discards --\n");
    for(i=0; i < state->discardCount[j]; i++)
    {
      printf("%d: %d\n", i, state->discard[j][i]);
    }
  }

  return player;
}

//executes single instance of test
int randomTest(int k[])
{

  struct gameState state, before;
  struct testRun run;

  initializeGame(2, k, 2, &state);

  int treasures[MAX_PLAYERS][2] = {0};
  int revealed[MAX_PLAYERS][10] = {0};
  int treasureCount[MAX_PLAYERS] = {0};
  int revealCount[MAX_PLAYERS] = {0};

  int player = fillDecks(treasures, revealed, treasureCount, revealCount, &state);

  printf("Current Player: %d\n",player);
  int card = adventurer;
  int choice1 = 0; 
  int choice2 = 0;
  int choice3 = 0;
  int handPos = 0;
  int a = 0;
  int *bonus = &a;

  if(STATS)
  {
    int i;
    printf("Hand with Card --\n");
    printf("HandPos:%d\n",handPos);

    for(i=0; i < state.handCount[player]; i++)
    {
      printf("%d: %d\n", i, state.hand[player][i]);
    }
  }

  //record state before function
  memcpy(&before, &state, sizeof(struct gameState));

  cardEffect(card, choice1, choice2, choice3, &state, handPos, bonus);

  int i=0;
  int cardsInHand = state.handCount[player];
  int coppersAdded = 0;
  int treasuresAfter = 0;
  int treasuresBefore = 0;

  int nextCard = 0;
  printf("BEFORE Player: %d\n",player);
  for(i=0; i < before.handCount[player]; i++)
  {
      nextCard = before.hand[player][i];
      printf("%d: %d\n",i,before.hand[player][i]);
      if(nextCard == silver || nextCard == gold || nextCard == copper)
      {
        treasuresBefore++;
      }
  }

  //in hand before + after
  nextCard = 0;
  printf("AFTER Player: %d\n",player);
  printf("count: %d\n",state.handCount[player]);
  for(i=0; i < state.handCount[player]; i++)
  {
      nextCard = state.hand[player][i];
      printf("%d: %d\n",i,before.hand[player][i]);
      if(nextCard == silver || nextCard == gold || nextCard == copper)
      {
        treasuresAfter++;
      }
  }


  if(STATS)
  {
    printf("Before: %d + Added: %d = After: %d\n", treasuresBefore, treasureCount[player], treasuresAfter);
  }

  int totalDiscards = state.discardCount[player];

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

  return 1;
}


int main (int argc, char** argv) {

  srand(time(NULL));

  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};

  int i;
  for(i = 0; i < RUNS; i++)
  {
    if(STATS)
    {
      printf("************************\n");
      printf("RUN: %d\n",i);
    }
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
