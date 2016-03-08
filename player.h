#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();
    
    Move *doMove(Move *opponentsMove, int msLeft);
    Move *bestMove(Side side);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
private:
	int timeTaken;
	Board *pBoard;
	Side pSide;
	Side oppSide;
	int corners[8];
	int goodInnerCorners[8];
	int innerSquare[16];
	int goodEdges[32];
	int nextMoves1[16];
	int nextMoves2[16];
	int nextEight[8];
};


#endif
