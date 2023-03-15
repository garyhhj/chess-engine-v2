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

/********************
*minimax + search
*********************/
int Evaluation::quiesenceSearch(Board& board, BoardState& boardState, int alpha, int beta, int depth) {
	Movelist ml;
	ml.moveGen(board, boardState);

	Board currBoard = board;
	BoardState currBoardState = boardState;

	bool captures = false; 
	{
		const int mlIndex = ml.getIndex();
		for (int i = 0; i < mlIndex; ++i) { if (Move::captureFlag(ml.getMove(i))) captures = true; }
	}

	if (!captures || ml.getIndex() == 0) {
		return Evaluation::evaluate(board, boardState, ml, depth);
	}

	//max 
	if (boardState.getSide() == white) {
		int maxEval = -50000;
		const int index = ml.getIndex();
		for (int i = 0; i < index; ++i) {
			if(!Move::captureFlag(ml.getMove(i))) continue; 

			board = currBoard;
			boardState = currBoardState;
			
			//search 
			board.makemove(ml.getMove(i), boardState);
			const int eval = Evaluation::quiesenceSearch(board, boardState, alpha, beta, depth - 1);
			maxEval = std::max(maxEval, eval);
			alpha = std::max(alpha, eval);
			if (beta <= alpha) break;
		}
		return maxEval;
	}

	//min
	else {
		int minEval = 50000;
		const int index = ml.getIndex();
		for (int i = 0; i < index; ++i) {
			if (!Move::captureFlag(ml.getMove(i))) continue;
			board = currBoard;
			boardState = currBoardState;

			//search 
			board.makemove(ml.getMove(i), boardState);
			const int eval = Evaluation::quiesenceSearch(board, boardState, alpha, beta, depth - 1);
			minEval = std::min(minEval, eval);
			beta = std::min(beta, eval);
			if (beta <= alpha) break;
		}
		return minEval;
	}
}



int Evaluation::minMaxHelper(Board& board, BoardState& boardState, int alpha, int beta, int depth) {
	Movelist ml;
	ml.moveGen(board, boardState);

	Board currBoard = board;
	BoardState currBoardState = boardState;

	//base case, reached depth or no more moves(checkmate/stalemate) 
	if (depth == 0 || ml.getIndex() == 0) {
		//call quiesence search 
		return Evaluation::quiesenceSearch(board, boardState, alpha, beta, depth); 
		//return Evaluation::evaluate(board, boardState, ml, depth);
	}

	//max 
	if (boardState.getSide() == white) {
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
		int minEval = 50000;
		const int index = ml.getIndex();
		for (int i = 0; i < index; ++i) {
			board = currBoard;
			boardState = currBoardState;

			//search 
			board.makemove(ml.getMove(i), boardState);
			const int eval = Evaluation::minMaxHelper(board, boardState, alpha, beta, depth - 1);
			minEval = std::min(minEval, eval); 
			beta = std::min(beta, eval); 
			if(beta <= alpha) break; 
		}
		return minEval; 
	}
}


int Evaluation::minMax(Board& board, BoardState& boardState, int depth, std::string& bestmove) {
	Movelist ml;
	ml.moveGen(board, boardState);

	Board currBoard = board;
	BoardState currBoardState = boardState;

	//base case
	if (depth <= 0) return 0; 

	//need some way to keep track of best move 
	int alpha = -50000; 
	int beta = 50000; 
	//max 
	if (boardState.getSide() == white) {
		int maxEval = -50000;
		const int index = ml.getIndex();
		for (int i = 0; i < index; ++i) {
			board = currBoard;
			boardState = currBoardState;

			//search 
			board.makemove(ml.getMove(i), boardState);
			const int eval = Evaluation::minMaxHelper(board, boardState, alpha, beta, depth - 1);
			
			Move::decode(ml.getMove(i)); 
			std::cout << " eval: " << eval << std::endl; 

			if (maxEval < eval) {
				maxEval = eval; 
				bestmove = Move::moveString(ml.getMove(i)); 
			}
			alpha = std::max(alpha, eval);
			if (beta <= alpha) break;
		}

		board = currBoard; 
		boardState = currBoardState; 
		return maxEval;
	}

	//min
	else {
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

		board = currBoard; 
		boardState = currBoardState; 
		return minEval;
	}
}



/********************
*sorting Moves
*********************/
int Evaluation::moveScore(const Board& board, move m) {
	//capture moves 
	if (Move::captureFlag(m)) {
		const map targetSquare = indexSquare[Move::targetSquare(m)];

		int targetPiece = wPawn; 
		for (int i = 0; i < 12; ++i) {
			if (targetSquare && board.getPiece()[i]) {
				targetPiece = i; 
				break; 
			}
		}

		return mvvlva[Move::piece(m)][targetPiece];
	}

	//quiet moves 
	else {

	}

	return 0; 
}

void Evaluation::sortMove(const Board& board, Movelist& ml) {
	//std::sort();
	std::sort(ml.getMovelist(), ml.getMovelist() + ml.getIndex(),
		[board](const move& lhs, const move& rhs) -> bool {
			return Evaluation::moveScore(board, lhs) > Evaluation::moveScore(board, rhs);
		});
}



/********************
*evaluation 
*********************/
int Evaluation::evaluate(Board& board, BoardState& boardState, Movelist& ml, int depth){
	//checkmate and stalemate 
	const map checkMask = board.checkMask(boardState);
	if (checkMask != AllOne && !ml.getIndex()) {
		//checkmate score 
		return (boardState.getSide() == white ? -49000 - depth : 49000 + depth);
	}
	else if (checkMask == AllOne && !ml.getIndex()) {
		//stalemate score 
		return 0;
	}
	
	//material score 
	const int materialScore = Evaluation::materialEvaluation(board); 
	const int mobilityScore = Evaluation::mobilityEvaluation(ml); 
	const int positionalScore = Evaluation::positionalEvaluation(board); 
	/*std::cout
		<< "material score: " << materialScore << "\n"
		<< "mobility score: " << mobilityScore << "\n" << std::flush; */


	return
		materialScore + 
		mobilityScore + 
		positionalScore; 
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

int Evaluation::positionalEvaluation(const Board& board) {
	int score = 0; 
	//leapers 
	map piece = board.getPiece()[wPawn]; 
	while (piece) {
		score += wPawnPScore[getlsbBitIndex(piece)]; 
		piece &= piece - 1;
	}

	piece = board.getPiece()[bPawn];
	while (piece) {
		score += bPawnPScore[getlsbBitIndex(piece)];
		piece &= piece - 1;
	}

	piece = board.getPiece()[wKnight];
	while (piece) {
		score += wKnightPScore[getlsbBitIndex(piece)];
		piece &= piece - 1;
	}

	piece = board.getPiece()[bKnight];
	while (piece) {
		score += bKnightPScore[getlsbBitIndex(piece)];
		piece &= piece - 1;
	}

	piece = board.getPiece()[wKing];
	while (piece) {
		score += wKingPScore[getlsbBitIndex(piece)];
		piece &= piece - 1;
	}

	piece = board.getPiece()[bKing];
	while (piece) {
		score += bKingPScore[getlsbBitIndex(piece)];
		piece &= piece - 1;
	}

	//sliders 
	piece = board.getPiece()[wBishop];
	while (piece) {
		score += wBishopPScore[getlsbBitIndex(piece)];
		piece &= piece - 1;
	}

	piece = board.getPiece()[bBishop];
	while (piece) {
		score += bBishopPScore[getlsbBitIndex(piece)];
		piece &= piece - 1;
	}

	piece = board.getPiece()[wRook];
	while (piece) {
		score += wRookPScore[getlsbBitIndex(piece)];
		piece &= piece - 1;
	}

	piece = board.getPiece()[bRook];
	while (piece) {
		score += bRookPScore[getlsbBitIndex(piece)];
		piece &= piece - 1;
	}	
	
	return score; 
}


//enum piece : const int {
//	wPawn = 0,
//	wKnight = 1,
//	wKing = 2,
//	wBishop = 3,
//	wRook = 4,
//	wQueen = 5,
//
//	bPawn = 6,
//	bKnight = 7,
//	bKing = 8,
//	bBishop = 9,
//	bRook = 10,
//	bQueen = 11,
//};
