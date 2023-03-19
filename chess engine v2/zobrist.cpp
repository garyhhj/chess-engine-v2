#include "zobrist.h"
#include "board.h"

/********************
*
*Zobrist
*
*********************/

map Zobrist::pieceKey[12][64]{}; //pieceKey[piece][square]
map Zobrist::enpassant[64]{}; //enapssant[square]
map Zobrist::castleKey[16]{}; //castleKey[castleflag(binary representation)]
map Zobrist::sideKey{};

void Zobrist::initZobristTables() {
	for (int piece = 0; piece < 12; ++piece) {
		for (int square = 0; square < 64; ++square) {
			Zobrist::pieceKey[piece][square] = Random::getRandomUint64();
		}
	}

	for (int square = 0; square < 64; ++square) {
		Zobrist::enpassant[square] = Random::getRandomUint64();
	}

	for (int i = 0; i < 16; ++i) {
		Zobrist::castleKey[i] = Random::getRandomUint64();
	}

	Zobrist::sideKey = Random::getRandomUint64();
}

//return hash key for zobrist
map Zobrist::hashZobrist(const Board& board, const BoardState& boardState) {
	map hash = 0x0ull;

	//board 
	for (int piece = 0; piece < 12; ++piece) {
		map pieceBoard = board.getPiece()[piece];
		while (pieceBoard) {
			hash ^= Zobrist::pieceKey[piece][getlsbBitIndex(pieceBoard)];
			pieceBoard &= pieceBoard - 1;
		}
	}

	//boardState
	if (boardState.side == black) {
		hash ^= Zobrist::sideKey;
	}

	if (boardState.enpassant != 0x0ull) {
		hash ^= Zobrist::enpassant[getlsbBitIndex(boardState.enpassant)];
	}

	hash ^= Zobrist::castleKey[boardState.getCastleRightBin()];

	return hash;
}


