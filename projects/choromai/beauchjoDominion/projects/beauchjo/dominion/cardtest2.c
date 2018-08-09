/* -----------------------------------------------------------------------
 * cardtest2.c
 *
 * Written by: Joe Beauchesne
 * ONID: beauchjo
 *
 * Last updated: 7/16/18
 *
 * This code heavily leverages the code provided in the classroom materials
 * that is said to be OK to use as a template.
 *
 * This code tests the newSmithy function, it can do this directly or through the use
 * of the cardEffect function. As long as the function call is correct, either should work and
 * both have been tested. The direction function calls are actually used in the final code.
 *
 * void newSmithy(struct gameState *state, int currentPlayer, int handPos);
 *
 * This function handles the playing of the newSmithy function, which is known to have a bug. The function
 * draws cards from the players hand.
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

        printf ("TESTING newSmithy() - nominal test case:\n");

        memset(&G, 23, sizeof(struct gameState));   // clear the game state
        memset(&Gcopy, 23, sizeof(struct gameState));   // clear the game state

        r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
        if (r == -1) {
            totalTestResult = -1;
            printf("Game state initialization failure\n");
        }
// Set all deck cards to estate cards
        for (j = 0; j < maxHandCount; j++) {
            G.deck[player][j] = estate;
        }
        G.deckCount[player]=maxHandCount;
        G.discardCount[player]=0;




        memcpy(&Gcopy, &G, sizeof(struct gameState));

#if (NOISY_TEST == 1)
    printf("deck card zero = %d\n", G.deck[player][0]);
    printf("hand card zero = %d\n", G.deck[player][0]);
    printf("deckCount = %d\n", G.deckCount[player]);
    printf("handCount = %d\n", G.handCount[player]);
#endif

        newSmithy(&G, player, handPos);


#if (NOISY_TEST == 1)
    printf("deck card zero = %d\n", G.deck[player][0]);
    printf("hand card zero = %d\n", G.deck[player][0]);
    printf("deckCount = %d\n", G.deckCount[player]);
    printf("handCount = %d\n", G.handCount[player]);
#endif
        testResult =0;
    // Hand count should be increased by 3 - but one card is played, so its 2

        testResult += testAssert(G.handCount[player], Gcopy.handCount[player]+2, &totalTestCount);
            if (testResult <= -1) {
                printf("hand count test failed\n");
                testResult = 0;
                testCaseResult = -1;
    }

#if (NOISY_TEST == 1)
    printf("newdeckCount = %d\n", G.deckCount[player]);
    printf("newhandCount = %d\n", G.handCount[player]);
    printf("newplayedCount = %d\n", G.playedCardCount);
    printf("olddeckCount = %d\n", Gcopy.deckCount[player]);
    printf("oldhandCount = %d\n", Gcopy.handCount[player]);
    printf("oldplayedCount = %d\n", Gcopy.playedCardCount);
#endif
    // Deck Count should be decreased by 3
        testResult += testAssert(G.deckCount[player], Gcopy.deckCount[player]-3, &totalTestCount);
        if (testResult <= -1) {
            printf("deck count test failed\n");
            testResult = 0;
            testCaseResult = -1;
        }

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
        testResult += testAssert(G.coins, Gcopy.coins, &totalTestCount);
        if (testResult <= -1) {
            printf("coins check failed\n");
            testResult = 0;
            testCaseResult = -1;
        }
        testResult += testAssert(G.numBuys, Gcopy.numBuys, &totalTestCount);
        if (testResult <= -1) {
            printf("numBuys check failed\n");
            testResult = 0;
            testCaseResult = -1;
        }
        testResult += testAssert(G.numActions, Gcopy.numActions, &totalTestCount);
        if (testResult <= -1) {
            printf("numActions check failed\n");
            testResult = 0;
            testCaseResult = -1;
        }
   // None of the other players' hands should be affected, either
        for (l=0; l<numPlayer; l++) {
            if (l !=player) {
                testResult += testAssert(G.deckCount[l], Gcopy.deckCount[l], &totalTestCount);
                if (testResult <= -1) {
                    printf("deckCount check failed for another player\n");
                    testResult = 0;
                    testCaseResult = -1;
                }
                testResult += testAssert(G.handCount[l], Gcopy.handCount[l], &totalTestCount);
                if (testResult <= -1) {
                    printf("deckCount check failed for another player\\n");
                    testResult = 0;
                    testCaseResult = -1;
                }
                testResult += testAssert(G.discardCount[l], Gcopy.discardCount[l], &totalTestCount);
                if (testResult <= -1) {
                    printf("deckCount check failed for another player\\n");
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

    printf ("TESTING newSmithy() - copper test case:\n");

    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    memset(&Gcopy, 23, sizeof(struct gameState));   // clear the game state

    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
    if (r == -1) {
        totalTestResult = -1;
        printf("Game state initialization failure\n");
    }

    for (j = 0; j < maxHandCount; j++) {
        G.deck[player][j] = copper;
    }
    G.deckCount[player]=maxHandCount;
    G.discardCount[player]=0;

    memcpy(&Gcopy, &G, sizeof(struct gameState));

#if (NOISY_TEST == 1)
    printf("deck card zero = %d\n", G.deck[player][0]);
    printf("hand card zero = %d\n", G.deck[player][0]);
    printf("deckCount = %d\n", G.deckCount[player]);
    printf("handCount = %d\n", G.handCount[player]);
#endif

    newSmithy(&G, player, handPos);


#if (NOISY_TEST == 1)
    printf("deck card zero = %d\n", G.deck[player][0]);
    printf("hand card zero = %d\n", G.deck[player][0]);
    printf("deckCount = %d\n", G.deckCount[player]);
    printf("handCount = %d\n", G.handCount[player]);
#endif
    testResult =0;
    // Hand count should be increased by 3 - but one card is played, so its 2

    testResult += testAssert(G.handCount[player], Gcopy.handCount[player]+2, &totalTestCount);
    if (testResult <= -1) {
        printf("hand count test failed\n");
        testResult = 0;
        testCaseResult = -1;
    }

#if (NOISY_TEST == 1)
    printf("newdeckCount = %d\n", G.deckCount[player]);
    printf("newhandCount = %d\n", G.handCount[player]);
    printf("newplayedCount = %d\n", G.playedCardCount);
    printf("olddeckCount = %d\n", Gcopy.deckCount[player]);
    printf("oldhandCount = %d\n", Gcopy.handCount[player]);
    printf("oldplayedCount = %d\n", Gcopy.playedCardCount);
#endif
    // Deck Count should be decreased by 3
    testResult += testAssert(G.deckCount[player], Gcopy.deckCount[player]-3, &totalTestCount);
    if (testResult <= -1) {
        printf("deck count test failed\n");
        testResult = 0;
        testCaseResult = -1;
    }

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
    testResult += testAssert(G.coins, Gcopy.coins, &totalTestCount);
    if (testResult <= -1) {
        printf("coins check failed\n");
        testResult = 0;
        testCaseResult = -1;
    }
    testResult += testAssert(G.numBuys, Gcopy.numBuys, &totalTestCount);
    if (testResult <= -1) {
        printf("numBuys check failed\n");
        testResult = 0;
        testCaseResult = -1;
    }
    testResult += testAssert(G.numActions, Gcopy.numActions, &totalTestCount);
    if (testResult <= -1) {
        printf("numActions check failed\n");
        testResult = 0;
        testCaseResult = -1;
    }
    // None of the other players' hands shoudl be affected, either
    for (l=0; l<numPlayer; l++) {
        if (l !=player) {
            testResult += testAssert(G.deckCount[l], Gcopy.deckCount[l], &totalTestCount);
            if (testResult <= -1) {
                printf("deckCount check failed for another player\n");
                testResult = 0;
                testCaseResult = -1;
            }
            testResult += testAssert(G.handCount[l], Gcopy.handCount[l], &totalTestCount);
            if (testResult <= -1) {
                printf("deckCount check failed for another player\\n");
                testResult = 0;
                testCaseResult = -1;
            }
            testResult += testAssert(G.discardCount[l], Gcopy.discardCount[l], &totalTestCount);
            if (testResult <= -1) {
                printf("deckCount check failed for another player\\n");
                testResult = 0;
                testCaseResult = -1;
            }

        }
    }


    if (testCaseResult >= 0) {
        printf("The copper case passed - player %d\n", player);
    } else {
        printf("The copper case case failed - player %d\n", player);
        totalTestResult = -1;
        testResult =0;
        testCaseResult =0;
    }


    printf ("TESTING newSmithy() - Fewer than three cards test case:\n");

    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    memset(&Gcopy, 23, sizeof(struct gameState));   // clear the game state

    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game


    for (j = 0; j < maxHandCount; j++) {
        G.deck[player][j] = copper;
    }
    G.deckCount[player]=2;
    G.discardCount[player]=0;

    memcpy(&Gcopy, &G, sizeof(struct gameState));

#if (NOISY_TEST == 1)
    printf("deck card zero = %d\n", G.deck[player][0]);
    printf("hand card zero = %d\n", G.deck[player][0]);
    printf("deckCount = %d\n", G.deckCount[player]);
    printf("handCount = %d\n", G.handCount[player]);
#endif
// This to check the call within the cardEffect function
//    newSmithy(&G, player, handPos);
    cardEffect(smithy, 0, 0, 0, &G, handPos, &bonus);


#if (NOISY_TEST == 1)
    printf("deck card zero = %d\n", G.deck[player][0]);
    printf("hand card zero = %d\n", G.deck[player][0]);
    printf("deckCount = %d\n", G.deckCount[player]);
    printf("handCount = %d\n", G.handCount[player]);
#endif
    testResult =0;
    // Hand count should be increased by 3 - but one card is played, and only two are available, so 1

    testResult += testAssert(G.handCount[player], Gcopy.handCount[player]+1, &totalTestCount);
    if (testResult <= -1) {
        printf("hand count test failed\n");
        testResult = 0;
        testCaseResult = -1;
    }

#if (NOISY_TEST == 1)
    printf("newdeckCount = %d\n", G.deckCount[player]);
    printf("newhandCount = %d\n", G.handCount[player]);
    printf("newplayedCount = %d\n", G.playedCardCount);
    printf("olddeckCount = %d\n", Gcopy.deckCount[player]);
    printf("oldhandCount = %d\n", Gcopy.handCount[player]);
    printf("oldplayedCount = %d\n", Gcopy.playedCardCount);
#endif
    // Deck Count should be decreased by 2 as there are only two cards
    testResult += testAssert(G.deckCount[player], Gcopy.deckCount[player]-2, &totalTestCount);
    if (testResult <= -1) {
        printf("deck count test failed\n");
        testResult = 0;
        testCaseResult = -1;
    }

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
    testResult += testAssert(G.coins, Gcopy.coins, &totalTestCount);
    if (testResult <= -1) {
        printf("coins check failed\n");
        testResult = 0;
        testCaseResult = -1;
    }
    testResult += testAssert(G.numBuys, Gcopy.numBuys, &totalTestCount);
    if (testResult <= -1) {
        printf("numBuys check failed\n");
        testResult = 0;
        testCaseResult = -1;
    }
    testResult += testAssert(G.numActions, Gcopy.numActions, &totalTestCount);
    if (testResult <= -1) {
        printf("numActions check failed\n");
        testResult = 0;
        testCaseResult = -1;
    }
    // None of the other players' hands shoudl be affected, either
    for (l=0; l<numPlayer; l++) {
        if (l !=player) {
            testResult += testAssert(G.deckCount[l], Gcopy.deckCount[l], &totalTestCount);
            if (testResult <= -1) {
                printf("deckCount check failed for another player\n");
                testResult = 0;
                testCaseResult = -1;
            }
            testResult += testAssert(G.handCount[l], Gcopy.handCount[l], &totalTestCount);
            if (testResult <= -1) {
                printf("deckCount check failed for another player\\n");
                testResult = 0;
                testCaseResult = -1;
            }
            testResult += testAssert(G.discardCount[l], Gcopy.discardCount[l], &totalTestCount);
            if (testResult <= -1) {
                printf("deckCount check failed for another player\\n");
                testResult = 0;
                testCaseResult = -1;
            }

        }
    }


    if (testCaseResult >= 0) {
        printf("The fewer than three card case passed - player %d\n", player);
    } else {
        printf("The fewer than three card case failed - player %d\n", player);
        totalTestResult = -1;
        testResult =0;
        testCaseResult =0;
    }


    if (totalTestResult >= 0) {
        printf("FINAL RESULT - All newSmithy tests passed!\n");
        printf("Total checks made = %d\n", totalTestCount);
    } else {
        printf("FINAL RESULT - There was at least one test failure during the newSmithy tests\n");
        printf("Total checks made = %d\n", totalTestCount);
    }

    return 0;
}
