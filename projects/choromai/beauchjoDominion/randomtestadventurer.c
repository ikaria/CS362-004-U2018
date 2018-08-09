/* -----------------------------------------------------------------------
 * randomtestadventurer.c
 *
 * Written by: Joe Beauchesne
 * ONID: beauchjo
 *
 * Last updated: 8/1/18
 *
 * This code heavily leverages the code provided in the classroom materials
 * that is said to be OK to use as a template.
 *
 * This code randomly tests the newAdventurer function, it can do this directly or through the use
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

/*int checkAdventurer(int p, struct gameState *post) {
    int r;

    r = drawCard (p, post);

    assert (r == 0);
}
*/

int main() {
    struct gameState G;
    struct gameState Gcopy;
//    int i,j;
//    int seed = 5;
    int totalTestResult = 0;
    int testResult = 0;
    int testCaseResult = 0;
    int totalTestCount = 0;
    int passingTestCases = 0;
    int failingTestCases = 0;
    int i,p,n;
    int card;
    int discardValBefore, discardValAfter;
    int maxCards = 17;
    int numCards;

    SelectStream(2);
    PutSeed(3);

// Total number of test runs
    for (n = 0; n < 10000; n++) {
        testResult = 0;
        testCaseResult = 0;
//        seed = floor(Random() * 1000);

// Randomly fill the game struct with ascii characters
        for (i = 0; i < sizeof(struct gameState); i++) {
            ((char*)&G)[i] = floor(Random() * 256);
        }

        p = floor(Random() * 4); // Should return 0, 1, 2, 3
        numCards = floor(Random() * MAX_DECK);
        G.deckCount[p] = numCards;
// Randomly fill cards into the deck
        for (int j=0; j<G.deckCount[p]; j++) {
            card = floor(Random() * maxCards);
            G.deck[p][j]=card;
        }
        G.discardCount[p] = floor(Random() * MAX_DECK);
        for (int j=0; j<G.discardCount[p]; j++) {
            card = floor(Random() * maxCards);
            G.discard[p][j]=card;
        }
        G.handCount[p] = floor(Random() * MAX_HAND);
        for (int j=0; j<G.handCount[p]; j++) {
            card = floor(Random() * maxCards);
            G.hand[p][j]=card;
        }

        memcpy(&Gcopy, &G, sizeof(struct gameState));

#if (NOISY_TEST == 1)
        printf("before deckCount = %d\n", G.deckCount[p]);
        printf("before handCount = %d\n", G.handCount[p]);
        printf("before discardCount = %d\n", G.discardCount[p]);
#endif

        newAdventurer(&G, p);
#if (NOISY_TEST == 1)
        printf("after deckCount = %d\n", G.deckCount[p]);
        printf("after handCount = %d\n", G.handCount[p]);
        printf("after discardCount = %d\n", G.discardCount[p]);
#endif
        testResult += testAssert(G.coins, Gcopy.coins, &totalTestCount);
        if (testResult <= -1) {
            printf("coins test failed\n");
            testResult = 0;
            testCaseResult = -1;
        }
// Verify number of buys hasn't changed
        testResult += testAssert(G.numBuys, Gcopy.numBuys, &totalTestCount);
        if (testResult <= -1) {
            printf("numBuys check failed\n");
            printf("after %d   before  %d \n", G.numBuys, Gcopy.numBuys);
            testResult = 0;
            testCaseResult = -1;
        }
// These are only valid if the deck isn't emptied...if it is, then perhaps there's an issue
// but that should be covered in the other cases.
        if (G.deckCount != 0) {
            testResult += testAssert(G.handCount[p], Gcopy.handCount[p]+2, &totalTestCount);
            if (testResult <= -1) {
                printf("hand count check failed\n");
                testResult = 0;
                testCaseResult = -1;
            }
                testResult += testAssert(G.deckCount[p] + G.discardCount[p], Gcopy.deckCount[p] + Gcopy.discardCount[p] - 2,
                                         &totalTestCount);
                if (testResult <= -1) {
                    printf("total cards in deck + discard check failed\n");
                    testResult = 0;
                    testCaseResult = -1;
                }
            }


// Check counts of treasure cards in discard pile before

        discardValAfter=0;
        discardValBefore=0;

        for (i=0; i<Gcopy.discardCount[p]; i++) {
            if ((Gcopy.discard[p][i] == 4) || (Gcopy.discard[p][i] == 5) || (Gcopy.discard[p][i] == 6)) {
                discardValBefore++;
            }
        }

        for (i=0; i<G.discardCount[p]; i++) {
            if ((G.discard[p][i] == 4) || (G.discard[p][i] == 5) || (G.discard[p][i] == 6)) {
                discardValAfter++;
            }
        }
#if (NOISY_TEST == 1)
        printf("Discard values counts before/after  = %i   %i  ", discardValBefore, discardValAfter);
#endif
        testResult += testAssert(discardValAfter, discardValBefore, &totalTestCount);
        if (testResult <= -1) {
            printf("discard pile value check failed\n");
            testResult = 0;
            testCaseResult = -1;
        }

        if (testCaseResult >= 0) {
//            printf("Case Result - All tests passed!\n");
            passingTestCases++;
        } else {
            printf("Case Result - There was at least one test failure during this test case \n");
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
        printf("Final Results - There was at least one test failure during at least one test case\n");
        printf("Total cases = %d\n", passingTestCases + failingTestCases);
        printf("Total passing cases = %d\n", passingTestCases);
        printf("Total failing cases = %d\n", failingTestCases);

    }

    return 0;
}
