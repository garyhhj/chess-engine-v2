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

int Evaluation::minMaxHelper(Board& board, BoardState& boardState, int alpha, int beta, int depth) {
	//base case 
	if (depth == 0) {
		//evaluate and return some number 
		Movelist ml;
		ml.moveGen(board, boardState);
		return Evaluation::evaluate(board, boardState, ml);
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
			const int eval = Evaluation::minMaxHelper(board, boardState, alpha, beta, depth - 1);
			maxEval = std::max(maxEval, eval); 
			alpha = std::max(alpha, eval); 
			if (beta <= alpha) break; 
		}
		return maxEval;
	}

	//min
	else {
		Movelist ml;
		ml.moveGen(board, boardState);

		Board currBoard = board;
		BoardState currBoardState = boardState;

		int minEval = 50000;
		const int index = ml.getIndex();
		for (int i = 0; i < index; ++i) {
			board = currBoard;
			boardState = currBoardState;

			//search 
			board.makemove(ml.getMove(i), boardState);
			const int eval = minMaxHelper(board, boardState, alpha, beta, depth - 1);
			minEval = std::min(minEval, eval); 
			beta = std::min(beta, eval); 
			if(beta <= alpha) break; 
		}

		return minEval; 
	}
}


std::string Evaluation::minMax(Board& board, BoardState& boardState, int depth) {
	//base case(doesn't make sense to use tbh) 
	if (depth == 0) {
		//evaluate and return some number 
		Movelist ml; 
		ml.moveGen(board, boardState);
		
		Board currBoard = board;
		BoardState currBoardState = boardState;

		std::string bestmove;
		int maxEval = (boardState.getSide() == white ? -50000 : 50000);
		const int index = ml.getIndex();
		for (int i = 0; i < index; ++i) {
			board = currBoard;
			boardState = currBoardState;

			//search 
			board.makemove(ml.getMove(i), boardState);
			const int eval = Evaluation::evaluate(board, boardState, ml);
			
			if (boardState.getSide() == white && maxEval < eval) {
				maxEval = eval; 
				bestmove = Move::moveString(ml.getMove(i)); 
			}
			else if (boardState.getSide() == black && eval < maxEval) {
				maxEval = eval; 
				bestmove = Move::moveString(ml.getMove(i)); 
			}
		}
		//return maxEval;
		return bestmove;
	}

	//need some way to keep track of best move 
	int alpha = -50000; 
	int beta = 50000; 
	//max 
	if (boardState.getSide() == white) {
		Movelist ml;
		ml.moveGen(board, boardState);

		Board currBoard = board;
		BoardState currBoardState = boardState;

		std::string bestmove; 
		int maxEval = -50000;
		const int index = ml.getIndex();
		for (int i = 0; i < index; ++i) {
			board = currBoard;
			boardState = currBoardState;

			//search 
			board.makemove(ml.getMove(i), boardState);
			const int eval = Evaluation::minMaxHelper(board, boardState, alpha, beta, depth - 1);

			if (maxEval < eval) {
				maxEval = eval; 
				bestmove = Move::moveString(ml.getMove(i)); 
				std::cout << "updated bestmove: " << bestmove << std::endl;
			}
			alpha = std::max(alpha, eval);
			if (beta <= alpha) break;
		}
		//return maxEval;
		return bestmove; 
	}

	//min
	else {
		Movelist ml;
		ml.moveGen(board, boardState);

		Board currBoard = board;
		BoardState currBoardState = boardState;

		std::string bestmove; 
		int minEval = 50000;
		const int index = ml.getIndex();
		for (int i = 0; i < index; ++i) {
			board = currBoard;
			boardState = currBoardState;

			//search 
			board.makemove(ml.getMove(i), boardState);
			const int eval = minMaxHelper(board, boardState, alpha, beta, depth - 1);
			if (eval < minEval) {
				minEval = eval; 
				bestmove = Move::moveString(ml.getMove(i)); 
			}

			beta = std::min(beta, eval);
			if (beta <= alpha) break;
		}

		return bestmove;
	}
}


int Evaluation::evaluate(Board& board, BoardState& boardState, Movelist& ml){
	//material score 
	const int materialScore = Evaluation::materialEvaluation(board); 
	const int mobilityScore = Evaluation::mobilityEvaluation(ml); 
	/*std::cout
		<< "material score: " << materialScore << "\n"
		<< "mobility score: " << mobilityScore << "\n" << std::flush; */

	return
		materialScore + mobilityScore; 
		/*Evaluation::materialEvaluation(board) +
		Evaluation::mobilityEvaluation(ml); */


	//some ideas for evaluation 
	//bishops hang back 
	//knights and pawns control center 
	//pawn structure score 

	//after certain amount of material is off the board //perhaps implement through some lazy technique... stop evaluating based on position and just let engine search super deep 
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
	return ml.getIndex() * 100; 
}
