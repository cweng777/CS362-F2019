/**************************************************************************
 * Author:Chung Yu Weng
 * Test target: baron refactored function
 * ***********************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

#define TEST_FUNCTION "handleBaron"

int handleBaron(int choice1, int currentPlayer, struct gameState *state);

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

    initializeGame(numPlayer,kCards, seed, &state);
     
    int currentPlayer = whoseTurn(&state);
    int choice1 = 0; //initialize choice1
    //int bonus = 0; //initialize bonus


    printf("Begin Testing %s():\n", TEST_FUNCTION);
    //---------------TEST 1a-----------------------------------------------------
    printf("TEST 1a : choice 1 = 0 => +1 buy, +1 Estate card to discard pile, -1 Estate card in supply pile---------------\n");
    //set TEST 1 variables
    memcpy(&stateT, &state, sizeof(struct gameState));
    int testBuy = stateT.numBuys;  //for later use of buy expected value
    int testSupplyE = stateT.supplyCount[estate];  //for later use of expected value of estate supplyCount
    choice1 = 0; //choose not to discard estate card
    //call the refactored functions
    handleBaron(currentPlayer, choice1, &stateT);   
 
    //assert the results
     //test +1 buy
    printf("buys = %d, expected = %d\n", stateT.numBuys, testBuy + 1);
    assertM("+1 buy", stateT.numBuys == testBuy + 1);
     //test +1 estate card
    printf("discard pile last card ID is = %d, expected = %d\n", stateT.discard[currentPlayer][stateT.discardCount[currentPlayer]-1], estate);
    assertM("+1 estate card to discard", stateT.discard[currentPlayer][stateT.discardCount[currentPlayer]-1] == estate);
     //test -1 estate supply card
    printf("estate supplyCount = %d, expected = %d\n", stateT.supplyCount[estate], testSupplyE - 1);
    assertM("-1 estate card in supply", stateT.supplyCount[estate] == testSupplyE - 1);
    printf("---------------------------------------------\n");
     

    //---------------TEST 1b(after find intro bug 1, adjust to test the rest of code)-----------------------------------------------------
    printf("TEST 1b(adjusted for introduce bug 1) : choice 1 = 1 => +1 buy, +1 Estate card to discard pile, -1 Estate card in supply pile---------------\n");
    //set TEST 1 variables
    memcpy(&stateT, &state, sizeof(struct gameState));
    testBuy = stateT.numBuys;  //for later use of buy expected value
    testSupplyE = stateT.supplyCount[estate];  //for later use of expected value of estate supplyCount
    choice1 = 1; //because introduce bug 1, adjust this to -1 to test the rest of code(document this)
    //call the refactored functions
    handleBaron(currentPlayer, choice1, &stateT);   
 
    //assert the results
     //test +1 buy
    printf("buys = %d, expected = %d\n", stateT.numBuys, testBuy + 1);
    assertM("+1 buy", stateT.numBuys == testBuy + 1);
     //test +1 estate card
    printf("discard pile last card ID is = %d, expected = %d\n", stateT.discard[currentPlayer][stateT.discardCount[currentPlayer]-1], estate);
    assertM("+1 estate card to discard", stateT.discard[currentPlayer][stateT.discardCount[currentPlayer]-1] == estate);
     //test -1 estate supply card
    printf("estate supplyCount = %d, expected = %d\n", stateT.supplyCount[estate], testSupplyE - 1);
    assertM("-1 estate card in supply", stateT.supplyCount[estate] == testSupplyE - 1);
    printf("---------------------------------------------\n");

    //---------------TEST 2-----------------------------------------------------
    printf("TEST 2 : choice 1 = -1(adjuct according to intro bug 1) => +1 buy, +4 coins, -1 Estate card from hand, +1 Estate card to discard pile------\n");
    //set TEST 2 variables
    memcpy(&stateT, &state, sizeof(struct gameState));
    testBuy = stateT.numBuys;
    int testCoin = stateT.coins;
    int p = 0; //used to count estate card from hand
    int testHandEBefore = 0;
    while(p < stateT.handCount[currentPlayer]) 
    {
        if(stateT.hand[currentPlayer][p] == estate)
            testHandEBefore++;
        p++;
    }
    choice1 = -1; //choose to discard estate card, adjuct according to intro bug 1, original one should be > 0
    //call the refactored functions
    handleBaron(currentPlayer, choice1, &stateT);   
    
    //assert the results
     //test +1 buy
    printf("buys = %d, expected = %d\n", stateT.numBuys, testBuy + 1);
    assertM("+1 buy", stateT.numBuys == testBuy + 1);
     //test +4 coins
    printf("coins = %d, expected = %d\n", stateT.coins, testCoin + 4);
    assertM("+4 coins", stateT.coins == testCoin + 4);
     //test -1 estate card from hand
    p = 0; //used to count estate card from hand
    int testHandEAfter = 0;
    while(p < stateT.handCount[currentPlayer]) 
    {
        if(stateT.hand[currentPlayer][p] == estate)
            testHandEAfter++;
        p++;
    }
    printf("estate card in hand = %d, expected = %d\n", testHandEAfter, testHandEBefore - 1);
    assertM("-1 Estate card from hand", testHandEAfter == testHandEBefore - 1);
     //test +1 Estate card to discard pile
    printf("discard pile last card ID is = %d, expected = %d\n", stateT.discard[currentPlayer][stateT.discardCount[currentPlayer]-1], estate);
    assertM("+1 Estate card to discard pile", stateT.discard[currentPlayer][stateT.discardCount[currentPlayer] - 1] == estate );
    printf("---------------------------------------------\n");


    //---------------TEST 3-----------------------------------------------------
    printf("TEST 3 : choice 1 = -1, but no estate card in hand => +1 buy, +1 Estate card to discard pile, -1 Estate card in supply pile---------------\n");
    //set TEST 3 variables
    memcpy(&stateT, &state, sizeof(struct gameState));
    testBuy = stateT.numBuys;  //for later buy ecpected value
    testSupplyE = stateT.supplyCount[estate]; 
    choice1 = -1; //choose not to discard estate card
    p = 0; //used to set hand card
    while(p < stateT.handCount[currentPlayer])  
    {
        stateT.hand[currentPlayer][p] = silver;
        p++;
    }
    //call the refactored functions
    handleBaron(currentPlayer, choice1, &stateT);   
    
    //assert the results
     //test +1 buy
    printf("buys = %d, expected = %d\n", stateT.numBuys, testBuy + 1);
    assertM("+1 buy", stateT.numBuys == testBuy + 1);
     //test +1 estate card
    printf("discard pile last card ID is = %d, expected = %d\n", stateT.discard[currentPlayer][stateT.discardCount[currentPlayer]-1], estate);
    assertM("+1 estate card to discard", stateT.discard[currentPlayer][stateT.discardCount[currentPlayer]-1] == estate);
     //test -1 estate supply card
    printf("estate supplyCount = %d, expected = %d\n", stateT.supplyCount[estate], testSupplyE - 1);
    assertM("-1 estate card in supply", stateT.supplyCount[estate] == testSupplyE - 1);
    
    printf("Test completed!\n");
    printf("--------------------------------------------------------------\n");

    return 0;
}

