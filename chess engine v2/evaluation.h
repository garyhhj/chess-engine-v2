#pragma once
#include "attackTable.h"
#include "evaluationTable.h"
#include "board.h"
#include "movelist.h"
#include "randomNum.h"

#include <algorithm>

/********************
*
*Utility
*
*********************/

const int infinity = 50000; 
const int mateScore = 49000; 
const int mateScoreLowerBound = 48000; 

/********************
*
*Repetition Table
*
*********************/

class Rtable {
public: 
	Rtable() = default; 

	static Rtable& get(); 
	static void increment(const map hash);
	static void decrement(); 
	static bool repetition(const map hash); 
	static void clear(); 
	static void debug(); 

private: 
	void Iincrement(const map hash);
	void Idecrement(); 
	bool Irepetition(const map hash); 
	void Iclear(); 
	void Idebug(); 

	map rTable[1000]; 
	inline static int index = 0;	
};


/********************
*
*Transposition Table
*
*********************/

typedef struct transposition{
	map exactHash = 0x0ull; 
	int eval = 0; 
	int depth = 0; 
	int flag = 0; 
} transposition;

class Ttable {
public: 
	static Ttable& get();
	static void debug(); 
	static int lookUp(const map hash, const int depth, const int alpha, const int beta); 
	static void insert(const map hash, const int depth, const int flag, int eval);
	static void clear(); 

private: 
	Ttable();
	~Ttable();
	
	void printTtable(const int index); 
	void Idebug(); 
	int IlookUp(const map hash, const int depth, const int alpha, const int beta);
	void Iinsert(const map hash, const int depth, const int flag, int eval);
	void Iclear(); 

	transposition* tTable;

public: 
	static const int tTableSize = 0x400000;

	static const int tFlagExact = 0;
	static const int tFlagAlpha = 1;
	static const int tFlagBeta = 2;
	static const int tFlagUnknown = 3; 

	static const int unknownEval = 1 << 30; 
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
	int negamax(Board& board, BoardState& boardState, int alpha, int beta, int depth); 

	int quiesenceSearch(Board& board, BoardState& boardState, int alpha, int beta);

	int moveScore(const Board& board, move m); 
	void sortMove(const Board&, Movelist& ml); 

	int evaluate(Board& board, BoardState& boardState, int depth);
	int materialEvaluation(const Board& board);
	int positionalEvaluation(const Board& board); 

	int pawnEvaluation(const Board& board); // stacked/isolated/past pawns 
	int rookEvaluation(const Board& board); // open/semi open files 
}