#pragma once
#include "attackTable.h"
#include "evaluationTable.h"
#include "board.h"
#include "movelist.h"
#include "randomNum.h"

#include <algorithm>

/********************
*
*Transposition Table
*
*********************/

const int tTableSize = 0x400000;

const int tFlagExact = 0; 
const int tFlagAlpha = 1; 
const int tFlagBeta = 2; 

typedef struct transposition{
	map exactHash; 
	int eval; 
	int depth; 
	int flag = 0; 
} transposition;

class Ttable {
public: 
	static Ttable& get();
	static void debug(); 

private: 
	Ttable();
	~Ttable();
	
	void printTtable(const int index); 
	void Idebug(); 

	transposition* tTable;
};

/********************
*
*Evaluation
*
*********************/

namespace Evaluation {
	const int MAXPLY = 64; 
	const int FULLDEPTHMOVES = 4; 
	const int REDUCTIONLIMIT = 3; 

	extern int ply; 
	extern int nodes; 

	extern move killerMoves[2][MAXPLY]; //[priority][ply]
	extern int historyScore[12][MAXPLY]; //[piece][ply]

	extern int pvLength[MAXPLY]; //[ply]
	extern move pvTable[MAXPLY][MAXPLY]; //[ply][ply] 

	bool canLMR(move m); 
	bool canNullMove(const Board& board); 
	//int negamaxHelper(Board& board, BoardState& boardState, int alpha, int beta, int depth);
	int negamax(Board& board, BoardState& boardState, int alpha, int beta, int depth); 

	//int minMax(Board& board, BoardState& boardState, int depth, std::string& bestmove);
	//int minMaxHelper(Board& board, BoardState& boardState, int alpha, int beta, int depth); 
	int quiesenceSearch(Board& board, BoardState& boardState, int alpha, int beta);

	int moveScore(const Board& board, move m); 
	void sortMove(const Board&, Movelist& ml); 

	int evaluate(Board& board, BoardState& boardState, int depth);
	int materialEvaluation(const Board& board);
	int mobilityEvaluation(const Movelist& ml);
	int positionalEvaluation(const Board& board); 
}