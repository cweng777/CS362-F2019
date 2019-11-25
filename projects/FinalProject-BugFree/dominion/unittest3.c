/**************************************************************************
 * Author: Chung Yu Weng
 * Test target: Bug03
 *              Checking the cost of the card they trash against the cost of 
 *              the card they want to buy is incorrect when playing Remodel.
 * ***********************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

#define TEST_BUG "BUG03"

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
    //initialize common variables
    int seed = 789;
    int numPlayer = 2;
    int kCards[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
    struct gameState state; //game state after calling cardEffect
    struct gameState stateT; //game state before calling cardEffect
    memset(&state,0,sizeof(struct gameState)); //clear state
    //memset(&stateT,0,sizeof(struct gameState)); //clear stateT

    initializeGame(numPlayer,kCards, seed, &state); 

    memcpy(&stateT, &state, sizeof(struct gameState));

    //---------------TEST 1-----------------------------------------------------
    printf("TEST 1: hand[cruuentPlayer][choice1] = gardens, choice2 = mine => discardCount + 2, gain a mine card(ID:11) \n");
    int currentPlayer = whoseTurn(&state);
    state.hand[currentPlayer][0] = remodel; 
    int handPos = 0;
    int choice1 = 1;
    state.hand[currentPlayer][choice1] = gardens;
    int choice2 = mine; 
    int coin_bonus = 0; //tracks coins gain from actions

    //call the function
    cardEffect(remodel, choice1, choice2, 0, &state, handPos, &coin_bonus);

    //assert the results
    printf("discardCount = %d, expected = %d\n", state.discardCount[currentPlayer], stateT.discardCount[currentPlayer] + 2);
    assertM("discard pile count", state.discardCount[currentPlayer] == stateT.discardCount[currentPlayer] + 2);
    
    if (state.discardCount[currentPlayer] >= 2)
    {
        printf("ID of gained card = %d, expected = %d\n", state.discard[currentPlayer] [state.discardCount[currentPlayer] - 2 ], mine);
        assertM("ID of gained card", state.discard[currentPlayer] [state.discardCount[currentPlayer] - 2 ] == mine);

    }
    else
    {
        printf("no card gained\n");
        assertM("ID of gained card", 0);
    }

    //---------------TEST 2------------------------------------------------------
    printf("TEST 2: hand[cruuentPlayer][choice1] = gardens, choice2 = province => violate game rule, should be no change \n");
    memcpy(&state, &stateT, sizeof(struct gameState));
    currentPlayer = whoseTurn(&state);
    state.hand[currentPlayer][0] = remodel; 
    handPos = 0;
    choice1 = 1;
    state.hand[currentPlayer][choice1] = gardens;
    choice2 = province; 
    coin_bonus = 0; //tracks coins gain from actions

    //call the function
    cardEffect(remodel, choice1, choice2, 0, &state, handPos, &coin_bonus);

    //assert the results
    printf("discardCount = %d, expected = %d\n", state.discardCount[currentPlayer], stateT.discardCount[currentPlayer]);
    assertM("discard pile count", state.discardCount[currentPlayer] == stateT.discardCount[currentPlayer]);

    printf("Test completed!\n");

    return 0;
}
