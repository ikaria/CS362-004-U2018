#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <string.h>
#include "utils.h"
//#include "randomtestadventure.h"

//RANDOM TESTING: VILLAGE CARD 
int const MAX_TREASURES = 2;
int const MAX_CARDS = 10;
int const STATS = 1;
int const LOG = 0;
int const RUNS = 100;

struct testRun
{
  int handPos;
  int player;
  int card;
  int numPlayers;
};


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

void initRun(struct testRun *run, int numPlayers)
{
  run->handPos = 0;
  run->card = village;
  //random 2-4 players
  run->player = randNum(numPlayers);
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
    else
    {
      handSize = 10;
    }
  }
  
  array[0] = handSize;
  array[1] = deckSize;
  array[2] = discardSize;

}

void fillDecks(struct testRun *run, struct gameState *state)
{

  int i,j;
  for(j=0; j < state->numPlayers; j++)
  {

    int decks[3];
    getPlayerDeckSizes(decks);

    if(STATS)
    {
      printf("Player: %d -- Hand: %d + Deck: %d + Discard: %d\n", j, decks[0]+5, decks[1], decks[2]);
    }
    
    state->handCount[j] = decks[0] + 5;
    state->deckCount[j] = decks[1] +5;
    state->discardCount[j] = decks[2];

    //set position of played card, defaults to 0 otherwise
    if(j == run->player)
    {
      if(state->handCount[run->player] > 1)
      {
        run->handPos = randNum(state->handCount[run->player]);
      }
    }

    //fill in reverse for easier counting of treasures amidst reveals
    for(i=state->handCount[j] - 1; i >= 0; i--)
    {
      state->hand[j][i] = getRandomCard();
      //seed village card
      if(run->handPos == i && j == run->player)
      {
        state->hand[j][i] = run->card;
      }
    }

    for(i=state->deckCount[j] - 1; i >= 0; i--)
    {
      state->deck[j][i] = getRandomCard();
    }

    //fill up discards
    for(i=state->discardCount[j] - 1; i >= 0; i--)
    {
      state->discard[j][i] = getRandomCard();
    }
  }

  if(LOG)
  {
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
  }
}

//executes single instance of test
int randomTest(int k[])
{
  struct gameState state, before;
  struct testRun run;

  run.numPlayers = 2 + randNum(3);
  initializeGame(run.numPlayers, k, 2, &state);
  initRun(&run, run.numPlayers);

  fillDecks(&run, &state);
  state.whoseTurn = run.player;

  int choice1 = 0; 
  int choice2 = 0;
  int choice3 = 0;
  int a = 0;
  int *bonus = &a;

  if(LOG)
  {
    int i;
    printf("Hand with Card --\n");
    printf("HandPos:%d\n", run.handPos);

    for(i=0; i < state.handCount[run.player]; i++)
    {
      printf("%d: %d\n", i, state.hand[run.player][i]);
    }
  }

  //record state before function
  memcpy(&before, &state, sizeof(struct gameState));

  cardEffect(run.card, choice1, choice2, choice3, &state, run.handPos, bonus);
  //cardEffect(run.card, choice1, choice2, choice3, &state, run.handPos, bonus);

  if(LOG)
  {
    printf("DECK AFTER: %d\n",state.deckCount[0]);
    printf("HAND AFTER: %d\n",state.handCount[0]);
    printf("DISCARD AFTER: %d\n",state.handCount[0]);
  }
  
  int success = 1;

  success &= AssertCondition("Incorrect number of cards played", 
    state.playedCardCount == 1);

  success &= AssertCondition("Played card incorrect", 
    //state.discard[run.player][state.discardCount[run.player] - 1] == village);
    state.playedCards[0] == village);

  success &= AssertCondition("Number of actions added not 2", 
    state.numActions == before.numActions + 2);

  success &= AssertCondition("Card added to hand is incorrect", 
    state.hand[run.player][state.handCount[run.player]-1] == before.deck[run.player][before.deckCount[run.player]-1]);

  success &= AssertCondition("Number of cards in hand incorrect", 
    before.handCount[run.player] == state.handCount[run.player]);


  char params[100];
  sprintf(params, "TEST: Current Player: %d | Card Added: %d | HandPos: %d", 
  run.player, before.deck[run.player][before.deckCount[run.player]-1], run.handPos);  
  

  AssertTest(params, success);

  return 0;
}


int main (int argc, char** argv) {

  srand(time(NULL));

  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};

  printf("\n**************  randomtest1: Village ***********\n");

  int i;
  for(i = 0; i < RUNS; i++)
  {
    if(STATS)
    {
      printf("RUN: %d\n",i);
    }
    randomTest(k);
    if(STATS)
    {
      printf("************************\n");
    }
  }

  return 0;
}
