#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /* 
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
     pBoard = new Board(); // player board
     //pBoard->Board::copy(); // set up the board
     timeTaken = 0;
     //initalize our side and the opponents side
     pSide = side;
     if (side == BLACK)
        oppSide = WHITE;
    else
        oppSide = BLACK;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /* 
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     *
    if (this->timeTaken >= msLeft && msLeft != -1)
    {
        cout << "Opponent Disqualified!" <<endl;
        return NULL;
    }
    */
    if (!pBoard->isDone())
    {
        pBoard->Board::doMove(opponentsMove, oppSide);

        //Find our best move, and implement our move onto our board
        Move *ourMove = bestMove(pSide);
        pBoard->Board::doMove(ourMove, pSide);
        return ourMove;
    }
    return NULL;
}

Move *Player::bestMove(Side side)
{
	//std::this_thread::sleep_for (std::chrono::seconds(1));
	
    /* The corners are the best possible moves to play. */
	int corners[] = {0, 0, 0, 7, 7, 0, 7, 7};
	/* The inner square (surrounds the starting square), is the safest
    * place to play if there are no good edges or corners available. */
	int innerSquare[] = {2, 3, 2, 4, 3, 2, 3, 4, 4, 2, 4, 4, 5, 3, 5, 4};
	/* The good edges are all the edges except the corners and the edge
	 * spots adjacent to the corners. These are the second best moves to
	 * be played, right after the corners. */
	int goodEdges[] = {0, 2, 0, 3, 0, 4, 0, 5,
						7, 2, 7, 3, 7, 4, 7, 5,
						2, 0, 3, 0, 4, 0, 4, 0,
						2, 7, 3, 7, 4, 7, 5, 7};
	/* These are the corners of the inner square. */
	int goodInnerCorners[] = {2, 2, 2, 4, 5, 2, 5, 4};
	
	/* The next best moves */
	int nextMoves2[] = {2, 1, 5, 1, 1, 2, 1, 5,
					   6, 2, 6, 5, 2, 6, 5, 6};
	/* The next best moves */
	int nextMoves1[] = {3, 1, 4, 1, 1, 3, 1, 4,
						6, 3, 6, 4, 3, 6, 4, 6};
	/* The next best moves */
	int nextEight[] = {0, 1, 1, 0, 6, 0, 1, 7,
					   6, 0, 7, 1, 6, 7, 7, 6};

    if (pBoard->hasMoves(pSide))
    {
		
		Move *moveC = new Move(corners[0], corners[1]);
		for (int i = 2; i < 8; i += 2)
		{
			Move *move = new Move(corners[i], corners[i + 1]);
			if (pBoard->Board::countChange(move, pSide) >
			    pBoard->Board::countChange(moveC, pSide))
			    moveC = move;
			//std::cerr << pBoard->Board::countChange(move, pSide) << std::endl;
		}
		if (pBoard->Board::checkMove(moveC, pSide))
		{
			std::cerr << "CORNER" << std::endl;
			return moveC;
		}
			
		Move *moveG = new Move(goodEdges[0], goodEdges[1]);
		for (int i = 2; i < 32; i += 2)
		{
			Move *move = new Move(corners[i], corners[i + 1]);
			if (pBoard->Board::countChange(move, pSide) >
				pBoard->Board::countChange(moveG, pSide))
				moveG = move;
		}
		if (pBoard->Board::checkMove(moveG, pSide))
			return moveG;	
			
		Move *moveI = new Move(goodInnerCorners[0], goodInnerCorners[1]);
		for (int i = 2; i < 8; i += 2)
		{
			Move *move = new Move(goodInnerCorners[i], goodInnerCorners[i + 1]);
			if (pBoard->Board::countChange(move, pSide) >
				pBoard->Board::countChange(moveI, pSide))
				moveI = move;
		}
		if (pBoard->Board::checkMove(moveI, pSide))
			return moveI;	

		Move *moveS = new Move(innerSquare[0], innerSquare[1]);
		for (int i = 2; i < 16; i += 2)
		{
			Move *move = new Move(innerSquare[i], innerSquare[i + 1]);
			if (pBoard->Board::countChange(move, pSide) >
				pBoard->Board::countChange(moveS, pSide))
				moveS = move;
		}
		if (pBoard->Board::checkMove(moveS, pSide))
			return moveS;			
			

			/*
		for (int i = 0; i < 8; i += 2)
		{
			Move *move = new Move(corners[i], corners[i + 1]);
			if (pBoard->Board::checkMove(move, pSide))
				return move;
		}	*/		
			
		Move *moveN = new Move(nextMoves1[0], nextMoves1[1]);
		for (int i = 2; i < 16; i += 2)
		{
			Move *move = new Move(nextMoves1[i], nextMoves1[i + 1]);
			if (pBoard->Board::countChange(move, pSide) >
				pBoard->Board::countChange(moveN, pSide))
				moveN = move;
		}
		if (pBoard->Board::checkMove(moveN, pSide))
			return moveN;
			
		Move *moveX = new Move(nextMoves2[0], nextMoves2[1]);
		for (int i = 2; i < 16; i += 2)
		{
			Move *move = new Move(nextMoves2[i], nextMoves2[i + 1]);
			if (pBoard->Board::countChange(move, pSide) >
				pBoard->Board::countChange(moveX, pSide))
				moveX = move;
		}
		if (pBoard->Board::checkMove(moveX, pSide))
			return moveX;
			
		Move *moveM = new Move(nextEight[0], nextEight[1]);
		for (int i = 2; i < 8; i += 2)
		{
			Move *move = new Move(nextEight[i], nextEight[i + 1]);
			if (pBoard->Board::countChange(move, pSide) >
				pBoard->Board::countChange(moveM, pSide))
				moveM = move;
		}
		if (pBoard->Board::checkMove(moveM, pSide))
			return moveM;

        return pBoard->Board::firstPossMove(pSide);
    }
    else
        return NULL;
}
