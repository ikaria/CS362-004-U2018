/* -----------------------------------------------------------------------
 * randomtestcard2.c
 *
 * Written by: Joe Beauchesne
 * ONID: beauchjo
 *
 * Last updated: 8/1/18
 *
 * This code heavily leverages the code provided in the classroom materials
 * that is said to be OK to use as a template.
 *
 * This code randomly tests the newVillage function, it can do this directly or through the use
 * of the cardEffect function. As long as the function call is correct, either should work and
 * both are tested.
 *
 * void newVillage(struct gameState *state, int currentPlayer);
 *
 * This function handles the playing of the newVillage function, which is known to have a bug. The function adds one card
 * to the player's hand (from their deck) and two actions to their total.
 *
 * This is a random generator and randomly sets up game states to run the tests.
 *
 *
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

// set NOISY_TEST to 0 to remove printfs from output

#define NOISY_TEST 0

/* Test Assert function. Takes two integers, compares them, and increments the counter
 * to track the number of comparisons that are actually performed during the testing.
 *
 *  */


int testAssert(int a, int b, int* c) {
    (*c)++;
//    printf("%i  %i\n", a, b);
    if (a==b) {
#if (NOISY_TEST == 1)
        printf("Test Passed\n");
#endif
        return 0;
    }
    else {
#if (NOISY_TEST == 1)
        printf("Test Failed\n");
#endif
        return -1;
    }
}


int main() {
    struct gameState G;
    struct gameState Gcopy;
//    int i,j;
//    int seed = 5;
    int totalTestResult = 0;
    int testResult = 0;
    int testCaseResult = 0;
    int totalTestCount = 0;
    int i,p,j,n;
    int card;
    int maxNumPlayers=4;
    int cardPosition;
    int maxCards = 27;
    int numCards;
    int passingTestCases = 0;
    int failingTestCases = 0;
// // //
//

    SelectStream(2);
    PutSeed(3);

// Total number of test runs
    for (n = 0; n < 10000; n++) {
        testResult = 0;
        testCaseResult = 0;

// Randomly fill the game with ascii characters
        for (i = 0; i < sizeof(struct gameState); i++) {
            ((char*)&G)[i] = floor(Random() * 256);
        }
// Randomly deal a random number of cards to all players
        for (j=0; j<maxNumPlayers; j++) {

            numCards = floor(Random() * MAX_DECK);
//            printf("numCards = %i\n", numCards);
            G.deckCount[j] = numCards;
//            printf("Deck count = %i\n", G.deckCount[j] );
            for (int l=0; l<G.deckCount[j]; l++) {
                card = floor(Random() * maxCards);
                G.deck[j][l]=card;
//            printf("%i - %i\n", j, card);
            }
            G.discardCount[j] = floor(Random() * MAX_DECK);
//            printf("Discard count = %i\n", G.discardCount[j] );
            for (int l=0; j<G.discardCount[l]; l++) {
//                printf("%i - %i\n", j, card);
                card = floor(Random() * maxCards);
                G.discard[j][l]=card;
//            printf("%i - %i - %i\n", j, l,  card);
            }
            G.handCount[j] = floor(Random() * MAX_HAND);
//            printf("Hand count = %i\n", G.handCount[j] );
            for (int l=0; l<G.handCount[l]; l++) {
                card = floor(Random() * maxCards);
                G.hand[j][l]=card;
//            printf("%i - %i\n", j, card);
            }
            G.playedCardCount = floor(Random() * MAX_HAND);
//            printf("Played card count = %i\n", G.playedCardCount );
            for (int l=0; l<G.playedCardCount; l++) {
                card = floor(Random() * maxCards);
                G.playedCards[l]=card;
//            printf("%i - %i\n", j, card);
            }

        }

// Choose a player randomly

        p = floor(Random() * 4); // Should return 0, 1, 2, 3



// Set the number of actions

        G.numActions = floor(Random() * MAX_DECK);

// Set the number of buys

        G.numBuys = floor(Random() * MAX_DECK);

// Set the number of coins

        G.coins = floor(Random() * MAX_DECK);

// Put the village card at a random position in the hand

        cardPosition = floor(Random() * G.handCount[p]);
        G.hand[p][cardPosition]=village;  // Maybe not necessary


//printf("before deckCount = %d\n", G.deckCount[p]);
//printf("before handCount = %d\n", G.handCount[p]);
//printf("before discardCount = %d\n", G.discardCount[p]);
//printf("before buys = %d\n", G.numBuys);
//printf("before actions = %d\n", G.numActions);
//printf("before coins = %d\n", G.coins);
//printf("card positions = %d  and card  %d\n", cardPosition, G.hand[p][cardPosition] );

// Save the gamestate for later comparison
        memcpy(&Gcopy, &G, sizeof(struct gameState));

        newVillage(&G, p, cardPosition);

//printf("after deckCount = %d\n", G.deckCount[p]);
//printf("after handCount = %d\n", G.handCount[p]);
//printf("after discardCount = %d\n", G.discardCount[p]);
//printf("after buys = %d\n", G.numBuys);
//printf("after actions = %d\n", G.numActions);
//printf("after coins = %d\n", G.coins);

        testResult += testAssert(G.coins, Gcopy.coins, &totalTestCount);
        if (testResult <= -1) {
            printf("coins test failed\n");
//            printf("after %d   before  %d \n", G.coins, Gcopy.coins);
            testResult = 0;
            testCaseResult = -1;
        }

// Hand Count should be unchanged (one card added, one gets played
        testResult += testAssert(G.handCount[p], Gcopy.handCount[p], &totalTestCount);
        if (testResult <= -1) {
            printf("hand count test failed\n");
//            printf("after %d   before  %d \n", G.handCount[p], Gcopy.handCount[p]);
            testResult = 0;
            testCaseResult = -1;
        }
// Deck count should be decreased by one
        testResult += testAssert(G.deckCount[p], Gcopy.deckCount[p]-1, &totalTestCount);
        if (testResult <= -1) {
            printf("deck count test failed\n");
//            printf("after %d   before  %d \n", G.deckCount[p], Gcopy.deckCount[p]);
            testResult = 0;
            testCaseResult = -1;
        }
// numBuys should be unchanged
        testResult += testAssert(G.numBuys, Gcopy.numBuys, &totalTestCount);
        if (testResult <= -1) {
            printf("numBuys check failed\n");
//            printf("after %d   before  %d \n", G.numBuys, Gcopy.numBuys);
            testResult = 0;
            testCaseResult = -1;
        }
        // Player should have two additional actions
        testResult += testAssert(G.numActions, Gcopy.numActions+2, &totalTestCount);
        if (testResult <= -1) {
            printf("numActions check failed\n");
//            printf("after %d   before  %d \n", G.numActions, Gcopy.numActions);
            testResult = 0;
            testCaseResult = -1;
        }


        if (testCaseResult >= 0) {
//            printf("Case Result - All tests passed!\n");
//            printf("Total checks made = %d\n", totalTestCount);
            passingTestCases++;
        } else {
            printf("Case Result - There was at least one test failure during this test case \n");
//            printf("Total checks made = %d\n", totalTestCount);
            totalTestResult = -1;
            failingTestCases++;

        }
        

    }

    if (totalTestResult >= 0) {
        printf("Final Results - All tests passed!\n");
        printf("Total cases = %d\n", passingTestCases + failingTestCases );
        printf("Total passing cases = %d\n", passingTestCases);
        printf("Total failing cases = %d\n", failingTestCases);
    } else {
        printf("Final Results - There was at least one test failure during at least one case\n");
        printf("Total cases = %d\n", passingTestCases + failingTestCases );
        printf("Total passing cases = %d\n", passingTestCases);
        printf("Total failing cases = %d\n", failingTestCases);

    }



    return 0;
}
