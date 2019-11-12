/**************************************************************************
 * Assignment 4
 * Author:Chung Yu Weng
 * class: CS362, Fall 2019
 * Test target: tribute refactored function
 * ***********************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>

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
    srand(time(NULL)); 

    //initialize common variables 
    int seed;
    int numPlayer;
    struct gameState state, stateT;
    int currentPlayer;
    int nextPlayer;
    int i;
    int bonus;

    int kCards[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
    int tributeRevealedCards[2] = {-1, -1};

    printf("**********************************************************************************************\n");
    printf("Begin Testing %s():\n", TEST_FUNCTION);

    int x;
    for (x = 0; x <= 200 ; x++)
    {
        //initailize each turn's variables
        seed = rand();
        numPlayer = 2 + rand() % 3;
        memset(&state,0,sizeof(struct gameState)); //clear state
        memset(&stateT,0,sizeof(struct gameState)); //clear stateT
    
        initializeGame(numPlayer,kCards, seed, &state);
        state.whoseTurn = rand() % numPlayer;
        currentPlayer = whoseTurn(&state);
        nextPlayer = currentPlayer + 1;

        for (i=0 ; i < numPlayer; i++)
        {
           state.deckCount[i] = rand() % 5;
           state.discardCount[i] = rand() % 5;
           state.handCount[i] = rand() % 5; //MAX_DECK is too large for this test, trying to use samller range for testing 
        }
        bonus = 0;

        //assign card to nextPlayer deck pile randomly
        for(i=0; i<state.deckCount[nextPlayer]; i++)
        {
            state.deck[nextPlayer][i] = rand() %  (treasure_map + 1); 
        } 

        //save initial state before function
        memcpy(&stateT, &state, sizeof(struct gameState)); 

        //call the refactored functions
        tributeR(currentPlayer, nextPlayer, tributeRevealedCards, &state, &bonus); 
        
        //assert the results
            //printf("%d\n", stateT.deckCount[nextPlayer]);
        if( stateT.deckCount[nextPlayer] >= 2
          && (adventurer <= stateT.deck[nextPlayer][stateT.deckCount[nextPlayer] - 1] && stateT.deck[nextPlayer][stateT.deckCount[nextPlayer] - 1] <= treasure_map) 
          && (adventurer <= stateT.deck[nextPlayer][stateT.deckCount[nextPlayer] - 2] && stateT.deck[nextPlayer][stateT.deckCount[nextPlayer] - 2] <= treasure_map) )
        {
            printf("TEST 1 : 2 nextPlayer top deck cards are same action cards(duplicate) => currentPlayer + 2 actions, nextPlayer -2 deck cards and +2 discard card---------------\n");
            printf("actions = %d, expected = %d\n", state.numActions, stateT.numActions + 2); //test introduce bug 1, duplicate card should +2 only, but here +4 because -- and i<=2  bug
            assertM("currentPlayer +2 action", state.numActions == stateT.numActions + 2);
            printf("buys = %d, expected = %d\n", state.numBuys, stateT.numBuys); //test introduce bug 2
            assertM("unchanged buys for current player", state.numBuys == stateT.numBuys);
            printf("nextPlayer deckCount = %d, expected = %d\n", state.deckCount[nextPlayer], stateT.deckCount[nextPlayer] - 2); // deckCount bug
            assertM("nextPlayer deckCount", state.deckCount[nextPlayer] == stateT.deckCount[nextPlayer] - 2);
            printf("nextPlayer discardCount = %d, expected = %d\n", state.discardCount[nextPlayer], stateT.discardCount[nextPlayer] + 2); //deckCount bug
            assertM("nextPlayer discardCount", state.discardCount[nextPlayer] == stateT.discardCount[nextPlayer] + 2);
            printf("----------------------------------------------------------------------------\n"); 
        }
        else if( stateT.deckCount[nextPlayer] >= 2
          && (((estate <= stateT.deck[nextPlayer][stateT.deckCount[nextPlayer] - 1] && stateT.deck[nextPlayer][stateT.deckCount[nextPlayer] - 1] <= province) 
          && (copper <= stateT.deck[nextPlayer][stateT.deckCount[nextPlayer] - 2] && stateT.deck[nextPlayer][stateT.deckCount[nextPlayer] - 2] <= gold)) 
          || ((estate <= stateT.deck[nextPlayer][stateT.deckCount[nextPlayer] - 2] && stateT.deck[nextPlayer][stateT.deckCount[nextPlayer] - 2] <= province) 
          && (copper <= stateT.deck[nextPlayer][stateT.deckCount[nextPlayer] - 1] && stateT.deck[nextPlayer][stateT.deckCount[nextPlayer] - 1] <= gold))) 
          )
        {
            printf("TEST 2 : 2 nextPlayer top deck cards are treasure and victory card => currentPlayer + 2 coins and + 2 hand cards, nextPlayer -2 deck cards and +2 discard card---------------\n"); 
            printf("coins = %d, expected = %d\n", state.coins, stateT.coins + 2); //updateCoin bug, should use bonus
            assertM("currentPlayer +2 coins", state.coins == stateT.coins + 2);
            printf("currentPlayer handCount = %d, expected = %d\n", state.handCount[currentPlayer], stateT.handCount[currentPlayer] + 2);
            assertM("currentPlayer handCount +2", state.handCount[currentPlayer] == stateT.handCount[currentPlayer] + 2);
            printf("actions = %d, expected = %d\n", state.numActions, stateT.numActions); //test introduce bug 1, duplicate card should +2 only, but here +4 because -- and i<=2  bug
            assertM("unchanged actions for currentPlayer", state.numActions == stateT.numActions); 
            printf("buys = %d, expected = %d\n", state.numBuys, stateT.numBuys); //test introduce bug 2
            assertM("unchanged buys for currentPlayer", state.numBuys == stateT.numBuys);
            printf("nextPlayer deckCount = %d, expected = %d\n", stateT.deckCount[nextPlayer], stateT.deckCount[nextPlayer] - 2); 
            assertM("nextPlayer deckCount", state.deckCount[nextPlayer] == stateT.deckCount[nextPlayer] - 2);
            printf("nextPlayer discardCount = %d, expected = %d\n", state.discardCount[nextPlayer], stateT.discardCount[nextPlayer] + 2); 
            assertM("nextPlayer discardCount", state.discardCount[nextPlayer] == stateT.discardCount[nextPlayer] + 2); 
            printf("-----------------------------------------------------------------------------\n"); 
        } 
        else if( stateT.deckCount[nextPlayer] == 0    //if nextPlayer has two actions cards on top deck after shuffle, then these cards will go to discard pile
          && (adventurer <= state.discard[nextPlayer][state.discardCount[nextPlayer] - 1] && state.discard[nextPlayer][state.discardCount[nextPlayer] - 1] <= treasure_map) 
          && (adventurer <= state.discard[nextPlayer][state.discardCount[nextPlayer] - 2] && state.discard[nextPlayer][state.discardCount[nextPlayer] - 2] <= treasure_map) )
        {
            printf("TEST 3 : nextPlayer deckCount = 0, all discard cards are same action cards(duplicate) => currentPlayer +4 actions ---------------\n");
            printf("actions = %d, expected = %d\n", state.numActions, stateT.numActions + 2); //also test introduce bug 1, add 3 times +2 actions because i<=2 bug
            assertM("currentPlayer +2 action", state.numActions == stateT.numActions + 2); //value changed, shuffle test success anyway
            printf("-----------------------------------------------------------------------------\n"); 
        }
    }
    
    printf("Test completed!\n");
    printf("**********************************************************************************************\n");

    return 0;
}
