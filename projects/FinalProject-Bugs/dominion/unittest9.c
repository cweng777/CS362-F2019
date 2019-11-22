/* -----------------------------------------------------------------------
 * Unit test for bug9
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
    printf ("--- UNIT TEST 9 STARTS ---\n");
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(2, k, 1000, &G); // initialize a new game

    int currentPlayerHand[5] = {tribute, silver, silver, gold, estate};
    memcpy(G.hand[0], currentPlayerHand, sizeof(int) * 5); // set player's hand

    // set top 2 cards of next player's deck
    G.deckCount[nextPlayer] = 2;
    G.deck[nextPlayer][0] = gold;
    G.deck[nextPlayer][1] = gold;

    // save previous state
    int coinsBefore = G.coins;
    int numActionsBefore = G.numActions;
    int handCountBefore = G.handCount[currentPlayer];

    cardEffect(tribute, 0, 0, 0, &G, 0, NULL);
    assertEqual(G.coins, coinsBefore + 2, "Check coins become pre + 2");
    assertEqual(G.numActions, numActionsBefore, "Check numActions unchanged");
    assertEqual(G.handCount[currentPlayer], handCountBefore - 1, "Check current player's handCount");

    printf("--- UNIT TEST 9 ENDS ---\n");
    printf("--------------------------\n");
    return 0;
}