#pragma once
#include "macro.h"
#include "board.h"

#include <stdint.h>
#include <iostream>

typedef uint32_t move; 

namespace Move {
	move makemove(int sourceSquare, int targetSquare, int piece, int promotePiece,
		bool captureFlag, bool doublePushFlag, bool enpassantFlag, bool castlingFlag);

	move makemove(uint64_t sourceSquare, uint64_t targetSquare, int piece, int promotePiece,
		bool captureFlag, bool doublePushFlag, bool enpassantFlag, bool castlingFlag);

	void decode(const move m);


	constexpr int sourceSquare(const move m); 
	constexpr int targetSquare(const move m);
	constexpr int piece(const move m);
	constexpr int promotePiece(const move m);
	
	constexpr bool captureFlag(const move m);
	constexpr bool doublePushFlag(const move m);
	constexpr bool enpassantFlag(const move m);
	constexpr bool castlingFlag(const move m);

};



class Movelist {
public: 
	Movelist(); 
	Movelist(const Movelist& rhs);
	Movelist& operator=(Movelist& rhs);
	~Movelist(); 

	constexpr void pushBack(const move m); 
	void print(); 

private:
	void swap(Movelist& m1, Movelist& m2); 
	constexpr void moveGen(const Board& board, const BoardState& boardState); 
	constexpr void moveGenWhite(const Board& board, const BoardState& boardState);
	constexpr void moveGenBlack(const Board& board, const BoardState& boardState);

	move* movelist;
	int index; 
};