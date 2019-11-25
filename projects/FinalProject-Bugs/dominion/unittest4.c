/**************************************************************************
 * Author: Chung Yu Weng
 * Test target: Bug04
 *              The bug causes isGameOver function to count only 25 of all
 *              27 cards. Two cards (sea_hag and treasure_map) would not be 
 *              counted because of the bug.
 * ***********************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

#define TEST_BUG "BUG04"

void assertM(char testName[],int result)
{
    if(result)
        printf("Testing target: \"%s\": PASS!\n", testName);
    else
        printf("Testing target: \"%s\": FAIL!\n", testName);
}

int main()
{
    printf("Begin Testing %s:\n", TEST_BUG);
    printf("TEST: sea_hag, treasure_map, and mine cardCount are set to 0 => the game should be over\n");
    //initialize variables
    struct gameState state; //game state after calling cardEffect
    int seed = 789;
    int numPlayer = 2;
    int kCards[10] = {adventurer, gardens, embargo, village, minion, mine, treasure_map, sea_hag, tribute, smithy};
    initializeGame(numPlayer,kCards, seed, &state);
    state.supplyCount[sea_hag] = 0;
    state.supplyCount[treasure_map] = 0;
    state.supplyCount[mine] = 0;
    int gameover = 0;

    //call the function
    gameover = isGameOver(&state);

    //assert the results
    assertM("game should be over", gameover == 1);

    printf("Test completed!\n");

    return 0;
}
