#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <string.h>
#include "utils.h"
//#include "randomtestadventure.h"

//RANDOM TESTING:  COUNCIL ROOM CARD 
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
  int i = 0;
  for(i=0; i<4; i++)
  run->handPos = 0;
  run->card = council_room;
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
      printf("Player: %d -- Hand: %d + Deck: %d + Discard: %d\n", j, decks[0] +5, decks[1] + 5, decks[2] + 5);
    }
    
    state->handCount[j] = decks[0] + 5;
    state->deckCount[j] = decks[1] + 5;
    state->discardCount[j] = decks[2] + 5;

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

  if(LOG)
  {
    printf("DECK AFTER: %d\n", state.deckCount[0]);
    printf("HAND AFTER: %d\n", state.handCount[0]);
    printf("DISCARD AFTER: %d\n", state.discardCount[0]);
  }

  int i;
  int cardsAddedCorrect = 1;
  for(i=0; i<4;i++)
  {
      if(i == 0)
      {
        if(before.deck[run.player][before.deckCount[run.player]-4] != state.hand[run.player][run.handPos])
        {
            cardsAddedCorrect &= 0;
        }
      }
      else
      {
        if(before.deck[run.player][before.deckCount[run.player]- 4 + i] != 
            state.hand[run.player][state.handCount[run.player]- i])
        {
            cardsAddedCorrect &= 0;
        }
      }

    if(LOG)
    {
        printf("card at handpos: %d\n", state.hand[run.player][run.handPos]);

        printf("**** *******\n");
        int j;
        for(j=0; j<before.deckCount[i]; j++)
        {
            printf("d: %d\n", before.deck[i][j]);
        }

        for(j=0; j<before.handCount[i]; j++)
        {
            printf("h: %d\n", before.hand[i][j]);
        }
    }
  }

  int opponentCardCountCorrect = 1;
  int opponentCardTypeCorrect = 1;

  for(i=0; i<state.numPlayers; i++)
  {
      if(i != run.player)
      {
          if(before.handCount[i] + 1 != state.handCount[i])
          {
              opponentCardCountCorrect &= 0;
          }
          if(state.hand[i][state.handCount[i]-1] != before.deck[i][before.deckCount[i]-1])
          {
              opponentCardTypeCorrect &= 0;
          }

      }
  }

  
  int success = 1;

  success &= AssertCondition("Incorrect number of cards played", state.playedCardCount);

  success &= AssertCondition("Discarded card incorrect", 
    state.playedCards[0] == council_room);

  success &= AssertCondition("Number of buys added not ", 
    state.numBuys == before.numBuys + 1);

  success &= AssertCondition("Number of cards in hand incorrect", 
    before.handCount[run.player] + 3 == state.handCount[run.player]);

  success &= AssertCondition("Cards added to hand incorrect", cardsAddedCorrect);

  success &= AssertCondition("Number of cards given to opponents incorrect", opponentCardCountCorrect);


  success &= AssertCondition("Cards given to opponents incorrect", opponentCardTypeCorrect);

  char params[100];
  sprintf(params, "TEST: Current Player: %d | 4 cards added, 1 buy added, opponents received cards",
  run.player);  

  AssertTest(params, success);

  return 0;
}


int main (int argc, char** argv) {

  srand(time(NULL));

  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};

  printf("\n**************  randomtest3: Council Room ***********\n");

  int i;
  for(i = 0; i < RUNS; i++)
  {
    if(STATS)
    {
      printf("RUN: %d\n",i+1);
    }
    randomTest(k);
    if(STATS)
    {
      printf("************************\n");
    }
  }

  return 0;
}
