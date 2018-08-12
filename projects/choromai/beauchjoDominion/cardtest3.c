#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <string.h>
#include "utils.h"

//TESTING: VILLAGE CARD 

//Village Played, Smithy Gained, Two Actions Gained, Village Discarded
int playAction_SmithyInHandTwoActionsAddedDiscardedVillage(struct gameState *state)
{

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
  int a = 0;
  int *bonus = &a;

  state->deck[player][0] = copper; 
  state->deck[player][1] = copper; 
  state->deck[player][2] = copper; 
  state->deck[player][3] = copper; 
  state->deck[player][4] = smithy; 

  int actionsBefore = state->numActions;
  int discardsBefore = state->playedCardCount;
  int cardsInHandBefore = state->handCount[player];

  cardEffect(card, choice1, choice2, choice3, state, handPos, bonus);

  int totalDiscards = state->playedCardCount - discardsBefore;
  int discardedCard = state->playedCards[state->playedCardCount-1];
  int actionsAdded = state->numActions - actionsBefore;

  //assertions
  int success = 1;

  success &= AssertCondition("Incorrect number of discards", totalDiscards == 1);
  success &= AssertCondition("Discarded card not village", discardedCard == village);
  success &= AssertCondition("Number of actions added not 2", actionsAdded == 2);
  success &= AssertCondition("Card added to hand is not smithy", state->hand[player][3] == smithy);
  success &= AssertCondition("Number of cards in hand incorrect", cardsInHandBefore == state->handCount[player]);

  return success;
}

//Village Played, Nothing Gained (empty deck), Two Actions Gained, Village Discarded
int playAction_4InHandTwoActionsAddedDiscardedVillage(struct gameState *state)
{

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
  int a = 0;
  int *bonus = &a;

  state->deckCount[player] = 0;
  int actionsBefore = state->numActions;
  int discardsBefore = state->playedCardCount;
  int cardsInHandBefore = state->handCount[player];

  cardEffect(card, choice1, choice2, choice3, state, handPos, bonus);

  int totalDiscards = state->playedCardCount - discardsBefore;
  int discardedCard = state->playedCards[state->playedCardCount-1];
  int actionsAdded = state->numActions - actionsBefore;

  //assertions
  int success = 1;

  success &= AssertCondition("Incorrect number of discards", totalDiscards == 1);
  success &= AssertCondition("Discarded card not village", discardedCard == village);
  success &= AssertCondition("Number of actions added not 2", actionsAdded == 2);
  success &= AssertCondition("No card should have been added.", state->hand[player][3] == adventurer);
  success &= AssertCondition("Number of cards in hand incorrect", cardsInHandBefore == 1 + state->handCount[player]);

  return success;
}


int main (int argc, char** argv) {
  struct gameState G;
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};

  printf("\n**************  cardtest3: Village ***********\n");

  int success = 1;

  initializeGame(2, k, 2, &G);
  success &= AssertTest("Test 1: Play Action, Normal Deck | Expection: 2 actions added, Smithy in hand, village discarded, # of cards in hand unchanged",
    playAction_SmithyInHandTwoActionsAddedDiscardedVillage(&G));

  initializeGame(2, k, 2, &G);
  success &= AssertTest("Test 2: Play Action, DeckEmpty | Expection: 2 actions added, 4 cards in hand, village discarded",
    playAction_4InHandTwoActionsAddedDiscardedVillage(&G));

  if(success)
  {
    printf ("ALL TESTS PASSED\n");
  }
  printf ("\n");

  return 0;
}
