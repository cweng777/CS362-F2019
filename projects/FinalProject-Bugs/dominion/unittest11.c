/* -----------------------------------------------------------------------
 * Unit test for bug11
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

    // set kingdom cards
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};

    printf ("--------------------------\n");
    printf ("--- UNIT TEST 11 STARTS ---\n");
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(2, k, 1000, &G); // initialize a new game

    int currentPlayerHand[5] = {minion, silver, silver, silver, estate};
    memcpy(G.hand[0], currentPlayerHand, sizeof(int) * 5); // set player's hand
    int numActionsBefore = G.numActions;

    int result = cardEffect(minion, 0, 0, 0, &G, 0, NULL);
    assertEqual(result, -1, "Check cardEffect returns -1");
    assertEqual(numActionsBefore, G.numActions, "Check numActions unchanged");

    printf("--- UNIT TEST 11 ENDS ---\n");
    printf("--------------------------\n");
    return 0;
}