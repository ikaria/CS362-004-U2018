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
int const LOG = 0;
int const RUNS = 1;

struct testRun
{
  int treasures[MAX_PLAYERS][2];
  int revealed[MAX_PLAYERS][100];
  int treasureCount[MAX_PLAYERS];
  int revealCount[MAX_PLAYERS];
  int handPos;
  int player;
  int card;
};

void initRun(struct testRun *run)
{
  int i,j;

  for(i=0; i < MAX_PLAYERS; i++)
  {
    for(j=0; j < MAX_TREASURES; j++)
    {
      run->treasures[i][j] = 0;
    }
  }

  for(i=0; i < MAX_PLAYERS; i++)
  {
    for(j=0; j < MAX_CARDS; j++)
    {
      run->revealed[i][j] = 0;
    }
  }

  for(i=0; i < MAX_PLAYERS; i++)
  {
    run->treasureCount[i] = 0;
  }

  for(i=0; i < MAX_PLAYERS; i++)
  {
    run->revealCount[i] = 0;
  }

  run->handPos = 0;
  run->player = 0;

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
      printf("Player: %d -- Hand: %d + Deck: %d + Discard: %d\n", j, decks[0] + 5, decks[1] + 5, decks[2] +5);
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

      if(j != run->player)
        continue;

      if(run->treasureCount[j] < 2)
      {
        //add particular card to treasures, augment count
        if(state->deck[j][i] == gold || state->deck[j][i] == silver || state->deck[j][i] == copper)
        {
          run->treasures[j][run->treasureCount[j]] = state->deck[j][i];
          run->treasureCount[j]++;
        }
        //if not a treasure, then a reveal
        else
        {
          run->revealed[j][run->revealCount[j]] = state->deck[j][i];
          run->revealCount[j]++;
        }
      }

    }

    //fill up discards
    for(i=state->discardCount[j] - 1; i >= 0; i--)
    {
      state->discard[j][i] = getRandomCard();

      if(j != run->player)
        continue;

      if(run->treasureCount[j] < 2)
      {
        //add particular card to treasures, augment count
        if(state->discard[j][i] == gold || state->discard[j][i] == silver || state->discard[j][i] == copper)
        {
          run->treasures[j][run->treasureCount[j]] = state->discard[j][i];
          run->treasureCount[j]++;
        }
        //if not a treasure, then a reveal
        else
        {
          run->revealed[j][run->revealCount[j]] = state->discard[j][i];
          run->revealCount[j]++;
        }
      }
    }
  }

  if(LOG)
  {
    printf("reveal count: %d\n", run->revealCount[run->player]);
    printf("treasure count: %d\n", run->treasureCount[run->player]);
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

  int numPlayers = 2 + randNum(3);
  initializeGame(numPlayers, k, 2, &state);
  //initRun(&run);
  run.card = adventurer;

  //random 2-4 players
  run.player = randNum(state.numPlayers);

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
  //memcpy(&before, &state, sizeof(struct gameState));

  cardEffect(run.card, choice1, choice2, choice3, &state, run.handPos, bonus);

  if(LOG)
  {
    printf("DECK AFTER: %d\n",state.deckCount[0]);
    printf("HAND AFTER: %d\n",state.handCount[0]);
    printf("DISCARD AFTER: %d\n",state.handCount[0]);
  }
  
  int i=0;
  int treasuresAfter = 0;
  int treasuresBefore = 0;

  int nextCard = 0;
  if(LOG)
    printf("Hand BEFORE Player: %d\n",run.player);
  for(i=0; i < before.handCount[run.player]; i++)
  {
      nextCard = before.hand[run.player][i];
      if(LOG)
        printf("%d: %d\n",i,before.hand[run.player][i]);
      if(nextCard == silver || nextCard == gold || nextCard == copper)
      {
        treasuresBefore++;
      }
  }

  //in hand before + after
  nextCard = 0;
  if(LOG)
  {
    printf("Hand AFTER Player: %d\n",run.player);
    printf("hand count: %d\n",state.handCount[run.player]);
  }
  for(i=0; i < state.handCount[run.player]; i++)
  {
      nextCard = state.hand[run.player][i];
      if(LOG)
        printf("%d: %d\n",i,state.hand[run.player][i]);
      if(nextCard == silver || nextCard == gold || nextCard == copper)
      {
        treasuresAfter++;
      }
  }


  if(LOG)
  {
    printf("Before: %d + Added: %d = After: %d\n", treasuresBefore, run.treasureCount[run.player], treasuresAfter);
  }

  int correctTreasureCount = 0;
  if(treasuresBefore + run.treasureCount[run.player] == treasuresAfter)
  {
    correctTreasureCount = 1;
  }


  int correctDiscardCount = 0;
  if(run.revealCount[run.player] <= before.deckCount[run.player])
  {
    if(state.discardCount[run.player] == run.revealCount[run.player] + before.discardCount[run.player])
    {
      correctDiscardCount = 1;
    }
  }
  else
  {
    if(before.discardCount[run.player] + before.deckCount[run.player] - run.revealCount[run.player] == state.discardCount[run.player])
    {
      correctDiscardCount = 1;
    }
  }

  int n;
  int match [2];
  int treasureTypeNotIncorrect = 0;
  for(n=0; n < run.treasureCount[run.player]; n++)
  {
    for(i=0; i<state.handCount[run.player]; i++)
    {
      if(run.treasures[run.player][n] == state.hand[run.player][i])
      {
          match[n]++;
      }
    }
  }

  if(run.treasureCount[run.player] == 1 && match[0] >= 1)
  {
    treasureTypeNotIncorrect = 1;  
  }
  if(run.treasureCount[run.player] == 2 && match[0] + match[1] >= 2)
  {
    treasureTypeNotIncorrect = 1;  
  }

  //split above, count 1st cards, count 2nd cards, then pass it.

  int success = 1;

  //did we add 2 treasure card and discard 2 cards from the deck in the process
  success &= AssertCondition("Treasure count in hand incorrect", correctTreasureCount);

  //are the treasure cards of correct type
  success &= AssertCondition("Treasure type incorrect", treasureTypeNotIncorrect);

  //are the added treasures really coppers
  success &= AssertCondition("Discard count incorrect", correctDiscardCount);

  //were 2 cards discarded
  success &= AssertCondition("Adventurer card NOT in played pile", state.playedCards[0] = adventurer); 

  char params[100];
  sprintf(params, "TEST: Current Player: %d | Hand: %d | Deck: %d | Discards: %d | Treasures: %d | Revealed: %d",
  run.player, before.handCount[run.player], before.deckCount[run.player], before.discardCount[run.player],
  run.treasureCount[run.player], run.revealCount[run.player]);

  AssertTest(params, success);
  return 0;
}


int main (int argc, char** argv) {

  srand(time(NULL));

  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};

  printf("\n**************  randomtest1: Adventurer ***********\n");

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
