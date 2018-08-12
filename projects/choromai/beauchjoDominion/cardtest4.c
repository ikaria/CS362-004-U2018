#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <string.h>
#include "utils.h"

//TESTING: COUNCIL ROOM 

//verify 4 cards added to currentPlayer's hand
//verify verify 1 buy added 
//verify other players have 1 additional card each
int playAction_4cardsAdded_opponentsGot1_buyAdded_fullDecks(struct gameState *state)
{
  //setup state
  struct gameState stateBefore;

  int player = 0;
  state->handCount[player] = 5;

  int j;

  //setup all players
  for(j=0; j<state->numPlayers; j++)
  {

    state->hand[j][0] = smithy;
    state->hand[j][1] = village;
    state->hand[j][2] = village;
    state->hand[j][3] = village;
    state->hand[j][4] = council_room;

    state->deck[j][0] = copper; 
    state->deck[j][1] = copper; 
    state->deck[j][2] = copper; 
    state->deck[j][3] = copper; 
    state->deck[j][4] = copper; 

  }

  int card = council_room;
  int choice1 = 0; 
  int choice2 = 0;
  int choice3 = 0;
  int handPos = 4;
  int a = 0;
  int *bonus = &a;

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
          //each players hand was increased by 1
          if(stateBefore.handCount[i] + 1 == state->handCount[i])
          {
              //the newly added card is indeed a copper
              if(state->hand[i][state->handCount[i]-1] == copper)
              {
                  opponentDraws++;
              }
          }
      }
      //as many coppers in last card as there are players
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

  int success = 1;

  //assertions
  //Should have 4 coppers in hand
  success &= AssertCondition("Four coppers not in hand.", fourCardsInHand);
  //should have 1 buy
  success &= AssertCondition("Incorrect # of Buys added.", buyAdded);
  //should have discarded council room
  success &= AssertCondition("Council room not discarded after play.", cardDiscarded);
  //other players received a copper
  success &= AssertCondition("Other players did not receive 1 copper card.", oneCardforOtherPlayers);

  return success;
}

//Test action with decks containing only 2 of 4 requested cards
int playAction_2cardsAdded_opponentsGot1_buyAdded_Only2CardDecks(struct gameState *state)
{
  //setup state
  struct gameState stateBefore;

  int player = 0;
  state->handCount[player] = 5;

  int j;

  //setup all players
  for(j=0; j<state->numPlayers; j++)
  {

    state->hand[j][0] = smithy;
    state->hand[j][1] = village;
    state->hand[j][2] = village;
    state->hand[j][3] = village;
    state->hand[j][4] = council_room;

    state->deckCount[j] = 2;
    state->deck[j][0] = copper; 
    state->deck[j][1] = copper; 

  }

  int card = council_room;
  int choice1 = 0; 
  int choice2 = 0;
  int choice3 = 0;
  int handPos = 4;
  int a = 0;
  int *bonus = &a;

  memcpy(&stateBefore, state, sizeof(struct gameState));

  cardEffect(card, choice1, choice2, choice3, state, handPos, bonus);

  int twoCardsInHand = 0;
  int buyAdded = 0;
  int oneCardforOtherPlayers = 0;
  int cardDiscarded = 0;

  //2 cards added, exact cards verified
  int i;
  if(stateBefore.handCount[player] + 1 == state->handCount[player])
  {
      int copperMatches = 0;
      int j;

      for(j = 0; j < 2; j++)
      {
        int pos = state->handCount[player] - 1 - j;
        if(state->hand[player][pos] == copper)
        {
            copperMatches++;
        }
        if(copperMatches == 2)
        {
            twoCardsInHand = 1;
        }
      }
  }

  int opponentDraws = 0;
  //opponents given the right card
  for(i=0; i < state->numPlayers; i++)
  {
      if(i != player)
      {
          //each players hand was increased by 1
          if(stateBefore.handCount[i] + 1 == state->handCount[i])
          {
              //the newly added card is indeed a copper
              if(state->hand[i][state->handCount[i]-1] == copper)
              {
                  opponentDraws++;
              }
          }
      }
  }

  //as many coppers in last card as there are players
  if(opponentDraws == state->numPlayers - 1)
  {
      oneCardforOtherPlayers = 1;
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

  int success = 1;

  //assertions
  //Should have 4 coppers in hand
  success &= AssertCondition("2 coppers not in hand.", twoCardsInHand);
  //should have 1 buy
  success &= AssertCondition("Incorrect # of Buys added.", buyAdded);
  //should have discarded council room
  success &= AssertCondition("Council room not discarded after play.", cardDiscarded);
  //other players received a copper
  success &= AssertCondition("Other players did not receive 1 copper card.", oneCardforOtherPlayers);

  return success;
}

//function has race condition, expected: none 
void raceCondition_NoneDetected()
{
  struct gameState G;
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};
  initializeGame(2, k, 2, &G);
  
  //setup state
  int player = 0;
  G.handCount[player] = 5;

  int j;

  //setup all players
  for(j=0; j<G.numPlayers; j++)
  {

    G.hand[j][0] = smithy;
    G.hand[j][1] = village;
    G.hand[j][2] = village;
    G.hand[j][3] = village;
    G.hand[j][4] = council_room;

    G.deck[j][0] = copper; 
    G.deck[j][1] = copper; 
    G.deck[j][2] = copper; 
    G.deck[j][3] = copper; 
    G.deck[j][4] = copper; 

  }

  int card = council_room;
  int choice1 = 0; 
  int choice2 = 0;
  int choice3 = 0;
  int handPos = 4;
  int a = 0;
  int *bonus = &a;

  cardEffect(card, choice1, choice2, choice3, &G, handPos, bonus);

}

int main (int argc, char** argv) {
  struct gameState G;
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};

  printf("\n**************  cardtest4: Council Room ***********\n");

  int success = 1;

  initializeGame(2, k, 2, &G);
  success &= AssertTest ("Test 0: Race Condition | Expected: None Detected ",
    TestRace(&raceCondition_NoneDetected));

  initializeGame(2, k, 2, &G);
  success &= AssertTest("Test 1: Play Action, Normal Deck | Expected: 4 Coppers in Hand, Buy Added, 1 copper for each opponent, council_room discarded",
    playAction_4cardsAdded_opponentsGot1_buyAdded_fullDecks(&G));

  initializeGame(2, k, 2, &G);
  success &= AssertTest("Test 2: Play Action, 2 Cards in Deck | Expected: 2 Coppers in Hand, Buy Added, 1 copper for each opponent, council_room discarded",
    playAction_2cardsAdded_opponentsGot1_buyAdded_Only2CardDecks(&G));

  if(success)
  {
    printf ("ALL TESTS PASSED\n");
  }
  printf ("\n");

  return 0;
}
