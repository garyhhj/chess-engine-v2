#include "board.h"

/********************
*
*Utils
*
*********************/

int Board::minMax(BoardState& boardState, int depth) {
	Board::IminMax(boardState, depth); 
}
int Board::IminMax(BoardState& boardState, int depth) {
	//base case 
	if (depth == 0) {
		//evaluate and return some number 
		Board::evaluate(boardState); 
	}
	
	//max 
	if (boardState.side == white) {
		Movelist ml; 
		ml.moveGen(*this, boardState); 

		Board currBoard = *this; 
		BoardState currBoardState = boardState; 

		int maxEval = -50000;
		for (int i = 0, const int index = ml.getIndex(); i < index; ++i) {
			*this = currBoard; 
			boardState = currBoardState; 

			//search 
			Board::makemove(ml.getMove(i), boardState); 
			maxEval = std::max(Board::minMax(boardState, depth - 1), maxEval); 
		}
		return maxEval; 
	}

	//min
	else {
		Movelist ml; 
		ml.moveGen(*this, boardState); 

		Board currBoard = *this; 
		BoardState currBoardState = boardState; 
		
		int maxEval = 50000;
		for (int i = 0, const int index = ml.getIndex(); i < index; ++i) {
			*this = currBoard; 
			boardState = currBoardState;

			//search 
			Board::makemove(ml.getMove(i), boardState);
			maxEval = std::min(Board::minMax(boardState, depth - 1), maxEval); 
		}
		return maxEval; 
	}
}

int Board::evaluate(const BoardState& boardState) const {
	return Board::Ievaluate(boardState); 
}
int Board::Ievaluate(const BoardState& boardState) const {
	//material score 
	return Board::materialEvaluation();
	

}

int Board::materialEvaluation() const {
	return 
		getNumBit(Board::piece[wPawn]) * materialScore[wPawn] +
		getNumBit(Board::piece[wKnight]) * materialScore[wKnight] +
		getNumBit(Board::piece[wBishop]) * materialScore[wBishop] +
		getNumBit(Board::piece[wRook]) * materialScore[wRook] +
		getNumBit(Board::piece[wQueen]) * materialScore[wQueen] +

		getNumBit(Board::piece[bPawn]) * materialScore[bPawn] +
		getNumBit(Board::piece[bKnight]) * materialScore[bKnight] +
		getNumBit(Board::piece[bBishop]) * materialScore[bBishop] +
		getNumBit(Board::piece[bRook]) * materialScore[bRook] +
		getNumBit(Board::piece[bQueen]) * materialScore[bQueen];
}
