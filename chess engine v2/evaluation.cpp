#include "evaluation.h"

/********************
*
*Utils
*
*********************/

int Evaluation::ply = 0;
int Evaluation::nodes = 0;

move Evaluation::killerMoves[2][Evaluation::MAXPLY]{}; //[priority][ply]
int Evaluation::historyScore[12][Evaluation::MAXPLY]{}; //[piece][ply]
int Evaluation::pvLength[Evaluation::MAXPLY]{}; //[ply]
move Evaluation::pvTable[Evaluation::MAXPLY][Evaluation::MAXPLY]{}; //[ply][ply] 



/********************
*
*Evaluation
*
*********************/

/********************
*negamax + alphabeta
*********************/


int Evaluation::quiesenceSearch(Board& board, BoardState& boardState, int alpha, int beta) {
	++Evaluation::nodes;


	Movelist ml;
	ml.moveGen(board, boardState);
	Evaluation::sortMove(board, ml);
	const int mlIndex = ml.getIndex();

	bool captures = false; 
	{
		for (int i = 0; i < mlIndex; ++i) {
			if (Move::captureFlag(ml.getMove(i))) {
				captures = true; 
				break; 
			}
		}
	}

	Board currBoard = board;
	BoardState currBoardState = boardState;

	
	//only want to evaluate positions with non captures 
	if (!captures) {
		return Evaluation::evaluate(board, boardState, ml, Evaluation::ply);
	}

	for (int i = 0; i < mlIndex; ++i) {
		if (!Move::captureFlag(ml.getMove(i))) continue; 

		board.makemove(ml.getMove(i), boardState);
		++Evaluation::ply;
		const int eval = -Evaluation::quiesenceSearch(board, boardState, -beta, -alpha);
		--Evaluation::ply;


		board = currBoard;
		boardState = currBoardState;

		//fail hard beta cut off (oppnent has refutation so this branch will never be played assuming optimal play) 
		if (eval >= beta) {
			return beta;
		}

		//found better move 
		if (eval > alpha) {
			alpha = eval;
		}
	}

	return alpha;
}


int Evaluation::negamax(Board& board, BoardState& boardState, int alpha, int beta, int depth) {
	++Evaluation::nodes; 
	Evaluation::pvLength[ply] = ply; 

	Movelist ml; 
	ml.moveGen(board, boardState); 
	Evaluation::sortMove(board, ml); 
	const int mlIndex = ml.getIndex(); 

	Board currBoard = board; 
	BoardState currBoardState = boardState; 

	if (depth == 0 || mlIndex == 0) {
		return Evaluation::quiesenceSearch(board, boardState, alpha, beta);  
		//return Evaluation::evaluate(board, boardState, ml, ply); 
	}

	//increase search depth while in check 
	if (board.attacked(board.getPiece()[(boardState.getSide() == white ? wKing : bKing)], boardState)) {
		++depth; 
	}

	for (int i = 0; i < mlIndex; ++i) {
		const move currmove = ml.getMove(i); 
		board.makemove(currmove, boardState); 
		++Evaluation::ply; 
		const int eval = -Evaluation::negamax(board, boardState, -beta, -alpha, depth - 1);
		--Evaluation::ply; 

		//if(Evaluation::ply == 0) Move::decode(ml.getMove(i)); std::cout << " score: " << eval << std::endl;
		


		board = currBoard;
		boardState = currBoardState;
		
		//fail hard beta cut off (oppnent has refutation so this branch will never be played assuming optimal play) 
		if (eval >= beta) {
			if (!Move::captureFlag(currmove)) {
				Evaluation::killerMoves[1][ply] = Evaluation::killerMoves[0][ply];
				Evaluation::killerMoves[0][ply] = currmove;
			}

			return beta; 
		}

		//found better move 
		if (eval > alpha) {
			using namespace Evaluation;
			historyScore[Move::piece(currmove)][Move::targetSquare(currmove)] += depth; 

			pvTable[ply][ply] = currmove; 
			
			for (int nextPly = ply + 1; nextPly < pvLength[ply + 1]; ++nextPly) {
				pvTable[ply][nextPly] = pvTable[ply + 1][nextPly]; 
			}
			pvLength[ply] = pvLength[ply + 1]; 

			/*if(Evaluation::ply == 0) Evaluation::bestmove = Move::moveString(currmove); */
			alpha = eval; 
		}
	}

	return alpha; 
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
			if (targetSquare & board.getPiece()[i]) {
				targetPiece = i; 
				break; 
			}
		}

		return mvvlva[Move::piece(m)][targetPiece] + 10000;
	}

	//quiet moves 
	else {
		//killer move 
		if (Evaluation::killerMoves[0][ply] == m) return 9000;
		else if (Evaluation::killerMoves[1][ply] == m) return 8000;

		//history score 
		else return Evaluation::historyScore[Move::piece(m)][Move::targetSquare(m)]; 
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
int Evaluation::evaluate(Board& board, BoardState& boardState, Movelist& ml, int ply){
	//checkmate and stalemate 
	const map checkMask = board.checkMask(boardState);
	if (checkMask != AllOne && !ml.getIndex()) {
		return -49000 + ply; 
		//return (boardState.getSide() == white ? -49000 - depth : -49000 - depth);
	}
	else if (checkMask == AllOne && !ml.getIndex()) {
		//stalemate score
		return 0;
	}
	
	//material score 
	const int materialScore = Evaluation::materialEvaluation(board); 
	//const int mobilityScore = Evaluation::mobilityEvaluation(ml); 
	const int positionalScore = Evaluation::positionalEvaluation(board); 
	/*std::cout
		<< "material score: " << materialScore << "\n"
		<< "mobility score: " << mobilityScore << "\n" << std::flush; */


	return (boardState.getSide() == white ? 1 : -1) * (
		materialScore +
		//mobilityScore + 
		positionalScore); 
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
