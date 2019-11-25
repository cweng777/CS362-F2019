/* -----------------------------------------------------------------------
 * Unit test for bug10
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

static void assertEqual(int actual, int expected, char *msg) {
    printf("%s: ", msg);
    if (actual == expected) {
        printf("PASS!: got %d\n", actual);
        return;
    }
    else {
        printf("FAILED: %d expected, but got %d.\n", expected, actual);
        return;
    }
}

int main() {
    struct gameState G;
    int currentPlayer = 0;
    int nextPlayer = 1;

    // set kingdom cards
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};

    printf ("--------------------------\n");
    printf ("--- UNIT TEST 10 STARTS ---\n");
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(2, k, 1000, &G); // initialize a new game

    int currentPlayerHand[5] = {ambassador, silver, silver, gold, estate};
    memcpy(G.hand[0], currentPlayerHand, sizeof(int) * 5); // set player's hand
    int handCountBefore = G.handCount[currentPlayer];
    int nextPlayerHandCountBefore = G.handCount[nextPlayer];

    int result = cardEffect(ambassador, 1, 1, 0, &G, 0, NULL);
    assertEqual(result, 0, "Check cardEffect returns 0");
    assertEqual(G.handCount[currentPlayer], handCountBefore - 2, "Check current player's handCount");
    assertEqual(G.handCount[nextPlayer], nextPlayerHandCountBefore + 1, "Check next player's handCount");

    printf("--- UNIT TEST 10 ENDS ---\n");
    printf("--------------------------\n");
    return 0;
}