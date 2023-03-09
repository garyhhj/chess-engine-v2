#pragma once
#include "macro.h"
#include "board.h"
#include "attackTable.h"

#include <stdint.h>
#include <iostream>



class Movelist {
public: 
	Movelist(); 
	Movelist(const Movelist& rhs);
	Movelist& operator=(Movelist& rhs);
	~Movelist(); 

	move getMove(const int index); 
	void moveGen(const Board& board, BoardState& boardState);
	constexpr void pushBack(const move m); 
	void print(); 

private:
	void swap(Movelist& m1, Movelist& m2); 
	void moveGenWhite(const Board& board, BoardState& boardState);
	void moveGenBlack(const Board& board, BoardState& boardState);

	move* movelist;
	int index; 
};