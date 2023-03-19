#pragma once
#include "macro.h"
#include "randomNum.h"
#include <stdint.h>

class Board; 
class BoardState; 

namespace Zobrist {
	extern map pieceKey[12][64]; //pieceKey[piece][square]
	extern map enpassant[64]; //enapssant[square]
	extern map castleKey[16]; //castleKey[castleflag(binary representation)]
	extern map sideKey;

	extern void initZobristTables();
	extern map hashZobrist(const Board& board, const BoardState& boardState);
}