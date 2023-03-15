#pragma once
#include "attackTable.h"
#include "evaluationTable.h"
#include "board.h"
#include "movelist.h"

#include <algorithm>

namespace Evaluation {

	extern int ply; 
	extern int nodes; 
	extern std::string bestmove;
	extern move killerMoves[2][64]; //[priority][ply]
	extern int historyScore[12][64]; //[piece][ply]


	//int negamaxHelper(Board& board, BoardState& boardState, int alpha, int beta, int depth);
	int negamax(Board& board, BoardState& boardState, int alpha, int beta, int depth); 

	//int minMax(Board& board, BoardState& boardState, int depth, std::string& bestmove);
	//int minMaxHelper(Board& board, BoardState& boardState, int alpha, int beta, int depth); 
	int quiesenceSearch(Board& board, BoardState& boardState, int alpha, int beta);

	int moveScore(const Board& board, move m); 
	void sortMove(const Board&, Movelist& ml); 

	int evaluate(Board& board, BoardState& boardState, Movelist& ml, int depth);
	int materialEvaluation(const Board& board);
	int mobilityEvaluation(const Movelist& ml);
	int positionalEvaluation(const Board& board); 
}