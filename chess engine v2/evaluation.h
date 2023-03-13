#pragma once
#include "attackTable.h"
#include "evaluationTable.h"
#include "board.h"
#include "movelist.h"

namespace Evaluation {
	int minMax(Board& board, BoardState& boardState, int depth); 
	int evaluate(Board& board, BoardState& boardState);

	int materialEvaluation(const Board& board);

}