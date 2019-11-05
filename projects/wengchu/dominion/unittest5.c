/**************************************************************************
 * Author:Chung Yu Weng
 * Test target: mine refactored function
 * ***********************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

#define TEST_FUNCTION "mineR"

int mineR(int choice1, int choice2, int currentPlayer, struct gameState *state, int handPos);

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
    int handPos = 0; 

    initializeGame(numPlayer,kCards, seed, &state);
     
    int currentPlayer = whoseTurn(&state);
    int choice1 = 0; //initialize choice1
    int choice2 = 0; //initialize choice2

    printf("Begin Testing %s():\n", TEST_FUNCTION);
    //---------------TEST 1-----------------------------------------------------
    printf("TEST 1 : choice1 = 1(set as silver), choice2 = gold => + gold card in hand, supply card for silver should not be changed because silver is trashed---------------\n"); 
    //set TEST 1 variables
    memcpy(&stateT, &state, sizeof(struct gameState));
    choice1 = 1;
    stateT.hand[currentPlayer][choice1] = silver;
    choice2 = gold; 
    int testSupplySilver = stateT.supplyCount[silver];
    int testBuy = stateT.numBuys; 
    int testAction = stateT.numActions;

    //call the refactored functions
    mineR(choice1, choice2, currentPlayer, &stateT, handPos);    

    //assert the results
    printf("currentlayer's hand cards are as following(1 means estate card; 4 means copper; 5 means silver; 6 means gold): ");
    int i;
    for(i=0; i < stateT.handCount[currentPlayer]; i++)
    {
         printf("(%d) ", stateT.hand[currentPlayer][i]);
    }
    printf("\n");

    printf("buys = %d, expected = %d\n", stateT.numBuys, testBuy);//test introduce bug 1
    assertM("unchanged buy", stateT.numBuys == testBuy);
    printf("--------------------\n");
    printf("actions = %d, expected = %d\n", stateT.numActions, testAction);//test introduce bug 2
    assertM("unchanged action", stateT.numActions == testAction);
    printf("--------------------\n");
    printf("ID of card gained in hand = %d, expected = %d\n", stateT.hand[currentPlayer][handPos], gold); //new gold card will be handPos because setup in the discardCard function
    assertM("+ gold card in hand", stateT.hand[currentPlayer][handPos] == gold);
    printf("--------------------\n");
    printf("supplyCount for silver = %d, expect = %d\n", stateT.supplyCount[silver], testSupplySilver);
    assertM("silver supplyCount", stateT.supplyCount[silver] == testSupplySilver);
    printf("-----------------------------------------\n");

    //---------------TEST 2-----------------------------------------------------
    printf("TEST 2 : choice1 = 1(set as silver), choice2 = province => no action and should not change anything because province is not a treasure card---------------\n"); 
    //set TEST 2 variables
    memcpy(&stateT, &state, sizeof(struct gameState));
    choice1 = 1;
    stateT.hand[currentPlayer][choice1] = silver;
    choice2 = province; 
    int testCardInHand = stateT.hand[currentPlayer][handPos];

    printf("Before mineR(), currentlayer's hand cards are as following(1 means estate card; 4 means copper; 5 means silver; 6 means gold; 3 means province): ");
    for(i=0; i < stateT.handCount[currentPlayer]; i++)
    {
         printf("(%d) ", stateT.hand[currentPlayer][i]);
    }

    //call the refactored functions
    mineR(choice1, choice2, currentPlayer, &stateT, handPos);    

    //assert the results
    printf("After mineR(), currentlayer's hand cards are as following(1 means estate card; 4 means copper; 5 means silver; 6 means gold; 3 means province): ");
    for(i=0; i < stateT.handCount[currentPlayer]; i++)
    {
         printf("(%d) ", stateT.hand[currentPlayer][i]);
    }
    printf("\n");
    printf("HandPos card ID in hand = %d, expected = %d\n", stateT.hand[currentPlayer][handPos], testCardInHand); //2 bug(>3 and range too large) (discussion) cause province card can be gained, shouldn't
    assertM("HandPos card in hand should not be changed", stateT.hand[currentPlayer][handPos] == testCardInHand);

    printf("Test completed!\n");
    printf("--------------------------------------------------------------\n");
    return 0;
}
