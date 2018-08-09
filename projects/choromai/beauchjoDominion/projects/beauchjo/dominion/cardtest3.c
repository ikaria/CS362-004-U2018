/* -----------------------------------------------------------------------
 * cardtest3.c
 *
 * Written by: Joe Beauchesne
 * ONID: beauchjo
 *
 * Last updated: 7/16/18
 *
 * This code heavily leverages the code provided in the classroom materials
 * that is said to be OK to use as a template.
 *
 * This code tests the newCouncilRoom function, it can do this directly or through the use
 * of the cardEffect function. As long as the function call is correct, either should work and
 * both have been tested. The direction function calls are actually used in the final code.
 *
 * void newCouncilRoom(struct gameState *state, int currentPlayer, int handPos);
 *
 * This function handles the playing of the newCouncil function, which is known to have a bug. The function
 * draws cards from the players deck, adds buys, and has each of the other players draw a card.
 *
 * Since this function affects other players, it is tested for all players.
 *
 * Test cases - nominal - player has a max count of estate cards and the proper number are drawn (passes)
 *              copper - player has a max count of copper cards and the proper number are drawn (fails)
 *              short - player does not have enough cards (passes)
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

/* This needs to move into a different location, but I'm struggling to figure out where
 *  For now I'll leave it here
 *
 * This function compares two integers and if they are equal, returns 0. If they are not equal, it
 * returns -1. This can be used at the next level to see if any tests fails by use of a test failure flag
 * after each execution. If any of the tests (which are all expected to pass) result in a failure, then the
 * flag has changed and so the total test fails as a whole.
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
    int j,l,r;
    int seed = 5;
    int numPlayer = 4;
    int maxHandCount = 500;
    int k[10] = {adventurer, council_room, feast, gardens, mine
            , remodel, smithy, village, baron, great_hall};
    int player=0;
    int bonus;  // Needed for cardEffect execution
    int handPos = 0;
// // //
//

// Run for each of the players in a four player game

//        for (player = 0; player < numPlayer; player++) {

// Test 1 - all cards in the deck are turned to estate cards

        printf ("TESTING newCouncilRoom() - nominal test case / estate cards:\n");

        memset(&G, 23, sizeof(struct gameState));   // clear the game state
        memset(&Gcopy, 23, sizeof(struct gameState));   // clear the game state

        r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
        if (r == -1) {
            totalTestResult = -1;
            printf("State initialization failed\n");
        }

        for (j = 0; j < maxHandCount; j++) {
            G.deck[player][j] = estate;
        }
        G.deckCount[player]=maxHandCount;
        G.discardCount[player]=0;


    for (player=0; player < numPlayer; player ++) {
        memcpy(&Gcopy, &G, sizeof(struct gameState));


#if (NOISY_TEST == 1)
        printf("----------------------------------\n");
        printf("Before newCouncilRoom Execution\n");
        printf("----------------------------------\n");
        printf("deckCount = %d\n", G.deckCount[player]);
        printf("handCount = %d\n", G.handCount[player]);
        printf("playedCount = %d\n", G.playedCardCount);
        printf("coins = %d\n", G.coins);
        printf("buys = %d\n", G.numBuys);
        printf("actions = %d\n", G.numActions);
        printf("----------------------------------\n");
#endif

        if (player != 0) {
            newCouncilRoom(&G, player, handPos);
        } else {
            cardEffect(council_room, 0, 0, 0, &G, handPos, &bonus);
        }


#if (NOISY_TEST == 1)
        printf("----------------------------------\n");
        printf("After newCouncilRoom Execution\n");
        printf("----------------------------------\n");
        printf("deckCount = %d\n", G.deckCount[player]);
        printf("handCount = %d\n", G.handCount[player]);
        printf("playedCount = %d\n", G.playedCardCount);
        printf("coins = %d\n", G.coins);
        printf("buys = %d\n", G.numBuys);
        printf("actions = %d\n", G.numActions);
        printf("----------------------------------\n");

#endif
        testResult =0;
        // Hand count should be increased by 4 - but one card is played, so its 3

        testResult += testAssert(G.handCount[player], Gcopy.handCount[player]+3, &totalTestCount);
        if (testResult <= -1) {
            printf("hand count test failed\n");
            testResult = 0;
            testCaseResult = -1;
        }

#if (NOISY_TEST == 1)
        printf("newdeckCount = %d\n", G.deckCount[player]);
        printf("newhandCount = %d\n", G.handCount[player]);
        printf("newplayedCount = %d\n", G.playedCardCount);
        printf("newbuys = %d\n", G.numBuys);
        printf("newActions = %d\n", G.numActions);
        printf("olddeckCount = %d\n", Gcopy.deckCount[player]);
        printf("oldhandCount = %d\n", Gcopy.handCount[player]);
        printf("oldplayedCount = %d\n", Gcopy.playedCardCount);
        printf("newActions = %d\n", Gcopy.numActions);

#endif
        // Deck Count should be decreased by 4
        testResult += testAssert(G.deckCount[player], Gcopy.deckCount[player]-4, &totalTestCount);
        if (testResult <= -1) {
            printf("deck count test failed\n");
            testResult = 0;
            testCaseResult = -1;
        }
        // One card is played, so the played count should be 1
        testResult += testAssert(G.playedCardCount, Gcopy.playedCardCount+1, &totalTestCount);
        if (testResult <= -1) {
            printf("played count test failed\n");
            testResult = 0;
            testCaseResult = -1;
        }

        // Supply Count should be unchanged
        testResult += testAssert(G.supplyCount[estate], Gcopy.supplyCount[estate], &totalTestCount);
        if (testResult <= -1) {
            printf("supply card count checks failed\n");
            testResult = 0;
            testCaseResult = -1;
        }
        // Number of coins should be unchanged
        testResult += testAssert(G.coins, Gcopy.coins, &totalTestCount);
        if (testResult <= -1) {
            printf("coins check failed\n");
            testResult = 0;
            testCaseResult = -1;
        }
        // Player should have one additional buy
        testResult += testAssert(G.numBuys, Gcopy.numBuys+1, &totalTestCount);
        if (testResult <= -1) {
            printf("numBuys check failed\n");
            testResult = 0;
            testCaseResult = -1;
        }
        // Player should have no additional actions
        testResult += testAssert(G.numActions, Gcopy.numActions, &totalTestCount);
        if (testResult <= -1) {
            printf("numActions check failed\n");
            testResult = 0;
            testCaseResult = -1;
        }
        // The other players should now have one additional card in their hand drawn from their deck
        for (l=0; l<numPlayer; l++) {
            if (l !=player) {

#if (NOISY_TEST == 1)
                printf("-------------------------------\n");
                printf("player %d check\n", l);
                printf("-------------------------------\n");
                printf("newdeckCount = %d\n", G.deckCount[l]);
                printf("newhandCount = %d\n", G.handCount[l]);
                printf("olddeckCount = %d\n", Gcopy.deckCount[l]);
                printf("oldhandCount = %d\n", Gcopy.handCount[l]);
                printf("-------------------------------\n");
#endif

                testResult += testAssert(G.deckCount[l], Gcopy.deckCount[l]-1, &totalTestCount);
                if (testResult <= -1) {
                    printf("deckCount check failed for player %d\n", l);
                    testResult = 0;
                    testCaseResult = -1;
                } else {
                    printf("deckCount check passed for player %d\n", l);
                }
                testResult += testAssert(G.handCount[l], Gcopy.handCount[l]+1, &totalTestCount);
                if (testResult <= -1) {
                    printf("handCount check failed for player %d\n", l);
                    testResult = 0;
                    testCaseResult = -1;
                }
                else {
                    printf("deckCount check passed for player %d\n", l);
                }
                testResult += testAssert(G.discardCount[l], Gcopy.discardCount[l], &totalTestCount);
                if (testResult <= -1) {
                    printf("discardCount check failed for player %d\n", l);
                    testResult = 0;
                    testCaseResult = -1;
                }

            }
        }


        if (testCaseResult >= 0) {
            printf("The estate case passed - player %d\n", player);
        } else {
            printf("The estate case case failed - player %d\n", player);
            totalTestResult = -1;
            testResult =0;
            testCaseResult =0;
        }
    }




    if (totalTestResult >= 0) {
        printf("FINAL RESULT - All tests passed!\n");
        printf("Total checks made = %d\n", totalTestCount);
    } else {
        printf("FINAL RESULT - There was at least one test failure during a test\n");
        printf("Total checks made = %d\n", totalTestCount);
    }

    return 0;
}
