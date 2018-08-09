/* -----------------------------------------------------------------------
 * unittest4.c
 *
 * Written by: Joe Beauchesne
 * ONID: beauchjo
 *
 * Last updated: 7/18/18
 *
 * This code heavily leverages the code provided in the classroom materials
 * that is said to be OK to use as a template.
 *
 * This code tests the buyCard function
 *
 * int buyCard(int supplyPos, struct gameState *state)
 *
 * This function buys a card. It sends a gameState and a card to buy, and then handles the
 * buy transaction.
 *
 *
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
    int totalCount = 0;
    int numPlayer = 4;
    int r;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G, Gcopy;
    int MAXSUPPLYPOS = 17;

// The first test just checks at game initialization

    printf ("TESTING buyCard():\n");


        // Initialize a gameState struct and what will be a copy of the struct

        memset(&G, 23, sizeof(struct gameState));   // clear the game state
        memset(&Gcopy, 23, sizeof(struct gameState));   // clear the game state
        r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
                if (r == -1) {
                    printf("Error initializing game\n");
                }

// At the start of a new game, the player should have 7 coppers and 3 estates between their deck and their hand, but
// their discard pile is empty.

// Populate the discard pile with a varying number of duchy cards for each user

// Test what happens when the number of buys is set to zero, should return -1

        G.numBuys = 0;
// Make a copy of the game state
        memcpy(&Gcopy, &G, sizeof(struct gameState));

        for (int i=0; i<MAXSUPPLYPOS; i++) {
            totalTestResult += testAssert(buyCard(i, &G), -1, &totalTestCount);
            totalCount++;
        }
// Verify that the game state has not changed
        totalTestResult += testAssert(memcmp(&Gcopy, &G, sizeof(struct gameState)),0, &totalTestCount);
//        printf("test result after no buys = %d\n", totalTestResult);

    if (totalTestResult >= 0) {
        printf("buyCard does not allow cards to be purchased when no buys are available\n");
    }


#if (NOISY_TEST == 1)
    printf("nobuy results = %d\n", totalTestResult );
#endif

// Test what happens when the number of buys is set to one, and number of coins is set to 0;
// No transaction can occur

        G.numBuys = 1;
        G.coins = 0;
// Copy the prebuy state
        memcpy(&Gcopy, &G, sizeof(struct gameState));

        for (i=1; i<MAXSUPPLYPOS; i++) {
            // Try to buy any of the 26 card positions except 0 or 4
            if (i != 4) {
                totalTestResult += testAssert(buyCard(i, &G), -1, &totalTestCount);
            } else {
                totalTestResult += testAssert(buyCard(i, &G), 0, &totalTestCount);
                totalTestResult += testAssert(G.coins, 0, &totalTestCount);
//                printf("coin result %d = %d\n", i, totalTestResult);

                totalTestResult += testAssert(G.discardCount[0], Gcopy.discardCount[0]+1, &totalTestCount);
                totalTestResult += testAssert(G.discardCount[1], Gcopy.discardCount[1], &totalTestCount);
                totalTestResult += testAssert(G.discardCount[2], Gcopy.discardCount[2], &totalTestCount);
                totalTestResult += testAssert(G.discardCount[3], Gcopy.discardCount[3], &totalTestCount);
                totalTestResult += testAssert(G.numBuys+1,Gcopy.numBuys, &totalTestCount);
//                printf("numBuys result %d = %d\n", i, totalTestResult);
            }
//            printf("test result after no coins %d = %d\n", i, totalTestResult);
        }
    if (totalTestResult >= 0) {
        printf("buyCard does not allow cards to be purchased when insufficient funds are available!\n");
    }


// Supply Pile empty test
// Reinitialize the game
        printf("Starting empty supply pile tests!\n");
        memset(&G, 23, sizeof(struct gameState));   // clear the game state
        memset(&Gcopy, 23, sizeof(struct gameState));   // clear the game state
        r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
        G.numBuys = 1;
        G.coins = 10;

        for (int i=0; i<MAXSUPPLYPOS; i++) {
            G.supplyCount[i]=0;
        }

    memcpy(&Gcopy, &G, sizeof(struct gameState));

    for (i=0; i<MAXSUPPLYPOS; i++) {
        totalTestResult += testAssert(buyCard(i, &G), -1, &totalTestCount);
// Since the supply piles are empty, no transactions should be made;
        for (int j=0; j<numPlayer; j++) {
            totalTestResult += testAssert(G.discardCount[j], Gcopy.discardCount[j], &totalTestCount);
            totalTestResult += testAssert(G.deckCount[j], Gcopy.deckCount[j], &totalTestCount);
            totalTestResult += testAssert(G.handCount[j], Gcopy.handCount[j], &totalTestCount);
        }

    }

    if (totalTestResult >= 0) {
        printf("buyCard does not allow a card to be bought from empty piles!\n");
    }

// Normal card test - 1 buy
// Reinitialize the game
    printf("Starting regular purchase test!\n");


    for (i=0; i<MAXSUPPLYPOS; i++) {
        memset(&G, 23, sizeof(struct gameState));   // clear the game state
        memset(&Gcopy, 23, sizeof(struct gameState));   // clear the game state
        r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
        G.numBuys = 1;
        G.coins = 10;

        memcpy(&Gcopy, &G, sizeof(struct gameState));
#if (NOISY_TEST == 1)
        printf("coins before = %d\n", G.coins );
        printf("buys before = %d\n", G.numBuys );
#endif
        totalTestResult += testAssert(buyCard(i, &G), 0, &totalTestCount);
#if (NOISY_TEST == 1)
        printf("coins after  = %d\n", G.coins );
        printf("buys after = %d\n", G.numBuys );
#endif
        totalTestResult += testAssert(G.coins, Gcopy.coins-getCost(i), &totalTestCount);
        totalTestResult += testAssert(G.numBuys, Gcopy.numBuys-1, &totalTestCount);

    }

    if (totalTestResult >= 0) {
        printf("buyCard passes the normal purchase test!\n");
    }





        if (totalTestResult >= 0) {
            printf("All tests passed - buyCard function!\n");
            printf("Total checks made = %d\n", totalTestCount);
        } else {
            printf("There was at least one test failure - buyCard function\n");
            printf("Total checks made = %d\n", totalTestCount);
        }



    return 0;
}
