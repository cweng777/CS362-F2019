/**************************************************************************
 * Author: Chung Yu Weng
 * Test target: Bug01 Chosen card is not trashed when playing Mine
 * ***********************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

#define TEST_BUG "BUG01"

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
    printf("TEST: hand[cruuentPlayer][choice1] = copper, choice2 = silver => no change in discard pile \n");
    //initialize variables
    int seed = 789;
    int numPlayer = 2;
    int kCards[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
    struct gameState state;
    //struct gameState stateT;
    memset(&state,0,sizeof(struct gameState)); //clear state
    //memset(&stateT,0,sizeof(struct gameState)); //clear stateT

    initializeGame(numPlayer,kCards, seed, &state); 

    //memcpy(&stateT, &state, sizeof(struct gameState));
    int currentPlayer = whoseTurn(&state);
    state.hand[currentPlayer][0] = mine; 
    int handPos = 0;
    int choice1 = 1;
    state.hand[currentPlayer][choice1] = copper;
    int choice2 = silver; 
    int coin_bonus = 0; //tracks coins gain from actions

    //calculate chosen card count in current player's discard pile before calling cardEffect
    int i;
    int countBefore = 0; ///Pre chosen card count in discard pile
    for (i=0; i<state.discardCount[currentPlayer]; i++)
    {
        if(state.discard[currentPlayer][i] == copper)
            countBefore++;
    }

    //call the function
    cardEffect(mine, choice1, choice2, 0, &state, handPos, &coin_bonus);

    //calculate chosen card count in current player's discard pile after calling cardEffect
    int countAfter = 0; ///Post chosen card count in discard pile
    for (i=0; i<state.discardCount[currentPlayer]; i++)
    {
        if(state.discard[currentPlayer][i] == copper)
            countAfter++;
    }

    //assert the results
    printf("copper card count in current player's discard pile = %d, expected = %d\n", countAfter, countBefore);
    assertM("Bug01 copper card count in discard pile not change", countAfter == countBefore);

    printf("Test completed!\n");

    return 0;
}
