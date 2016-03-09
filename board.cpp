#include "board.h"

/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board() {
    taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board();
    newBoard->black = black;
    newBoard->taken = taken;
    return newBoard;
}

/* Returns true if the location (x, y) on the board is occupied (taken),
 * false otherwise. */
bool Board::occupied(int x, int y) {
    return taken[x + 8*y];
}

/* Returns true if the location (x, y) on the board is of color 'side'.
 * False otherwise (if location is unoccupied or if the location
 * occupied is not of color 'side'). */
bool Board::get(Side side, int x, int y) {
    return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
}

/* Set the location (x, y) to taken and if the side is black, set the
 * (x + 8y) position in the array black to true. Set it to false otherwise. */
void Board::set(Side side, int x, int y) {
    taken.set(x + 8*y);
    black.set(x + 8*y, side == BLACK);
}

/* Returns true if the location (x, y) exists on the board. False otherwise. */
bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}

 
/*
 * Returns true if the game is finished; false otherwise. The game is finished 
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(Side side) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) return true;
        }
    }
    return false;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, Side side) {
    // Passing is only legal if you have no moves.
    if (m == NULL) return !hasMoves(side);

    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));

                if (onBoard(x, y) && get(side, x, y)) return true;
            }
        }
    }
    return false;
}

/*
 * Gives the first possible move of all possible moves
 * and returns it. Assumes there are possible moves
 *
 * For initial basic AI testing.
 */
Move *Board::firstPossMove(Side side)
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move *move = new Move(i, j);
            if (checkMove(move, side)) return move;
        }
    }
    return NULL;
}

vector<Move*> Board::potentialMoves(Side side)
{
	vector<Move*> result;
	for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move *move = new Move(i, j);
            if (checkMove(move, side))
				result.push_back(move);
        }
    }
    
    return result;
}


/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, Side side) {
    // A NULL move means pass.
    if (m == NULL) return;

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return;

    int X = m->getX();
    int Y = m->getY();
    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    set(side, x, y);
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    set(side, X, Y);
}

/*
 * Returns the amount of stones that would change color
 */
int Board::countChange(Move *m, Side side) {
    // A NULL move means pass.
    if (m == NULL) return 0;
    int count = 0;

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return 0;

    int X = m->getX();
    int Y = m->getY();
    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    x += dx;
                    y += dy;
                    count++;
                }
            }
        }
    }
    return count;
}

Move *Board::bestMoveSpace(Side side)
{
	
	//std::this_thread::sleep_for (std::chrono::seconds(1));
	
    /* The corners are the best possible moves to play.
     * Score for corners is 100.*/
	int corners[] = {0, 0, 0, 7, 7, 0, 7, 7};

	/* The good edges are all the edges except the corners and the edge
	 * spots adjacent to the corners. These are the second best moves to
	 * be played, right after the corners. 
	 * Score for good edges is 50*/
	int goodEdges[] = {0, 2, 0, 3, 0, 4, 0, 5,
						7, 2, 7, 3, 7, 4, 7, 5,
						2, 0, 3, 0, 4, 0, 4, 0,
						2, 7, 3, 7, 4, 7, 5, 7};
						
	/* The inner square (surrounds the starting square), is the safest
    * place to play if there are no good edges or corners available. 
    * Score for inner square is 25*/
	int innerSquare[] = {2, 3, 2, 4, 3, 2, 3, 4, 4, 2, 4, 4, 5, 3, 5, 4};
	
	/* These are the corners of the inner square. 
	 * Score for good inner corners is 15*/
	int goodInnerCorners[] = {2, 2, 2, 4, 5, 2, 5, 4};
	
	/* The next best moves Score is 5*/
	int nextMoves1[] = {3, 1, 4, 1, 1, 3, 1, 4,
						6, 3, 6, 4, 3, 6, 4, 6};
	
	/* The next best moves. Score is 1*/
	int nextMoves2[] = {2, 1, 5, 1, 1, 2, 1, 5,
					   6, 2, 6, 5, 2, 6, 5, 6};

	/* The next best moves. Score is -5*/
	int nextEight[] = {0, 1, 1, 0, 6, 0, 1, 7,
					   6, 0, 7, 1, 6, 7, 7, 6};

	/* The last 8 moves are the worst moves possible. They will have
	 * scores of -25 */
	
    if (hasMoves(side))
    {
		
		Move *moveC = new Move(corners[0], corners[1]);
		for (int i = 2; i < 8; i += 2)
		{
			Move *move = new Move(corners[i], corners[i + 1]);
			if (countChange(move, side) >
			    countChange(moveC, side))
			    moveC = move;
			//std::cerr << pBoard->Board::countChange(move, pSide) << std::endl;
		}
		if (checkMove(moveC, side))
		{
			std::cerr << "1::CORNER" << std::endl;
			return moveC;
		}
			
		Move *moveG = new Move(goodEdges[0], goodEdges[1]);
		for (int i = 2; i < 32; i += 2)
		{
			Move *move = new Move(corners[i], corners[i + 1]);
			if (countChange(move, side) >
				countChange(moveG, side))
				moveG = move;
		}
		if (checkMove(moveG, side))
		{
			std::cerr << "2::GOOD EDGE" << std::endl;
			return moveG;	
		}
			
		Move *moveI = new Move(goodInnerCorners[0], goodInnerCorners[1]);
		for (int i = 2; i < 8; i += 2)
		{
			Move *move = new Move(goodInnerCorners[i], goodInnerCorners[i + 1]);
			if (countChange(move, side) >
				countChange(moveI, side))
				moveI = move;
		}
		if (checkMove(moveI, side))
		{
			std::cerr << "3::GOOD INNER CORNER" << std::endl;
			return moveI;	
		}

		Move *moveS = new Move(innerSquare[0], innerSquare[1]);
		for (int i = 2; i < 16; i += 2)
		{
			Move *move = new Move(innerSquare[i], innerSquare[i + 1]);
			if (countChange(move, side) >
				countChange(moveS, side))
				moveS = move;
		}
		if (checkMove(moveS, side))
		{
			std::cerr << "4::INNER SQUARE" << std::endl;
			return moveS;			
		}	
			
		Move *moveN = new Move(nextMoves1[0], nextMoves1[1]);
		for (int i = 2; i < 16; i += 2)
		{
			Move *move = new Move(nextMoves1[i], nextMoves1[i + 1]);
			if (countChange(move, side) >
				countChange(moveN, side))
				moveN = move;
		}
		if (checkMove(moveN, side))
		{
			std::cerr << "5::NEXT MOVES 1" << std::endl;
			return moveN;
		}
			
		Move *moveX = new Move(nextMoves2[0], nextMoves2[1]);
		for (int i = 2; i < 16; i += 2)
		{
			Move *move = new Move(nextMoves2[i], nextMoves2[i + 1]);
			if (countChange(move, side) >
				countChange(moveX, side))
				moveX = move;
		}
		if (checkMove(moveX, side))
		{
			std::cerr << "6::NEXT MOVES 2" << std::endl;
			return moveX;
		}
			
		Move *moveM = new Move(nextEight[0], nextEight[1]);
		for (int i = 2; i < 8; i += 2)
		{
			Move *move = new Move(nextEight[i], nextEight[i + 1]);
			if (countChange(move, side) >
				countChange(moveM, side))
				moveM = move;
		}
		if (checkMove(moveM, side))
		{
			std::cerr << "7::NEXT EIGHT" << std::endl;
			return moveM;
		}
		
		std::cerr << "8::WORST MOVE" << std::endl;

        return firstPossMove(side);
    }
    else
        return NULL;
}

int Board::scoreSpace(Side side)
{
	int corners[] = {0, 0, 0, 7, 7, 0, 7, 7};
	int goodEdges[] = {0, 2, 0, 3, 0, 4, 0, 5,
						7, 2, 7, 3, 7, 4, 7, 5,
						2, 0, 3, 0, 4, 0, 4, 0,
						2, 7, 3, 7, 4, 7, 5, 7};

	int innerSquare[] = {2, 3, 2, 4, 3, 2, 3, 4, 4, 2, 4, 4, 5, 3, 5, 4};

	int goodInnerCorners[] = {2, 2, 2, 4, 5, 2, 5, 4};

	int nextMoves1[] = {3, 1, 4, 1, 1, 3, 1, 4,
						6, 3, 6, 4, 3, 6, 4, 6};

	int nextMoves2[] = {2, 1, 5, 1, 1, 2, 1, 5,
					   6, 2, 6, 5, 2, 6, 5, 6};

	int nextEight[] = {0, 1, 1, 0, 6, 0, 1, 7,
					   6, 0, 7, 1, 6, 7, 7, 6};

	int score = 0;
	bool foundScore = false;
	
	for (int r = 0; r < 8; r++)
	{
		for (int c = 0; c < 8; c++)
		{
			if (get(side, r, c))
			{
				for (int i = 0; i < 8; i += 2)
				{
					if (r == corners[i] && c == corners[i + 1])
					{
						score += 100;
						foundScore = true;
					}
				}
				if (foundScore == false)
				{
					for (int i = 0; i < 32; i += 2)
					{
						if (r == goodEdges[i] && c == corners[i + 1])
						{
							score += 50;
							foundScore = true;
						}
					}
				}
				if (foundScore == false)
				{
					for (int i = 0; i < 16; i += 2)
					{
						if (r == innerSquare[i] && c == innerSquare[i + 1])
						{
							score += 25;
							foundScore = true;
						}
					}
				}
				if (foundScore == false)
				{
					for (int i = 0; i < 8; i += 2)
					{
						if (r == goodInnerCorners[i] && c == goodInnerCorners[i + 1])
						{
							score += 15;
							foundScore = true;
						}
					}
				}	
				if (foundScore == false)
				{
					for (int i = 0; i < 16; i += 2)
					{
						if (r == nextMoves1[i] && c == nextMoves1[i + 1])
						{
							score += 5;
							foundScore = true;
						}
					}
				}
				if (foundScore == false)
				{
					for (int i = 0; i < 16; i += 2)
					{
						if (r == nextMoves2[i] && c == nextMoves2[i + 1])
						{
							score += 1;
							foundScore = true;
						}
					}
				}
				if (foundScore == false)
				{
					for (int i = 0; i < 16; i += 2)
					{
						if (r == nextEight[i] && c == nextEight[i + 1])
						{
							score -= 5;
							foundScore = true;
						}
					}
				}
				else
				{
					score -= 25;
				}		
			}
		}
	}
	return score;
    
}

/*
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/*
 * Current count of black stones.
 */
int Board::countBlack() {
    return black.count();
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    return taken.count() - black.count();
}


/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    taken.reset();
    black.reset();
    for (int i = 0; i < 64; i++) {
        if (data[i] == 'b') {
            taken.set(i);
            black.set(i);
        } if (data[i] == 'w') {
            taken.set(i);
        }
    }
}
