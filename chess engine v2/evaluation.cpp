#include "evaluation.h"

/********************
*
*Utils
*
*********************/


/********************
*
*Evaluation
*
*********************/

int Evaluation::minMax(Board& board, BoardState& boardState, int depth) {
	//base case 
	if (depth == 0) {
		//evaluate and return some number 
		Evaluation::evaluate(board, boardState); 
	}
	
	//max 
	if (boardState.getSide() == white) {
		Movelist ml; 
		ml.moveGen(board, boardState); 

		Board currBoard = board; 
		BoardState currBoardState = boardState; 

		int maxEval = -50000;
		const int index = ml.getIndex();
		for (int i = 0; i < index; ++i) {
			board = currBoard; 
			boardState = currBoardState; 

			//search 
			board.makemove(ml.getMove(i), boardState); 
			maxEval = std::max(Evaluation::minMax(board, boardState, depth - 1), maxEval); 
		}
		return maxEval; 
	}

	//min
	else {
		Movelist ml; 
		ml.moveGen(board, boardState); 

		Board currBoard = board; 
		BoardState currBoardState = boardState; 
		
		int maxEval = 50000;
		const int index = ml.getIndex();
		for (int i = 0; i < index; ++i) {
			board = currBoard; 
			boardState = currBoardState;

			//search 
			board.makemove(ml.getMove(i), boardState);
			maxEval = std::min(minMax(board, boardState, depth - 1), maxEval); 
		}
		return maxEval; 
	}
}


int Evaluation::evaluate(Board& board, BoardState& boardState){
	//material score 
	return Evaluation::materialEvaluation(board);
	

}

int Evaluation::materialEvaluation(const Board& board) {
	return materialScore[wPawn];
		//getNumBit(Board::piece[wPawn]) * materialScore[wPawn] +
		//getNumBit(Board::piece[wKnight]) * materialScore[wKnight] +
		//getNumBit(Board::piece[wBishop]) * materialScore[wBishop] +
		//getNumBit(Board::piece[wRook]) * materialScore[wRook] +
		//getNumBit(Board::piece[wQueen]) * materialScore[wQueen] +

		//getNumBit(Board::piece[bPawn]) * materialScore[bPawn] +
		//getNumBit(Board::piece[bKnight]) * materialScore[bKnight] +
		//getNumBit(Board::piece[bBishop]) * materialScore[bBishop] +
		//getNumBit(Board::piece[bRook]) * materialScore[bRook] +
		//getNumBit(Board::piece[bQueen]) * materialScore[bQueen];
}
