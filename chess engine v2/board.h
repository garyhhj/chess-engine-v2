#pragma once
#include "macro.h"

class BoardState {
	friend constexpr void fen::parseStartPosition(); 
	friend constexpr void fen::parse(); 

private: 
	int side; 
	map enapssant; 
	map castlingRights; 
};

class Board {
	friend constexpr void fen::parseStartPosition(); 
	friend constexpr void fen::parse(); 

	//make move 

private: 
	//leaper pieces 
	map wPawn; map bPawn; 
	map wKnight; map bKnight; 
	map wKing; map bKing; 

	//slider pieces 
	map wBishop; map bBishop; 
	map wRook; map bRook; 
	map wQueen; map bQueen; 
};

namespace fen {
	constexpr void parse(); 
	constexpr void parseStartPosition();
}

