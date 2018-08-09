/* -----------------------------------------------------------------------
 * unittest2.c
 *
 * Written by: Joe Beauchesne
 * ONID: beauchjo
 *
 * Last updated: 7/18/18
 *
 * This code heavily leverages the code provided in the classroom materials
 * that is said to be OK to use as a template.
 *
 * This code tests the gainCard function
 *
 * int gainCard(int supplyPos, struct gameState *state, int toFlag, int player)
 *
 * Demonstration of how to write unit tests for dominion-base
 * Include the following lines in your makefile:
 *
 * unittest2: unittest2.c dominion.o rngs.o
 *      gcc -o unittest2 -g  unittest2.c dominion.o rngs.o $(CFLAGS)
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
//    int i;
    int seed = 5;
//    int testAssertResult=0;
    int totalTestResult=0;
    int supplyPos = 0;
    int toFlag = 0;
    int numPlayer = 4;
    int players, r;
    int handCount = 20;
    int totalTestCount = 0;
//    int bonus;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G;
//    int maxHandCount = 5;
    int maxSupplyPos = 27;

// Test with a card not present in the game

    printf ("TESTING gainCard():\n");
    for (supplyPos = 0; supplyPos < maxSupplyPos; supplyPos++)
    {
        for (toFlag = 0; toFlag <= 2; toFlag++) {
            for (players = 0; players <= numPlayer; players++) {
#if (NOISY_TEST == 1)
                printf("Test %d player with supply position %d with %d trash status.\n", players, supplyPos, toFlag);
#endif
                memset(&G, 23, sizeof(struct gameState));   // clear the game state
                r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
                if (r == -1) {
                    totalTestResult = -1;
                }
                G.handCount[players] = handCount;                 // set the number of cards in a hand
                G.deckCount[players] = handCount;
                G.discardCount[players] = handCount;
                int cardResult = gainCard(supplyPos, &G, toFlag, players);
//                printf("Function result = %d\n", cardResult);
#if (NOISY_TEST == 1)
                if (cardResult == -1) {
                    printf("Card %d is not used in the game\n", supplyPos);
                }
                printf("toFlag = %d\n", toFlag);
                printf("G.handcount = %d\n", G.handCount[players]);
                printf("G.deckcount = %d\n", G.deckCount[players]);
                printf("G.discardcount = %d\n", G.discardCount[players]);
#endif
                if ((toFlag == 0 && cardResult != -1)) {
                    totalTestResult += testAssert(G.discardCount[players], handCount + 1, &totalTestCount);
                    totalTestResult += testAssert(G.deckCount[players], handCount, &totalTestCount);
                    totalTestResult += testAssert(G.handCount[players], handCount, &totalTestCount);
                }

                if ((toFlag == 1 && cardResult != -1)) {
                    totalTestResult += testAssert(G.deckCount[players], handCount + 1, &totalTestCount);
                    totalTestResult += testAssert(G.discardCount[players], handCount, &totalTestCount);
                    totalTestResult += testAssert(G.handCount[players], handCount, &totalTestCount);
                }

                if ((toFlag == 2) && (cardResult != -1)) {
                    totalTestResult += testAssert(G.handCount[players], handCount + 1, &totalTestCount);
                    totalTestResult += testAssert(G.deckCount[players], handCount, &totalTestCount);
                    totalTestResult += testAssert(G.discardCount[players], handCount, &totalTestCount);
                }
// For cards 17 and higher, which aren't used in this test game, we should show a failure if this isn't de
                if ((supplyPos >= 17) && (cardResult != -1)) {
                    totalTestResult--;
                }

            }
            if (totalTestResult >= 0) {
                printf("A loop of cases passed - gain card!\n");
            }
        }
    }

    if (totalTestResult >= 0) {
        printf("All tests passed - gain card!\n");
        printf("Total asserts conducted - %d cases\n", totalTestCount);
    } else {
        printf("There was at least one test failure\n");
        printf("Total asserts conducted - %d cases\n", totalTestCount);
    }



    return 0;
}
