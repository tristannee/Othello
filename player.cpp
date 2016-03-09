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
 * Sets current board for the player
 * to a given board.
 */
void Player::setBoard(Board *b)
{
	pBoard = b;
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
        Move *ourMove;
        //NON MINIMAX ALGORITHM - just based on best move space heuristic
        if (!testingMinimax)
			ourMove = pBoard->Board::bestMoveSpace(pSide);
        else
			ourMove = bestMoveMinimax(pSide, oppSide);
        //MINIMAX ALGORITHM
        
        pBoard->Board::doMove(ourMove, pSide);
        return ourMove;
    }
    return NULL;
}


/*
 * Uses minimax algorithm to find our best current move
 * 
 * Current depth: 2 ply
 */
Move *Player::bestMoveMinimax(Side ourside, Side theirside)
{	
	if (pBoard->hasMoves(ourside))
	{
		//initialize moves list, sets our current best move
		//to the first move in that list, and also initializes
		//a board to be used temporarily in each check of the
		//opponents move and resulting score
		vector<Move*> moves = pBoard->Board::potentialMoves(ourside);
		Move *bestMove = moves[0]; //our best move
		Move *tempMove;
		int tempScore;

		//calculate initial score for the first move
		Board *tempBoard = pBoard->Board::copy();
		tempBoard->Board::doMove(bestMove, ourside);
		
		tempMove = tempBoard->Board::bestMoveSpace(theirside);
		//tempMove = tempBoard->Board::bestMoveCount(theirside);
		
		tempBoard->Board::doMove(tempMove, theirside);
		int ourCount = tempBoard->Board::count(ourside);
		int theirCount = tempBoard->Board::count(theirside);
		//int ourSpace = tempBoard->Board::scoreSpace(ourside);
		//int theirSpace = tempBoard->Board::scoreSpace(theirside);
		int bestScore = ourCount - theirCount;
		//int bestScore = ourSpace - theirSpace;
		//std::cerr << "Our Space Count :: " << ourSpace << "---";
		//std::cerr << "Their Space Count :: " << theirSpace << endl;
			
		if (moves.size() > 1)
		{
			for (unsigned int i = 1; i < moves.size(); i++)
			{
				tempBoard = pBoard->Board::copy();
				
				//do one of our moves
				tempBoard->Board::doMove(moves[i], ourside);
				
				
				if (tempBoard->hasMoves(theirside))
				{
					tempMove = tempBoard->Board::bestMoveSpace(theirside);
					//tempMove = tempBoard->Board::bestMoveCount(theirside);
					
					//do their best move based on our current move
					tempBoard->Board::doMove(tempMove, theirside);
					
					//calculate score based on differences of stone numbers
					//tempScore = tempBoard->Board::count(ourside) - tempBoard->Board::count(theirside);
					
					//or calculate score based on specialized spaces algorithm
					tempScore = tempBoard->Board::scoreSpace(ourside) - tempBoard->Board::scoreSpace(theirside);
					
					if (tempScore > bestScore)
						bestMove = moves[i];
				}
			}
		}
		return bestMove;
	}
	return NULL;
}


