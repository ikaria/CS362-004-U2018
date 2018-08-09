/* -----------------------------------------------------------------------
 * unittest3.c
 *
 * Written by: Joe Beauchesne
 * ONID: beauchjo
 *
 * Last updated: 7/16/18
 *
 * This code heavily leverages the code provided in the classroom materials
 * that is said to be OK to use as a template.
 *
 * This code tests the fullDeckCount function
 *
 * int fullDeckCount(int player, int card, struct gameState *state)
 *
 * This function returns the number of cards of a specific type in a players hand
 * by adding the number in their deck, hand, and discard pile.
 *
 * Each player gets a different number of cards to the discard pile to ensure some of the robustness of the code.
 *
 * The test function does the following:
 * 1) Tests the standard game setup with 7 coppers and 3 estates. This covers cards in the hand and the deck.
 * 2) Then the MAXDECK is filled through the discard pile to add 490 duchys so that the player has a total of
 *    500 cards. Then the count is checked against cards
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
    int i;
    int seed = 5;
    int totalTestResult=0;
    int totalTestCount=0;
    int numPlayer = 4;
    int r;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G, Gcopy;

// The first test just checks at game initialization

    printf ("TESTING fullDeckCount():\n");

#if (NOISY_TEST == 1)
        printf("Test %d player with supply position %d with %d trash status.\n", players, supplyPos, toFlag);
#endif
// Initialize an array of card counts to 0;

    for (int j=0; j<numPlayer; j++) {

        int cardCounts[27]= {0};
        memset(&G, 23, sizeof(struct gameState));   // clear the game state
        memset(&Gcopy, 23, sizeof(struct gameState));   // clear the game state
        r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
        if (r == -1) {
            totalTestResult = -1;
        }

// At the start of a new game, the player should have 7 coppers and 3 estates between their deck and their hand, but
// their discard pile is empty.

// Populate the discard pile with a varying number of duchy cards for each user

        for (i=0; i<490; i++) {
            G.discard[j][i] = duchy;
            G.discardCount[j]++;
        }
        G.discardCount[j]-=10*j;
#if (NOISY_TEST == 1)
        printf("discardCount = %d\n", G.discardCount[j] );
#endif

// We copy the gamestate to make sure that it hasn't changed. This function should not affect
// the game state at all at this point.

        memcpy(&Gcopy, &G, sizeof(struct gameState));

        for (i=0; i<27; i++) {
            // get the fullDeckCount for this player
            cardCounts[i] = fullDeckCount(j, i, &G);
#if (NOISY_TEST == 1)
            printf("Card %d  count  %d\n", i, cardCounts[i]);
#endif
        }
// G.discardCount[j]-=10*j;  // This tests that the memcmp function works ok - if the line is uncommented
// then the first test below will fail.
// Verify that the game state is unchanged after running these tests

        totalTestResult += testAssert(memcmp(&Gcopy, &G, sizeof(struct gameState)),0, &totalTestCount);
        if (totalTestResult >= 0) {
            printf("fullDeckCount does not change the game state - pass for a specific player!\n");
        }
        totalTestResult += testAssert(cardCounts[0],0, &totalTestCount);
        if (totalTestResult >= 0) {
            printf("fullDeck count works when no cards of the requested type is present for a specific player!\n");
        }
        totalTestResult += testAssert(cardCounts[1],3, &totalTestCount);
        if (totalTestResult >= 0) {
            printf("fullDeck count works when a small number cards of the requested type is present for a specific player!\n");
        }
        totalTestResult += testAssert(cardCounts[2],490-10*j, &totalTestCount);
        if (totalTestResult >= 0) {
            printf("fullDeck count correctly accounts for cards in the discard pile for a specific player!\n");
        }
//        totalTestResult += testAssert(cardCounts[3],0, &totalTestCount);
//        totalTestResult += testAssert(cardCounts[4],7, &totalTestCount);
    }

        if (totalTestResult >= 0) {
            printf("All tests passed - full deck count function!\n");
            printf("Total asserts conducted - %d cases\n", totalTestCount);
        } else {
            printf("There was at least one test failure\n");
            printf("Total asserts conducted - %d cases\n", totalTestCount);
        }



    return 0;
}
