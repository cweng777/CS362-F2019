/**************************************************************************
 * Assignment 4
 * Author:Chung Yu Weng
 * class: CS362, Fall 2019
 * Test target: baron refactored function
 * ***********************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define TEST_FUNCTION "baronR"

int baronR(int choice1, int currentPlayer, struct gameState *state, int *bonus);

void assertM(char testName[],int result)
{
    if(result)
        printf("Testing target: \"%s\": PASS!\n", testName);
    else
        printf("Testing target: \"%s\": FAIL!\n", testName);
}

int main() 
{
    srand(time(NULL));

    //initialize common variables
    int seed;
    int numPlayer;
    struct gameState state;
    int currentPlayer;
    int i, p;
    int choice1;
    int bonus;
    int testBuy; //for later use of buy expected value
    int testSupplyE; //for later use of expected value of estate supplyCount
    int testCoin;
    int testHandEBefore = 0;
    int testHandEAfter = 0;

    int kCards[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};

    printf("**********************************************************************************************\n");
    printf("Begin Testing %s():\n", TEST_FUNCTION);

    int x;
    for (x = 0; x <= 50; x++)
    {
        //initailize each turn's variables
        seed = rand();
        numPlayer = 2 + rand() % 3;
        memset(&state,0,sizeof(struct gameState)); //clear state
    
        initializeGame(numPlayer,kCards, seed, &state);
        state.whoseTurn = rand() % numPlayer;
        currentPlayer = whoseTurn(&state);

        for (i=0 ; i < numPlayer; i++)
        {
           state.deckCount[i] = rand() % MAX_DECK;
           state.discardCount[i] = rand() % MAX_DECK;
           state.handCount[i] = rand() % MAX_HAND; 
        } 
        choice1 = rand() % 2;
        bonus = 0;

        testBuy = state.numBuys; 
        testSupplyE = state.supplyCount[estate];
        testCoin = state.coins;

        //print out hand cards
        /*                       //MAX_HAND has bug
        for(i=0; i < state.handCount[currentPlayer]; i++)
        {
            printf("(%d) ", state.hand[currentPlayer][i]);
        }
        printf("\n");
        */

        //used to count estate card from hand(before baronR())
        p = 0; 
        testHandEBefore = 0;
        while(p < state.handCount[currentPlayer]) 
        {
            if(state.hand[currentPlayer][p] == estate)
                testHandEBefore++;
            p++;
        }

        //call the refactored functions
        baronR(choice1, currentPlayer, &state, &bonus);   

        //used to count estate card from hand(after baronR())
        p = 0; //used to count estate card from hand
        testHandEAfter = 0;
        while(p < state.handCount[currentPlayer]) 
        {
            if(state.hand[currentPlayer][p] == estate)
                testHandEAfter++;
            p++;
        }


        //assert the results
        if(choice1 == 0)
        {
            printf("TEST 1 : choice 1 = 0 => +1 buy, +1 Estate card to discard pile, -1 Estate card in supply pile---------------\n");
            //test +1 buy
            printf("buys = %d, expected = %d\n", state.numBuys, testBuy + 1);
            assertM("+1 buy", state.numBuys == testBuy + 1);
            //test +1 estate card
            printf("discard pile last card ID is = %d, expected = %d\n", state.discard[currentPlayer][state.discardCount[currentPlayer]-1], estate);
            assertM("+1 estate card to discard", state.discard[currentPlayer][state.discardCount[currentPlayer]-1] == estate);
            //test -1 estate supply card
            printf("estate supplyCount = %d, expected = %d\n", state.supplyCount[estate], testSupplyE - 1);
            assertM("-1 estate card in supply", state.supplyCount[estate] == testSupplyE - 1);
        }
        else if (choice1 == 1 && testHandEBefore > 0)
        {
            printf("TEST 2 : choice 1 = 1 => +1 buy, +4 coins, -1 Estate card from hand, +1 Estate card to discard pile------\n");
            //test +1 buy
            printf("buys = %d, expected = %d\n", state.numBuys, testBuy + 1);
            assertM("+1 buy", state.numBuys == testBuy + 1);
            //test +4 coins
            printf("coins = %d, expected = %d\n", state.coins, testCoin + 4);
            assertM("+4 coins", state.coins == testCoin + 4);
            //test -1 estate card from hand
            printf("estate card in hand = %d, expected = %d\n", testHandEAfter, testHandEBefore - 1);
            assertM("-1 Estate card from hand", testHandEAfter == testHandEBefore - 1);
            //test +1 Estate card to discard pile
            printf("discard pile last card ID is = %d, expected = %d\n", state.discard[currentPlayer][state.discardCount[currentPlayer]-1], estate);
            assertM("+1 Estate card to discard pile", state.discard[currentPlayer][state.discardCount[currentPlayer] - 1] == estate );
        
        } else if (choice1 == 1 && testHandEBefore == 0)
        {
            printf("TEST 3 : choice 1 = 1, but no estate card in hand => +1 buy, +1 Estate card to discard pile, -1 Estate card in supply pile--------\n");
            //test +1 buy
            printf("buys = %d, expected = %d\n", state.numBuys, testBuy + 1);
            assertM("+1 buy", state.numBuys == testBuy + 1);
            //test +1 estate card
            printf("discard pile last card ID is = %d, expected = %d\n", state.discard[currentPlayer][state.discardCount[currentPlayer]-1], estate);
            assertM("+1 estate card to discard", state.discard[currentPlayer][state.discardCount[currentPlayer]-1] == estate);
            //test -1 estate supply card
            printf("estate supplyCount = %d, expected = %d\n", state.supplyCount[estate], testSupplyE - 1);
            assertM("-1 estate card in supply", state.supplyCount[estate] == testSupplyE - 1);
        } 
    
    }
    
    printf("Test completed!\n");
    printf("**********************************************************************************************\n");

    return 0;
}
