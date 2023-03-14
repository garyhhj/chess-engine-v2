#pragma once
#include "attackTable.h"
#include "evaluationTable.h"
#include "board.h"
#include "movelist.h"

namespace Evaluation {
	std::string minMax(Board& board, BoardState& boardState, int depth); //returns move string of best move 
	int minMaxHelper(Board& board, BoardState& boardState, int alpha, int beta, int depth); 
	int evaluate(Board& board, BoardState& boardState, Movelist& ml);

	int materialEvaluation(const Board& board);
	int mobilityEvaluation(const Movelist& ml);
	int positionalEvaluation(const Board& board); 
}