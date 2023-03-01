#pragma once
#include "macro.h"

#include <stdint.h>
#include <iostream>

class Move {
	Move(); 
	Move(int sourceSquare, int targetSquare, int piece, int promotePiece, bool captureFlag, bool doublePushFlag, bool enpassantFlag, bool castlingFlag);


	void decode(); 

private: 
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

	uint64_t* movelist;
	int index; 
};