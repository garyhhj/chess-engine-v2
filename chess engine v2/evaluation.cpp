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


int Evaluation::evaluate(Board& board, BoardState& boardState, Movelist& ml){
	//material score 
	const int materialScore = Evaluation::materialEvaluation(board); 
	const int mobilityScore = Evaluation::mobilityEvaluation(ml); 
	std::cout
		<< "material score: " << materialScore << "\n"
		<< "mobility score: " << mobilityScore << "\n" << std::flush; 

	return
		Evaluation::materialEvaluation(board) +
		Evaluation::mobilityEvaluation(ml); 

}

int Evaluation::materialEvaluation(const Board& board) {
	return 
		getNumBit(board.getPiece()[wPawn]) * materialScore[wPawn] +
		getNumBit(board.getPiece()[wKnight]) * materialScore[wKnight] +
		getNumBit(board.getPiece()[wBishop]) * materialScore[wBishop] +
		getNumBit(board.getPiece()[wRook]) * materialScore[wRook] +
		getNumBit(board.getPiece()[wQueen]) * materialScore[wQueen] +

		getNumBit(board.getPiece()[bPawn]) * materialScore[bPawn] +
		getNumBit(board.getPiece()[bKnight]) * materialScore[bKnight] +
		getNumBit(board.getPiece()[bBishop]) * materialScore[bBishop] +
		getNumBit(board.getPiece()[bRook]) * materialScore[bRook] +
		getNumBit(board.getPiece()[bQueen]) * materialScore[bQueen];
}

int Evaluation::mobilityEvaluation(const Movelist& ml) {
	return ml.getIndex() * 1000; 
}
