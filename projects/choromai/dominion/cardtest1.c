#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include "race.h"

//TESTING: SMITHY CARD 

//Deck has at least 3 cards, three coppers added, 7 cards total 
int atLeastThreeCardsInDeck(struct gameState *state)
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
  state->hand[player][4] = gold;

  int card = smithy;
  int choice1 = 0; 
  int choice2 = 0;
  int choice3 = 0;
  int handPos = 0;
  int *bonus;

  //add 3 coppers to deck
  state->supplyCount[copper] = 3;
  gainCard(copper,state,1,player);
  gainCard(copper,state,1,player);
  gainCard(copper,state,1,player);

  cardEffect(card, choice1, choice2, choice3, state, handPos, bonus);

  //cards 1,6,7 should now be coppers
  if(state->hand[player][6] == copper &&
    state->hand[player][5] == copper && 
    state->hand[player][0] == copper)
  {
    //last played card should be smithy
    if(state->playedCards[state->playedCardCount-1] == smithy)
    {
        //make sure count is accurate
        if(state->handCount[player] == 7)    
        {
            pass = 1;
        }
    }
  }

  printf ("Test 1: At least 3 Cards in Deck | Expection: Played = Smithy, 3 Coppers added, total 7 cards | Result: ");
  if(pass)
    printf ("PASSED\n");
  else
    printf ("FAILED\n");

  return pass;
}

//Deck has less than 3 cards, three coppers added, 7 cards total 
int lessThanThreeCardsInDeck(struct gameState *state)
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
  state->hand[player][4] = gold;

  int card = smithy;
  int choice1 = 0; 
  int choice2 = 0;
  int choice3 = 0;
  int handPos = 0;
  int *bonus;

  //add 3 coppers to deck
  state->supplyCount[copper] = 3;
  gainCard(copper,state,1,player);
  gainCard(copper,state,1,player);
  gainCard(copper,state,0,player);

  cardEffect(card, choice1, choice2, choice3, state, handPos, bonus);

  //cards 1,6,7 should now be coppers
  if(state->hand[player][6] == copper &&
    state->hand[player][5] == copper && 
    state->hand[player][0] == copper)
  {
    //last played card should be smithy
    if(state->playedCards[state->playedCardCount-1] == smithy)
    {
        //make sure count is accurate
        if(state->handCount[player] == 7)    
        {
            pass = 1;
        }
    }
  }

  printf ("Test 2: Less than 3 Cards in Deck | Expection: Played = Smithy, 3 Coppers added, total 7 cards | Result: ");
  if(pass)
    printf ("PASSED\n");
  else
    printf ("FAILED\n");

  return pass;
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
  int *bonus;

  //add 3 coppers to deck
  gainCard(copper,&G,1,player);
  gainCard(copper,&G,1,player);
  gainCard(copper,&G,1,player);

  cardEffect(card, choice1, choice2, choice3, &G, handPos, bonus);
}


int main (int argc, char** argv) {
  struct gameState G;
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};

  printf("\n**************  cardtest1: Smithy ***********\n");

  int success = 0;

  success += TestRace(&raceCondition_NoneDetected);

  printf("Test 0: Race Condition | Expectation: None Detected | Result: ");
  if(success == 1)
    printf("PASSED\n");
  else
    printf("FAILED\n");

  if(success == 0)
  {
    printf("REMAINING TESTS ABORTED\n");
    return 0;
  }

  initializeGame(2, k, 2, &G);
  success += atLeastThreeCardsInDeck(&G);

  initializeGame(2, k, 2, &G);
  success += lessThanThreeCardsInDeck(&G);

  if(success == 3)
  {
    printf ("ALL TESTS PASSED\n");
  }
  printf ("\n");

  return 0;
}
