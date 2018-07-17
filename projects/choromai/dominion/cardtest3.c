#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <string.h>
#include "race.h"

//TESTING: VILLAGE CARD 

//Village Played, Smithy Gained, Two Actions Gained, Village Discarded
int playAction_SmithyInHandTwoActionsAddedDiscardedVillage(struct gameState *state)
{
  int pass = 0;

  int player = 0;
  state->handCount[player] = 5;

  //setup state
  state->hand[player][0] = smithy;
  state->hand[player][1] = village;
  state->hand[player][2] = village;
  state->hand[player][3] = village;
  state->hand[player][4] = adventurer;

  int card = village;
  int choice1 = 0; 
  int choice2 = 0;
  int choice3 = 0;
  int handPos = 3;
  int *bonus;

  state->deck[player][0] = copper; 
  state->deck[player][1] = copper; 
  state->deck[player][2] = copper; 
  state->deck[player][3] = copper; 
  state->deck[player][4] = smithy; 

  int actionsBefore = state->numActions;
  int discardsBefore = state->playedCardCount;

  cardEffect(card, choice1, choice2, choice3, state, handPos, bonus);

  int i=0;
  int cardsInHand = state->handCount[player];
  int nextCard = 0;

  int totalDiscards = state->playedCardCount - discardsBefore;
  int discardedCard = state->playedCards[state->playedCardCount-1];
  int actionsAdded = state->numActions - actionsBefore;

  //assertions
  if(totalDiscards == 1 && discardedCard == village && actionsAdded == 2)
  {
      if(state->hand[player][3] == smithy)
      {
        pass = 1;
      }
  }

  printf ("Test 1: Play Action, Normal Deck | Expection: 2 actions added, Smithy in hand, village discarded | Result: ");
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

  printf("\n**************  cardtest3: Village ***********\n");

  int success = 0;

  initializeGame(2, k, 2, &G);
  success += playAction_SmithyInHandTwoActionsAddedDiscardedVillage(&G);

  if(success == 2)
  {
    printf ("ALL TESTS PASSED\n");
  }
  printf ("\n");

  return 0;
}
