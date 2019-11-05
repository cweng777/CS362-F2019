/**************************************************************************
 * Author:Chung Yu Weng
 * Test target: tribute refactored function
 * ***********************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

#define TEST_FUNCTION "tributeR"

int tributeR(int currentPlayer, int nextPlayer, int tributeRevealedCards[], struct gameState *state, int *bonus);

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
    int nextPlayer = currentPlayer + 1;
    int bonus = 0; //initialize bonus
    int tributeRevealedCards[2] = {-1, -1};


    printf("Begin Testing %s():\n", TEST_FUNCTION);
    //---------------TEST 1----------------------------------------------------- 
    printf("TEST 1 : 2 nextPlayer top deck cards are same action cards(duplicate) => currentPlayer + 2 actions, nextPlayer -2 deck cards and +2 discard card---------------\n");
    //set TEST 1 variables
    memcpy(&stateT, &state, sizeof(struct gameState));
    stateT.deck[nextPlayer][stateT.deckCount[nextPlayer] - 1] = ambassador;
    stateT.deck[nextPlayer][stateT.deckCount[nextPlayer] - 2] = ambassador;
    int testAction = stateT.numActions;
    printf("nextplayer's deck cards are as following(20 means ammassador card;o means curse card; 1 means estate card; 4 means copper): ");
    int i;
    for(i=0; i < stateT.deckCount[nextPlayer]; i++)
    {
         printf("(%d) ", stateT.deck[nextPlayer][i]);
    }
    printf("\n");
    //set up expected value variables
    int testBuy = stateT.numBuys;
    int testNextPDeck = stateT.deckCount[nextPlayer];
    int testNextPDiscard = stateT.discardCount[nextPlayer];

    //call the refactored functions
    tributeR(currentPlayer, nextPlayer, tributeRevealedCards, &stateT, &bonus);

    //assert the results
    printf("actions = %d, expected = %d\n", stateT.numActions, testAction + 2); //test introduce bug 1, duplicate card should +2 only, but here +4 because -- and i<=2  bug
    assertM("currentPlayer +2 action", stateT.numActions == testAction + 2);
    printf("-------------------------\n");
    printf("buys = %d, expected = %d\n", stateT.numBuys, testBuy); //test introduce bug 2
    assertM("unchanged buys for current player", stateT.numBuys == testBuy);
    printf("-------------------------\n");
    printf("nextPlayer deckCount = %d, expected = %d\n", stateT.deckCount[nextPlayer], testNextPDeck - 2); // deckCount bug
    assertM("nextPlayer deckCount", stateT.deckCount[nextPlayer] == testNextPDeck - 2);
    printf("-------------------------\n");
    printf("nextPlayer discardCount = %d, expected = %d\n", stateT.discardCount[nextPlayer], testNextPDiscard + 2); //deckCount bug
    assertM("nextPlayer discardCount", stateT.discardCount[nextPlayer] == testNextPDiscard + 2);
    printf("-------------------------------------------\n");
 
    //---------------TEST 2-----------------------------------------------------
    //test money, get card, once each
    printf("TEST 2 : 2 nextPlayer top deck cards are treasure and victory card => currentPlayer + 2 coins and + 2 hand cards, nextPlayer -2 deck cards and +2 discard card---------------\n");
    //set TEST 2 variables
    memcpy(&stateT, &state, sizeof(struct gameState));
    stateT.deckCount[nextPlayer] = 5;
    stateT.deck[nextPlayer][0] = ambassador;
    stateT.deck[nextPlayer][1] = ambassador;
    stateT.deck[nextPlayer][2] = ambassador;
    stateT.deck[nextPlayer][3] = copper;
    stateT.deck[nextPlayer][4] = estate;
    testAction = stateT.numActions;
    int testCoin = stateT.coins;
    int testHandCount = stateT.handCount[currentPlayer];
    printf("nextplayer's deck cards are as following(20 means ammassador card;o means curse card; 1 means estate card; 4 means copper): ");
    
    for(i=0; i < stateT.deckCount[nextPlayer]; i++)
    {
         printf("(%d) ", stateT.deck[nextPlayer][i]);
    }
    printf("\n");
    //set up expected value variables
    testBuy = stateT.numBuys;
    testNextPDeck = stateT.deckCount[nextPlayer];
    testNextPDiscard = stateT.discardCount[nextPlayer]; 

    //call the refactored functions
    tributeR(currentPlayer, nextPlayer, tributeRevealedCards, &stateT, &bonus);

    //assert the results
    printf("coins = %d, expected = %d\n", stateT.coins, testCoin + 2); //updateCoin bug, should use bonus
    assertM("currentPlayer +2 coins", stateT.coins == testCoin + 2);
    printf("----------------------\n");
    printf("currentPlayer handCount = %d, expected = %d\n", stateT.handCount[currentPlayer], testHandCount + 2);
    assertM("currentPlayer handCount +2", stateT.handCount[currentPlayer] == testHandCount + 2);
    printf("----------------------\n");
    printf("actions = %d, expected = %d\n", stateT.numActions, testAction); //test introduce bug 1, duplicate card should +2 only, but here +4 because -- and i<=2  bug
    assertM("unchanged actions for currentPlayer", stateT.numActions == testAction); 
    printf("----------------------\n");
    printf("buys = %d, expected = %d\n", stateT.numBuys, testBuy); //test introduce bug 2
    assertM("unchanged buys for currentPlayer", stateT.numBuys == testBuy);
    printf("----------------------\n");
    printf("nextPlayer deckCount = %d, expected = %d\n", stateT.deckCount[nextPlayer], testNextPDeck - 2); 
    assertM("nextPlayer deckCount", stateT.deckCount[nextPlayer] == testNextPDeck - 2);
    printf("----------------------\n");
    printf("nextPlayer discardCount = %d, expected = %d\n", stateT.discardCount[nextPlayer], testNextPDiscard + 2); 
    assertM("nextPlayer discardCount", stateT.discardCount[nextPlayer] == testNextPDiscard + 2);
    printf("-------------------------------------------\n");


    //---------------TEST 3-----------------------------------------------------  
    printf("TEST 3 : nextPlayer deckCount = 0, all discard cards are same action cards(duplicate) => currentPlayer +4 actions ---------------\n"); //test if deck no card, it will shuffle to get card form discard pile
    //set TEST 3 variables
    memcpy(&stateT, &state, sizeof(struct gameState));
    stateT.deckCount[nextPlayer] = 0; 
    stateT.discardCount[nextPlayer] = 5;
    for (i=0; i<5; i++)
    {
        stateT.discard[nextPlayer][i] = ambassador;
    }

    testAction = stateT.numActions;

    //call the refactored functions
    tributeR(currentPlayer, nextPlayer, tributeRevealedCards, &stateT, &bonus);

    //assert the results
    printf("actions = %d, expected = %d\n", stateT.numActions, testAction + 2); //also test introduce bug 1, add 3 times +2 actions because i<=2 bug
    assertM("currentPlayer +2 action", stateT.numActions == testAction + 2); //value changed, shuffle test success anyway

    printf("Test completed!\n");
    printf("--------------------------------------------------------------\n");
    return 0;
}
