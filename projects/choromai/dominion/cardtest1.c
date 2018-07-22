#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include "utils.h"

//TESTING: SMITHY CARD 

//Deck has at least 3 cards, three coppers added, 7 cards total 
int atLeastThreeCardsInDeck(struct gameState *state)
{

  int player = 0;
  state->handCount[player] = 5;

  //setup state
  state->hand[player][0] = smithy;
  state->hand[player][1] = village;
  state->hand[player][2] = village;
  state->hand[player][3] = village;
  state->hand[player][4] = gold;

  int card = smithy;
  int choice1 = 0; 
  int choice2 = 0;
  int choice3 = 0;
  int handPos = 0;
  int a = 0;
  int *bonus = &a;

  //add 3 coppers to deck
  state->supplyCount[copper] = 3;
  gainCard(copper,state,1,player);
  gainCard(copper,state,1,player);
  gainCard(copper,state,1,player);

  //run test
  cardEffect(card, choice1, choice2, choice3, state, handPos, bonus);

  //eval results
  int success = 1;

  //cards 1,6,7 should now be coppers
  success &= AssertCondition("Cards added are not 3 coppers.",
    state->hand[player][0] == copper &&
    state->hand[player][5] == copper && 
    state->hand[player][6] == copper);

  //last played card should be smithy
  success &= AssertCondition("Last played card not Smithy.",
    state->playedCards[state->playedCardCount-1] == smithy);

  //we should now have 7 cards
  success &= AssertCondition("Don't have 7 cards.",
    state->handCount[player] == 7);    

  return success;
}

//Deck has less than 3 cards, three coppers added, 7 cards total 
int lessThanThreeCardsInDeck(struct gameState *state)
{

  int player = 0;
  state->handCount[player] = 5;

  //setup state
  state->hand[player][0] = smithy;
  state->hand[player][1] = village;
  state->hand[player][2] = village;
  state->hand[player][3] = village;
  state->hand[player][4] = gold;

  int card = smithy;
  int choice1 = 0; 
  int choice2 = 0;
  int choice3 = 0;
  int handPos = 0;
  int *bonus = NULL;

  //add 3 coppers to deck
  state->supplyCount[copper] = 3;
  gainCard(copper,state,1,player);
  gainCard(copper,state,1,player);
  gainCard(copper,state,0,player);

  cardEffect(card, choice1, choice2, choice3, state, handPos, bonus);

  int success = 1;

  //cards 1,6,7 should now be coppers
  success &= AssertCondition("Cards added are not 3 coppers.",
    state->hand[player][0] == copper &&
    state->hand[player][5] == copper && 
    state->hand[player][6] == copper);
  
  //last played card should be smithy
  success &= AssertCondition("Last played card not Smithy.",
    state->playedCards[state->playedCardCount-1] == smithy);
    
  //we should now have 7 cards
  success &= AssertCondition("Don't have 7 cards",
    state->handCount[player] == 7);

  return success;
}

//function has race condition, expected: none 
void raceCondition_NoneDetected()
{

  struct gameState G;
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};
  initializeGame(2, k, 2, &G);

  int player = 1; 
  G.handCount[player] = 5;

  //setup state
  G.hand[player][0] = smithy;
  G.hand[player][1] = village;
  G.hand[player][2] = village;
  G.hand[player][3] = village;
  G.hand[player][4] = gold;

  int card = smithy;
  int choice1 = 0; 
  int choice2 = 0;
  int choice3 = 0;
  int handPos = 0;
  int *bonus = NULL;

  //add 3 coppers to deck
  gainCard(copper,&G,1,player);
  gainCard(copper,&G,1,player);
  gainCard(copper,&G,1,player);

  cardEffect(card, choice1, choice2, choice3, &G, handPos, bonus);
}

//Deck has less than 3 cards, three coppers added, 7 cards total 
int lessThanThreeInDeckNoneInDiscard(struct gameState *state)
{

  int player = 0;
  state->handCount[player] = 5;

  //setup state
  state->hand[player][0] = smithy;
  state->hand[player][1] = village;
  state->hand[player][2] = village;
  state->hand[player][3] = village;
  state->hand[player][4] = gold;

  state->deckCount[player] = 0;

  int card = smithy;
  int choice1 = 0; 
  int choice2 = 0;
  int choice3 = 0;
  int handPos = 0;
  int *bonus = NULL;

  //add 3 coppers to deck
  state->supplyCount[copper] = 2;
  gainCard(copper,state,1,player);
  gainCard(copper,state,1,player);

  cardEffect(card, choice1, choice2, choice3, state, handPos, bonus);

  int success = 1;

  //cards 1,6 should now be coppers
  success &= AssertCondition("Cards added are 2 coppers.",
    state->hand[player][0] == copper &&
    state->hand[player][5] == copper); 
  
  //last played card should be smithy
  success &= AssertCondition("Last played card not Smithy.",
    state->playedCards[state->playedCardCount-1] == smithy);
    
  //we should now have 6 cards
  success &= AssertCondition("Don't have 6 cards",
    state->handCount[player] == 6);

  return success;
}

int main (int argc, char** argv) {
  struct gameState G;
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};

  printf("\n**************  cardtest1: Smithy ***********\n");

  int success = 1;

  success &= AssertTest ("Test 0: Race Condition | Expected: None Detected ",
    TestRace(&raceCondition_NoneDetected));

  if(success == 0)
  {
    printf("RACE CONDITION DETECTED. Remaining sub-tests aborted.\n");
    return 0;
  }

  initializeGame(2, k, 2, &G);
  success &= AssertTest("Test 1: At least 3 Cards in Deck | Expected: Played = Smithy, 3 Coppers added, total 7 cards",
    atLeastThreeCardsInDeck(&G));

  initializeGame(2, k, 2, &G);
  success &= AssertTest("Test 2: Less than 3 Cards in Deck | Expected: Played = Smithy, 3 Coppers added, total 7 cards",
    lessThanThreeCardsInDeck(&G));
    
  initializeGame(2, k, 2, &G);
  success &= AssertTest("Test 3: Less than 3 Cards in Deck, none in Discard Pile | Expected: Played = Smithy, 3 Coppers added, total 7 cards",
    lessThanThreeInDeckNoneInDiscard(&G));
    
  if(success)
  {
    printf ("ALL TESTS PASSED\n");
  }
  printf ("\n");

  return 0;
}
