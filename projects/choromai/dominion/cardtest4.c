#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <string.h>
#include "race.h"

//TESTING: VILLAGE CARD 

//verify 4 cards added to currentPlayer's hand
//verify verify 1 buy added 
//verify other players have 1 additinal card each
int testAction(struct gameState *state)
{
  int pass = 0;
  struct gameState stateBefore;

  int player = 0;
  state->handCount[player] = 5;

  //setup state
  int j;

  for(j=0; j<state->numPlayers; j++)
  {

    state->hand[player][0] = smithy;
    state->hand[player][1] = village;
    state->hand[player][2] = village;
    state->hand[player][3] = village;
    state->hand[player][4] = council_room;

    state->deck[player][0] = copper; 
    state->deck[player][1] = copper; 
    state->deck[player][2] = copper; 
    state->deck[player][3] = copper; 
    state->deck[player][4] = copper; 

  }

    int card = council_room;
    int choice1 = 0; 
    int choice2 = 0;
    int choice3 = 0;
    int handPos = 4;
    int *bonus;

  memcpy(&stateBefore, state, sizeof(struct gameState));

  cardEffect(card, choice1, choice2, choice3, state, handPos, bonus);

  int fourCardsInHand = 0;
  int buyAdded = 0;
  int oneCardforOtherPlayers = 0;
  int cardDiscarded = 0;

  //4 cards added, exact cards verified
  int i;
  if(stateBefore.handCount[player] + 3 == state->handCount[player])
  {
      int copperMatches = 0;
      int j;

      for(j = 0; j < 4; j++)
      {
        int pos = state->handCount[player] - 1 - j;
        if(state->hand[player][pos] == copper)
        {
            copperMatches++;
        }
        if(copperMatches == 4)
        {
            fourCardsInHand = 1;
        }
      }
  }

  //opponents given the right card
  for(i=0; i < state->numPlayers; i++)
  {
      int opponentDraws = 0;
      if(i != player)
      {
          if(stateBefore.handCount[i] + 1 == state->handCount[i])
          {
              if(state->hand[i][state->handCount[i]-1] == copper)
              {
                  opponentDraws++;
              }
          }
      }
      if(opponentDraws == state->numPlayers - 1)
      {
          oneCardforOtherPlayers = 1;
      }
  }

  //Buys augmented correctly
  if(stateBefore.numBuys + 1 == state->numBuys)
  {
      buyAdded = 1;
  }

  //council room discarded after play
  if(state->playedCards[state->playedCardCount-1] == council_room)
  {
      cardDiscarded = 1;
  }


  //assertions
  if(fourCardsInHand && buyAdded && cardDiscarded && oneCardforOtherPlayers)
  {
    pass = 1;
  }

  printf ("Test 1: Play Action, Normal Deck | Expection: Four Matching Cards in Hand, Buy Added, 1 card for each opponent  | Result: ");
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

  printf("\n**************  cardtest3: Council Room ***********\n");

  int success = 0;

  initializeGame(2, k, 2, &G);
  success += testAction(&G);

  if(success == 2)
  {
    printf ("ALL TESTS PASSED\n");
  }
  printf ("\n");

  return 0;
}
