/**************************************************************************
 * Author:Chung Yu Weng
 * Test target: ambassador refactored function
 * ***********************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

#define TEST_FUNCTION "handleAmbassador"

int handleAmbassador(int currentPlayer, int choice1, int choice2, struct gameState *state, int handPos);

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
    printf("TEST 1 : choice 1 = 1, choice 2 = 2 => currentPlayer -3 cards(1 ambassador, 2 revealed cards) from hand, otherPlayer +1 revealed card to discard---------------\n"); 
    //set TEST 1 variables
    memcpy(&stateT, &state, sizeof(struct gameState));
    stateT.handCount[currentPlayer] = 5; //set this run as the first run of the game, so have 5 cards
    
     //set currentplayer hand cards
    stateT.hand[currentPlayer][0] = ambassador;
    stateT.hand[currentPlayer][1] = curse;
    stateT.hand[currentPlayer][2] = curse;
    stateT.hand[currentPlayer][3] = ambassador;
    stateT.hand[currentPlayer][4] = silver;
    printf("Initial condition: currentPlayer has 2 curse cards he wants to return to supply\n");
    printf("currentplayer's hand cards are as following(20 means ammassador card;o means curse card; 5 means silver card): ");
    int i =0;
    for(i=0; i < stateT.handCount[currentPlayer]; i++)
    {
         printf("(%d) ", stateT.hand[currentPlayer][i]);
    }
    printf("\n");

    choice1 = 1; //currentPlayer want to return this position card
    choice2 = 1; //1 copy to be returned to supply
    handPos = 3;  //to avoid of bug
    int otherPlayer = currentPlayer + 1; //assume two players
    int testDiscardOP = stateT.discardCount[otherPlayer]; //the amount of otherPlayer's discard pile (for later expected value use)
    int testRevealed = stateT.hand[currentPlayer][choice1]; //the revealed card ID (for later expected value use)
    int testSupplyCount = stateT.supplyCount[testRevealed]; //supplyCount for revealed card (for later expected value use)

    //call the refactored functions
    handleAmbassador(currentPlayer, choice1, choice2, &stateT, handPos);

    //assert the results
    printf("currentPlayer handCount = %d, expected = %d\n", stateT.handCount[currentPlayer], 3); //one ambassador to discard, 1 curse card back to supply, handCount - 2
    assertM("currentPlayer handCount - 1", stateT.handCount[currentPlayer] == 3);
    printf("------------------ \n");
    printf("otherPlayer discardCount = %d, expected = %d\n", stateT.discardCount[otherPlayer], testDiscardOP +1);
    assertM("otherPlayer discardCount +1 ", stateT.discardCount[otherPlayer] == testDiscardOP + 1);
    printf("------------------ \n");
    printf("otherPlayer card gained  = %d, expected = %d\n", stateT.discard[otherPlayer][stateT.discardCount[otherPlayer] -1], testRevealed);
    assertM("otherPlayer gains the right revealed card", stateT.discard[otherPlayer][stateT.discardCount[otherPlayer] -1] == testRevealed);  //could be passed because of initial value = 0 for curse card
    printf("------------------ \n");
    printf("supplyCount for revealed card = %d, expected = %d\n", stateT.supplyCount[testRevealed], testSupplyCount + 1);  //supplyCount for revealed card: +2 returned card from currentPlayer, -1 to the otherPlayer  
    assertM("supplyCount for revealed card", stateT.supplyCount[testRevealed] == testSupplyCount + 1);

    printf("--------------------------------------------\n");


    //---------------TEST 2-----------------------------------------------------  //introduce bug 1 should make it work again, try to bypass intrduced bug 1 to test the rest of code
    printf("TEST 2 : choice 1 = 1, choice 2 = 1, handpos = 1 => all value should be same becuase choice1 == handpos, no changed should be made---------------\n"); 
    //set TEST 1 variables
    memcpy(&stateT, &state, sizeof(struct gameState));
    handPos = 1;  //impossible value because handPos must be ambassador card, i am trying to bypass introduce bug and test the rest half of code
    stateT.handCount[currentPlayer] = 5; //set this run as the first run of the game(discuss)
    
     //set currentplayer hand cards
    stateT.hand[currentPlayer][0] = ambassador;
    stateT.hand[currentPlayer][1] = copper;  //because of i == state->hand[currentPlayer][choice1] bug, here can not choose card id that is greater than 5 
    stateT.hand[currentPlayer][2] = curse;
    stateT.hand[currentPlayer][3] = copper;
    stateT.hand[currentPlayer][4] = silver;
    printf("Initial condition: currentPlayer has 2 curse cards he wants to return to supply\n");
    printf("currentplayer's hand cards are as following(20 means ammassador card;o means curse card; 5 means silver card): ");
    for(i=0; i < stateT.handCount[currentPlayer]; i++)
    {
         printf("(%d) ", stateT.hand[currentPlayer][i]);
    }
    printf("\n");

    choice1 = 1; //currentPlayer want to return this position card
    choice2 = 1; //2 copies to be returned to supply
    otherPlayer = currentPlayer + 1; //assume two players
    testDiscardOP = stateT.discardCount[otherPlayer]; //the amount of otherPlayer's discard pile 
    int testLastCard = stateT.discard[otherPlayer][stateT.discardCount[otherPlayer] -1];
    testSupplyCount = stateT.supplyCount[testRevealed]; //supplyCount for revealed card;

    //call the refactored functions
    handleAmbassador(currentPlayer, choice1, choice2, &stateT, handPos);
    //assert the results
    printf("currentPlayer handCount = %d, expected = %d\n", stateT.handCount[currentPlayer], 5); //it work by reduce two cards(1 from copy; 1 form hand card)
    assertM("currentPlayer handCount not changed", stateT.handCount[currentPlayer] == 5); 
    printf("------------------ \n");
    printf("otherPlayer discardCount = %d, expected = %d\n", stateT.discardCount[otherPlayer], testDiscardOP); //result: gain one discardcard
    assertM("otherPlayer discardCount not changed", stateT.discardCount[otherPlayer] == testDiscardOP);
    printf("------------------ \n");
    printf("otherPlayer card gained  = %d, expected = %d\n", stateT.discard[otherPlayer][stateT.discardCount[otherPlayer] -1], testLastCard); //try to see the ohter player's gained card
    assertM("otherPlayer lastCard should not be changed", stateT.discard[otherPlayer][stateT.discardCount[otherPlayer] -1] == testLastCard); 
    printf("------------------ \n");
    printf("supplyCount for revealed card = %d, expected = %d\n", stateT.supplyCount[testRevealed], testSupplyCount);  //should not be changed  
    assertM("supplyCount for revealed card", stateT.supplyCount[testRevealed] == testSupplyCount);

    printf("Test completed!\n");
    printf("--------------------------------------------------------------\n");
    return 0;
}
