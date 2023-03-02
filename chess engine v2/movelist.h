#pragma once
#include "macro.h"
#include "board.h"

#include <stdint.h>
#include <iostream>

class Move {
	Move(); 
	Move(int sourceSquare, int targetSquare, int piece, int promotePiece, 
		bool captureFlag, bool doublePushFlag, bool enpassantFlag, bool castlingFlag);

	Move(uint64_t sourceSquare, uint64_t targetSquare, int piece, int promotePiece,
		bool captureFlag, bool doublePushFlag, bool enpassantFlag, bool castlingFlag);

	void decode(); 
private: 

	constexpr int sourceSquare(); 
	constexpr int targetSquare();
	constexpr int piece();
	constexpr int promotePiece();

	constexpr bool captureFlag();
	constexpr bool doublePushFlag(); 
	constexpr bool enpassantFlag(); 
	constexpr bool castlingFlag(); 

	uint32_t move; 
};

class Movelist {
public: 
	Movelist(); 
	Movelist(const Movelist& rhs);
	Movelist& operator=(Movelist& rhs);
	~Movelist(); 

	consteval void pushBack(uint64_t move); 
	void print(); 

private:
	void swap(Movelist& m1, Movelist& m2); 
	constexpr void moveGen(const Board& board, const BoardState& boardState); 
	constexpr void moveGenWhite(const Board& board, const BoardState& boardState);
	constexpr void moveGenBlack(const Board& board, const BoardState& boardState);

	uint64_t* movelist;
	int index; 
};