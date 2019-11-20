/**************************************************************************
 * Assignment 4
 * Author:Chung Yu Weng
 * class: CS362, Fall 2019
 * Test target: minion refactored function
 * ***********************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>

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
    srand(time(NULL)); 

    //initialize common variables 
    int seed;
    int numPlayer;
    struct gameState state, stateT;
    int currentPlayer;
    int handPos;
    int i;
    int choice1;
    int choice2;
    //int bonus;
    int testAction;  //for later expected action value
    int testCoin; //for later expected coins value
    //int otherPlayer;


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
        memset(&stateT,0,sizeof(struct gameState)); //clear stateT
    
        initializeGame(numPlayer,kCards, seed, &state);
        state.whoseTurn = rand() % numPlayer;
        currentPlayer = whoseTurn(&state);
        handPos = rand() % 5; 


        for (i=0 ; i < numPlayer; i++)
        {
           state.deckCount[i] = rand() % MAX_DECK;
           state.discardCount[i] = rand() % MAX_DECK;
           state.handCount[i] = rand() % MAX_HAND; 
        } 
        choice1 = rand() % 2;
        choice2 = rand() % 2;
        //bonus = 0;

        testAction = state.numActions;  //for later expected action value
        testCoin = state.coins; //for later expected coins value
        //otherPlayer = currentPlayer + 1; //compare with unit test(assume only 2 player)

        //save initial state before function
        memcpy(&stateT, &state, sizeof(struct gameState));

        //call the refactored functions
        handleMinion(currentPlayer, choice1, choice2, &state, handPos);

        //assert the results
        if(choice1 == 1 && choice2 == 0 )
        {
            printf("TEST 1 : choice 1 = 1, choice 2 = 0 => +1 action, +2 coins---------------\n");
            //test +1 action
            printf("actions = %d, expected = %d\n", state.numActions, testAction + 1); 
            assertM("+1 action", state.numActions == testAction + 1);  
            //test +2 coins
            printf("coins= %d, expected = %d\n", state.coins, testCoin + 2); 
            assertM("+2 coins", state.coins == testCoin + 2);
        }
        else if(choice1 == 0 && choice2 == 1)
        {
            printf("TEST 2 : choice 1 = 0, choice 2 = 1 => +1 action, currentPlayer discard and redraw 4 cards, other players redraw if hand cards > 4---------------\n"); 
            //test +1 action
            printf("actions = %d, expected = %d\n", state.numActions, testAction + 1); 
            assertM("+1 action", state.numActions == testAction + 1);
            //test currentPlayer discard and redraw 4 cards
            printf("currentPlayer handCount = %d, expected = 4\n", state.handCount[currentPlayer]); 
            printf("currentPlayer discardCount = %d, expected = 4\n", state.discardCount[currentPlayer]); 
            assertM("currentPlayer redraw", state.handCount[currentPlayer] == 4 && state.discardCount[currentPlayer] == 4);
            //test other players
            for (i=0; i < numPlayer; i++)
            {
                if(i != currentPlayer)
                {
                    if(stateT.handCount[i] >= 5)
                    {
                        printf("this ohter player has >= 5 hand cards\n");
                        printf("otherPlayer handCount = %d, expected = 4\n", state.handCount[i]);
                        printf("otherPlayer discardCount = %d, expected = %d\n", state.discardCount[i], stateT.discardCount[i] + stateT.handCount[i]);
                        assertM("otherPlayer handCount after redraw", state.handCount[i] == 4);
                        assertM("otherPlayer discardCount after redraw", state.discardCount[i] ==  stateT.discardCount[i] + stateT.handCount[i]);
                        
                    }
                    else
                    {
                        printf("this ohter player has less than 5 hand cards\n");
                        printf("otherPlayer handCount = %d, expected = %d\n", state.handCount[i], stateT.handCount[i]);
                        printf("otherPlayer discardCount = %d, expected = %d\n", state.discardCount[i], stateT.discardCount[i]);
                        assertM("otherPlayer handCount after redraw", state.handCount[i] == stateT.handCount[i]);
                        assertM("otherPlayer discardCount after redraw", state.discardCount[i] ==  stateT.discardCount[i]);
                       
                    }
                }
  
            }
        }

    }
    
    printf("Test completed!\n");
    printf("**********************************************************************************************\n");

    return 0;
}
