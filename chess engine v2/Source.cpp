#include "macro.h"
#include "attackTable.h"
#include "board.h"
#include "movelist.h"
#include "perft.h"
#include "uci.h"
#include "evaluation.h"
#include "zobrist.h"
#include "randomNum.h"
//#include "evaluationTable.h"

#include <iostream>
#include <bitset>
#include <map>


int main() {
	
	Board board;
	BoardState boardState;
	Zobrist::initZobristTables();
	Ttable::clear(); 

	std::string fenStart = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ";

	Fen::parse(fenStart, board, boardState);

	UCI::uciRun(board, boardState);
}