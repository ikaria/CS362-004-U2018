/* -----------------------------------------------------------------------
 * unittest1.c
 *
 * Written by: Joe Beauchesne
 * ONID: beauchjo
 *
 * Last updated: 7/16/18
 *
 * This code heavily leverages the code provided in the classroom materials
 * that is said to be OK to use as a template.
 *
 * This code, just like the provided code, tests the updateCoins function.
 *
 * Demonstration of how to write unit tests for dominion-base
 * Include the following lines in your makefile:
 *
 * unittest1: unittest1.c dominion.o rngs.o
 *      gcc -o testUpdateCoins -g  testUpdateCoins.c dominion.o rngs.o $(CFLAGS)
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
    int testAssertResult=0;
    int totalTestResult=0;
    int totalTestCount = 0;
    int numPlayer = 4;
    int maxBonus = 10;
    int players, r, handCount;
    int bonus;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G;
    int maxHandCount = 25;

    // arrays of all coppers, silvers, and golds
    // In reality there can never be this many cards in any given hand but
    // but this bounds the problem to match the max hand of the dominion code

    int coppers[MAX_HAND];
    int silvers[MAX_HAND];
    int golds[MAX_HAND];
    int smithys[MAX_HAND];
    int villages[MAX_HAND];
    int adventurers[MAX_HAND];


    for (i = 0; i < MAX_HAND; i++)
    {
        coppers[i] = copper;
        silvers[i] = silver;
        golds[i] = gold;
        smithys[i] = smithy;
        villages[i] = village;
        adventurers[i] = adventurer;

    }

    printf ("TESTING updateCoins():\n");
    for (players = 0; players < numPlayer; players++)
    {
        for (handCount = 0; handCount <= maxHandCount; handCount++)
        {
            for (bonus = 0; bonus <= maxBonus; bonus++)
            {
#if (NOISY_TEST == 1)
                printf("Test player %d with %d treasure card(s) and %d bonus.\n", players, handCount, bonus);
#endif
                memset(&G, 23, sizeof(struct gameState));   // clear the game state
                r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
                if (r == -1) {
                    totalTestResult = -1;
                    printf("Game state initialization failure\n");
                }
                G.handCount[players] = handCount;                 // set the number of cards in a hand
                memcpy(G.hand[players], coppers, sizeof(int) * handCount); // set all the cards to copper
                updateCoins(players, &G, bonus);
#if (NOISY_TEST == 1)
                printf("G.coins = %d, expected = %d\n", G.coins, handCount * 1 + bonus, &totalTestCount);
#endif
//                assert(G.coins == handCount * 1 + bonus); // check if the number of coins is correct
                testAssertResult=testAssert(G.coins, handCount * 1 + bonus, &totalTestCount);
                if (testAssertResult == -1) {
                    totalTestResult = -1;
                }
                memcpy(G.hand[players], silvers, sizeof(int) * handCount); // set all the cards to silver
                updateCoins(players, &G, bonus);
#if (NOISY_TEST == 1)
                printf("Silver - G.coins = %d, expected = %d\n", G.coins, handCount * 2 + bonus);
#endif
//                assert(G.coins == handCount * 2 + bonus); // check if the number of coins is correct
                testAssertResult=testAssert(G.coins, handCount * 2 + bonus, &totalTestCount);
                if (testAssertResult == -1) {
                    totalTestResult = -1;
                }
                memcpy(G.hand[players], golds, sizeof(int) * handCount); // set all the cards to gold
                updateCoins(players, &G, bonus);
#if (NOISY_TEST == 1)
                printf("Gold - %d - G.coins = %d, expected = %d\n", G. handCount[players], G.coins, handCount * 3 + bonus);
#endif
//                assert(G.coins == handCount * 3 + bonus); // check if the number of coins is correct
                testAssertResult=testAssert(G.coins, handCount * 3 + bonus, &totalTestCount);
                if (testAssertResult == -1) {
                    totalTestResult = -1;
                }
// Test that smithy cards return 0 coins
                memcpy(G.hand[players], smithys, sizeof(int) * handCount); // set all the cards to gold
                updateCoins(players, &G, bonus);
#if (NOISY_TEST == 1)
                printf("G.coins = %d, expected = %d\n", G.coins, handCount * 0 + bonus);
#endif
                testAssertResult=testAssert(G.coins, handCount * 0 + bonus, &totalTestCount);
                if (testAssertResult == -1) {
                    totalTestResult = -1;
                }

// Test that adventurer cards return 0 coins
                memcpy(G.hand[players], adventurers, sizeof(int) * handCount); // set all the cards to gold
                updateCoins(players, &G, bonus);
#if (NOISY_TEST == 1)
                printf("G.coins = %d, expected = %d\n", G.coins, handCount * 0 + bonus);
#endif
                testAssertResult=testAssert(G.coins, handCount * 0 + bonus, &totalTestCount);
                if (testAssertResult == -1) {
                    totalTestResult = -1;
                }
// Test that village cards return 0 coins
                memcpy(G.hand[players], villages, sizeof(int) * handCount); // set all the cards to gold
                updateCoins(players, &G, bonus);
#if (NOISY_TEST == 1)
                printf("G.coins = %d, expected = %d\n", G.coins, handCount * 0 + bonus);
#endif
                testAssertResult=testAssert(G.coins, handCount * 0 + bonus, &totalTestCount);
                if (testAssertResult == -1) {
                    totalTestResult = -1;
                }
            }
            if (totalTestResult >= 0) {
                printf ("A bonus test case passed\n");
            }
        }
        if (totalTestResult >= 0) {
            printf ("A hand count test case passed\n");
        }
    }

    if (totalTestResult >= 0) {
        printf("All tests passed - updateCoins function!\n");
        printf("Total asserts conducted - %d cases\n", totalTestCount);
    } else {
        printf("There was at least one test failure\n");
        printf("Total asserts conducted - %d cases\n", totalTestCount);
    }



    return 0;
}
