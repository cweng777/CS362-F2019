/**************************************************************************
 * Author:Chung Yu Weng
 * Test target: minion refactored function
 * ***********************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

#define TEST_FUNCTION "handleMinion"

int handleMinion(int currentPlayer, int choice1, int choice2, struct gameState *state, int handPos);

void assertM(char testName[],int result)
{
    if(result)
        printf("Testing target: \"%s\": PASS!\n", testName);
    else
        printf("Testing target: \"%s\": FAIL!\n", testName);
}

int main()
{
    //initialize common variables
    int seed = 789;
    int numPlayer = 2;
    int kCards[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
    struct gameState state;
    struct gameState stateT;
    memset(&state,0,sizeof(struct gameState)); //clear state
    //int handPos = 0; //test

    initializeGame(numPlayer,kCards, seed, &state);
     
    int currentPlayer = whoseTurn(&state);
    int choice1 = 0; //initialize choice1
    int choice2 = 0; //initialize choice2
    //int bonus = 0; //initialize bonus

    printf("Begin Testing %s():\n", TEST_FUNCTION);
    //---------------TEST 1-----------------------------------------------------
    printf("TEST 1 : choice 1 = 1, choice 2 = 0 => +1 action, +2 coins---------------\n"); 
    //set TEST 1 variables
    memcpy(&stateT, &state, sizeof(struct gameState));
    choice1 = 1;
    choice2 = 0;
    int testAction = stateT.numActions;  //for later expected action value
    int testCoin = stateT.coins; //for later expected coins value

    //call the refactored functions
    handleMinion(currentPlayer, choice1, choice2, &stateT, 0);

    //assert the results
     //test +1 action
    printf("actions = %d, expected = %d\n", stateT.numActions, testAction + 1);
    assertM("+1 action", stateT.numActions == testAction + 1);
    printf("------------------ \n");
     //test +2 coins
    printf("coins= %d, expected = %d\n", stateT.coins, testCoin + 2);
    assertM("+2 coins", stateT.coins == testCoin + 2);

    printf("--------------------------------------------\n");
    //---------------TEST 2-----------------------------------------------------
    printf("TEST 2 : choice 1 = 0, choice 2 = 1 => +1 action, currentPlayer discard and redraw 4 cards, otherPlayer redraw if hand cards > 4---------------\n"); 
    //set TEST 2 variables
    memcpy(&stateT, &state, sizeof(struct gameState));
    choice1 = 0;
    choice2 = 1;
    testAction = stateT.numActions;  //for later expected  action value
    stateT.handCount[currentPlayer] = 2; //set currentPlayer still has two cards in hand 
    int otherPlayer = currentPlayer + 1; //here assume 2 player(discuss this in doucmunet)
    stateT.handCount[otherPlayer] = 5; //set otherPlayer has 5 cards in hand
    stateT.discardCount[currentPlayer] = 0; //assume this run is at the beginning of the game, currentPlayer has zero cards in discard pile 
    stateT.discardCount[otherPlayer] = 0; //assume this run is at the bginning of the game, otherPlayer has zero cards in discard pile

    printf("starting assumeing status: currenPlayer handCount 2, discardCount 0; otherPlayer handCount 5, discardCount 0");

    //call the refactored functions
    handleMinion(currentPlayer, choice1, choice2, &stateT, 0);

    //assert the results
     //test +1 action
    printf("actions = %d, expected = %d\n", stateT.numActions, testAction + 1); 
    assertM("+1 action", stateT.numActions == testAction + 1);
    printf("------------------ \n");
     //test currentPlayer discard and redraw 4 cards
    printf("currentPlayer handCount = %d, expected = 4\n", stateT.handCount[currentPlayer]); 
    printf("currentPlayer discardCount = %d, expected = 4\n", stateT.discardCount[currentPlayer]); 
    assertM("currentPlayer redraw", stateT.handCount[currentPlayer] == 4 && stateT.discardCount[currentPlayer] == 4);
    printf("------------------ \n");

     //test otherPlayer discard and redraw 4 cards if hand cards > 4
    printf("otherPlayer handCount = %d, expected = 4\n", stateT.handCount[otherPlayer]);
    printf("otherPlayer discardCount = %d, expected = 4\n", stateT.discardCount[otherPlayer]);
    assertM("otherPlayer redraw", stateT.handCount[otherPlayer] == 4 && stateT.discardCount[otherPlayer] == 4);

    printf("Test completed!\n");
    printf("--------------------------------------------------------------\n");
    return 0;
}
