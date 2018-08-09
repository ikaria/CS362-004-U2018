/* -----------------------------------------------------------------------
 * cardtest1.c
 *
 * Written by: Joe Beauchesne
 * ONID: beauchjo
 *
 * Last updated: 7/18/18
 *
 * This code heavily leverages the code provided in the classroom materials
 * that is said to be OK to use as a template.
 *
 * This code tests the newAdventurer function, it can do this directly or through the use
 * of the cardEffect function. As long as the function call is correct, either should work and
 * both are tested.
 *
 * void newAdventurer(struct gameState *state, int currentPlayer);
 *
 * This function handles the playing of the newAdventurer function, which is known to have a bug. The function
 * has the player look through their hand and reveal (up to) three treasure cards that get added to their hand. These
 * are gold, silver, and copper cards.
 *
 * The unit test validates the cases where there are no treasure cards,  all cards are coppers, all are silvers,
 * all are gold, and a case where there is only one treasure card.
 *
 *
 * Overall, I wish that I had refactored this to run in a loop, which reduces the length of the code at the
 * cost of complexity. Even given the two weeks to work the assignment, along with the other commitments, I just
 * don't feel I have time to implement and debug it.
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
    int j,l,r;
    int seed = 5;
    int numPlayer = 4;
    int maxHandCount = 500;
    int k[10] = {adventurer, council_room, feast, gardens, mine
            , remodel, smithy, village, baron, great_hall};
    int player;
    int bonus;
// // //
//

// Run for each of the players in a four player game

        for (player = 0; player < numPlayer; player++) {

// Player has no treasure cards at all - all cards turned to estate

        printf ("TESTING newAdventurer() - no treasure cards test:\n");

        memset(&G, 23, sizeof(struct gameState));   // clear the game state
        memset(&Gcopy, 23, sizeof(struct gameState));   // clear the game state

// initialize a new game
        r = initializeGame(numPlayer, k, seed, &G);
        if (r == -1) {
            totalTestResult = -1;
            printf("State initialization failed\n");
        }

// In order to run properly, need to run end turn to set up the correct player,
// otherwise the program will segfault. There's no real reason to believe this is
// necessary although it certainly confirms it. At a minimum, it helped me to understand
// parts of the code that I certainly wouldn't have understood without it. There's other ways
// around this but I used that elsewhere on other function tests so I'm going to use this one
// here.

            for (int k=0; k<player; k++) {
                endTurn(&G);
//                printf("Ran endTurn\n");
            }

        for (j = 0; j < maxHandCount; j++) {
            G.deck[player][j] = estate;
        }


        memcpy(&Gcopy, &G, sizeof(struct gameState));

#if (NOISY_TEST == 1)
    printf("deckCount = %d\n", G.deckCount[player]);
    printf("handCount = %d\n", G.handCount[player]);
#endif

//        newAdventurer(&G, player);

        cardEffect(adventurer, 0, 0, 0, &G, 0, &bonus);

#if (NOISY_TEST == 1)
    printf("deck card zero = %d\n", G.deck[player][0]);
    printf("hand card zero = %d\n", G.deck[player][0]);
    printf("deckCount = %d\n", G.deckCount[player]);
    printf("handCount = %d\n", G.handCount[player]);
#endif
        testResult =0;
    // Hand count should be unchanged
        testResult += testAssert(G.handCount[player], Gcopy.handCount[player], &totalTestCount);
    // Deck Count should be empty
        testResult += testAssert(G.deckCount[player], 0, &totalTestCount);
        testResult += testAssert(G.supplyCount[estate], Gcopy.supplyCount[estate], &totalTestCount);
            if (testResult <= -1) {
                printf("card count checks failed\n");
                testResult = 0;
                testCaseResult = -1;
            }
        testResult += testAssert(G.coins, Gcopy.coins, &totalTestCount);
        if (testCaseResult <= -1) {
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
                    printf("deckCount check failed\n");
                    testResult = 0;
                    testCaseResult = -1;
                }
                testResult += testAssert(G.handCount[l], Gcopy.handCount[l], &totalTestCount);
                if (testResult <= -1) {
                    printf("handCount check failed\n");
                    testResult = 0;
                    testCaseResult = -1;
                }
                testResult += testAssert(G.discardCount[l], Gcopy.discardCount[l], &totalTestCount);
                if (testResult <= -1) {
                    printf("discardCount check failed\n");
                    testResult = 0;
                    testCaseResult = -1;
                }

            }
        }


        if (testCaseResult >= 0) {
            printf("The No treasure cards case passed - player %d\n", player);
        } else {
            printf("The No treasure cards case failed - player %d\n", player);
            totalTestResult = -1;
            testResult =0;
            testCaseResult =0;
        }

    // All copper card test

        printf ("TESTING newAdventurer() - copper test:\n");

        memset(&G, 23, sizeof(struct gameState));   // clear the game state
        memset(&Gcopy, 23, sizeof(struct gameState));   // clear the game state
        r = initializeGame(numPlayer, k, seed, &G); // initialize a new game

        for (int k=0; k<player; k++) {
            endTurn(&G);
        }
        for (j = 0; j < maxHandCount; j++) {
            G.deck[player][j] = copper;
        }
        memcpy(&Gcopy, &G, sizeof(struct gameState));

#if (NOISY_TEST == 1)
    printf("deck card zero = %d\n", G.deck[player][0]);
    printf("hand card zero = %d\n", G.deck[player][0]);
    printf("deckCount = %d\n", G.deckCount[player]);
    printf("handCount = %d\n", G.handCount[player]);
#endif
        newAdventurer(&G, player);

#if (NOISY_TEST == 1)
    printf("deck card zero = %d\n", G.deck[player][0]);
    printf("hand card zero = %d\n", G.deck[player][0]);
    printf("deckCount = %d\n", G.deckCount[player]);
    printf("handCount = %d\n", G.handCount[player]);
#endif
        testResult += testAssert(G.handCount[player], Gcopy.handCount[player]+2, &totalTestCount);
        testResult += testAssert(G.deckCount[player], Gcopy.deckCount[player]-2, &totalTestCount);
        testResult += testAssert(G.supplyCount[copper], Gcopy.supplyCount[copper], &totalTestCount);
        if (testResult <= -1) {
            printf("card count checks failed\n");
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
// Verify that the other players are unaffected

        for (l=0; l<numPlayer; l++) {
            if (l !=player) {
                testResult += testAssert(G.deckCount[l], Gcopy.deckCount[l], &totalTestCount);
                testResult += testAssert(G.handCount[l], Gcopy.handCount[l], &totalTestCount);
                testResult += testAssert(G.discardCount[l], Gcopy.discardCount[l], &totalTestCount);
            }
        }

        if (testCaseResult >= 0) {
            printf("All copper cards case passed - player %d\n", player);
        } else {
            printf("All copper cards case failed - player %d\n", player);
            totalTestResult = -1;
            testCaseResult =0;
            testResult =0;
        }

    // All silver card test

        printf ("TESTING newAdventurer() - silver test :\n");

        memset(&G, 23, sizeof(struct gameState));   // clear the game state
        memset(&Gcopy, 23, sizeof(struct gameState));   // clear the game state
        r = initializeGame(numPlayer, k, seed, &G); // initialize a new game

        for (int k=0; k<player; k++) {
            endTurn(&G);
        }
        for (j = 0; j < maxHandCount; j++) {
            G.deck[player][j] = silver;
        }
        memcpy(&Gcopy, &G, sizeof(struct gameState));

#if (NOISY_TEST == 1)
    printf("deck card zero = %d\n", G.deck[player][0]);
    printf("hand card zero = %d\n", G.deck[player][0]);
    printf("deckCount = %d\n", G.deckCount[player]);
    printf("handCount = %d\n", G.handCount[player]);
#endif
        newAdventurer(&G, player);

#if (NOISY_TEST == 1)
    printf("deck card zero = %d\n", G.deck[player][0]);
    printf("hand card zero = %d\n", G.deck[player][0]);
    printf("deckCount = %d\n", G.deckCount[player]);
    printf("handCount = %d\n", G.handCount[player]);
#endif
        testResult += testAssert(G.handCount[player], Gcopy.handCount[player]+2, &totalTestCount);
        testResult += testAssert(G.deckCount[player], Gcopy.deckCount[player]-2, &totalTestCount);
        testResult += testAssert(G.supplyCount[silver], Gcopy.supplyCount[silver], &totalTestCount);
        if (testResult <= -1) {
            printf("card count checks failed\n");
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
        for (l=0; l<numPlayer; l++) {
            if (l !=player) {
                testResult += testAssert(G.deckCount[l], Gcopy.deckCount[l], &totalTestCount);
                testResult += testAssert(G.handCount[l], Gcopy.handCount[l], &totalTestCount);
                testResult += testAssert(G.discardCount[l], Gcopy.discardCount[l], &totalTestCount);
            }
         }

        if (testCaseResult >= 0) {
            printf("All silver cards case passed - player %d\n", player);
        } else {
            printf("All silver cards case failed - player %d\n", player);
            totalTestResult = -1;
            testCaseResult =0;
            testResult =0;
        }

    // All gold card test

        printf ("TESTING newAdventurer() - gold test :\n");

        memset(&G, 23, sizeof(struct gameState));   // clear the game state
        memset(&Gcopy, 23, sizeof(struct gameState));   // clear the game state
        r = initializeGame(numPlayer, k, seed, &G); // initialize a new game

        for (int k=0; k<player; k++) {
            endTurn(&G);
        }
        for (j = 0; j < maxHandCount; j++) {
            G.deck[player][j] = gold;
        }
        memcpy(&Gcopy, &G, sizeof(struct gameState));

#if (NOISY_TEST == 1)
    printf("deck card zero = %d\n", G.deck[player][0]);
    printf("hand card zero = %d\n", G.deck[player][0]);
    printf("deckCount = %d\n", G.deckCount[player]);
    printf("handCount = %d\n", G.handCount[player]);
#endif

        newAdventurer(&G, player);

#if (NOISY_TEST == 1)
    printf("deck card zero = %d\n", G.deck[player][0]);
    printf("hand card zero = %d\n", G.deck[player][0]);
    printf("deckCount = %d\n", G.deckCount[player]);
    printf("handCount = %d\n", G.handCount[player]);
#endif
        testResult += testAssert(G.handCount[player], Gcopy.handCount[player]+2, &totalTestCount);
        testResult += testAssert(G.deckCount[player], Gcopy.deckCount[player]-2, &totalTestCount);
        testResult += testAssert(G.supplyCount[gold], Gcopy.supplyCount[gold], &totalTestCount);
        if (testResult <= -1) {
            printf("card count checks failed\n");
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
        for (l=0; l<numPlayer; l++) {
           if (l !=player) {
               testResult += testAssert(G.deckCount[l], Gcopy.deckCount[l], &totalTestCount);
               testResult += testAssert(G.handCount[l], Gcopy.handCount[l], &totalTestCount);
               testResult += testAssert(G.discardCount[l], Gcopy.discardCount[l], &totalTestCount);
           }
        }

    //    for (j=0; j<numPlayer; j++) {
    //        if (j != )
    //        testResult += testAssert(G.handCount[j], Gcopy.handCount[j], &totalTestCount);
    //        testResult += testAssert(G.deckCount[j], Gcopy.deckCount[j], &totalTestCount);
    //    }

        if (testCaseResult >= 0) {
            printf("All golds cards case passed - player %d\n", player);
        } else {
            printf("All golds cards case failed - player %d\n", player);
            totalTestResult = -1;
            testCaseResult =0;
            testResult =0;
        }

// One treasure card test

            printf ("TESTING newAdventurer() - Fewer than treasure cards case :\n");

            memset(&G, 23, sizeof(struct gameState));   // clear the game state
            memset(&Gcopy, 23, sizeof(struct gameState));   // clear the game state
            r = initializeGame(numPlayer, k, seed, &G); // initialize a new game

            for (int k=0; k<player; k++) {
                endTurn(&G);
            }
            for (j = 1; j < maxHandCount; j++) {
                G.deck[player][j] = estate;
            }
            G.deck[player][0] = copper; // Set card [0] to a copper
            memcpy(&Gcopy, &G, sizeof(struct gameState));

#if (NOISY_TEST == 1)
    printf("deck card zero = %d\n", G.deck[player][0]);
    printf("hand card zero = %d\n", G.deck[player][0]);
    printf("deckCount = %d\n", G.deckCount[player]);
    printf("handCount = %d\n", G.handCount[player]);
#endif

            newAdventurer(&G, player);

#if (NOISY_TEST == 1)
            printf("deck card zero = %d\n", G.deck[player][0]);
    printf("hand card zero = %d\n", G.deck[player][0]);
    printf("deckCount = %d\n", G.deckCount[player]);
    printf("handCount = %d\n", G.handCount[player]);
#endif
            testResult += testAssert(G.handCount[player], Gcopy.handCount[player]+1, &totalTestCount);
            testResult += testAssert(G.deckCount[player], 0, &totalTestCount);
            testResult += testAssert(G.supplyCount[copper], Gcopy.supplyCount[copper], &totalTestCount);
            if (testResult <= -1) {
                printf("card count checks failed\n");
                testResult = 0;
                testCaseResult = -1;
            }
            testResult += testAssert(G.coins, Gcopy.coins, &totalTestCount);
            if (testResult <= -1) {
                printf("coins check failed\n");
                testResult =0;
                testCaseResult = -1;
            }
            testResult += testAssert(G.numBuys, Gcopy.numBuys, &totalTestCount);
            if (testResult <= -1) {
                printf("numBuys check failed\n");
                testResult =0;
                testCaseResult = -1;
            }
            testResult += testAssert(G.numActions, Gcopy.numActions, &totalTestCount);
            if (testResult <= -1) {
                printf("numActions check failed\n");
                testResult =0;
                testCaseResult = -1;
            }

            for (l=0; l<numPlayer; l++) {
                if (l !=player) {
                    testResult += testAssert(G.deckCount[l], Gcopy.deckCount[l], &totalTestCount);
                    testResult += testAssert(G.handCount[l], Gcopy.handCount[l], &totalTestCount);
                    testResult += testAssert(G.discardCount[l], Gcopy.discardCount[l], &totalTestCount);
                }
            }


            if (testCaseResult >= 0) {
                printf("The One treasure card case passed - player %d\n", player);
            } else {
                printf("The One treasure card case failed! - player %d\n", player);
                totalTestResult = -1;
                testResult =0;
                testCaseResult = 0;
            }

// // //
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
