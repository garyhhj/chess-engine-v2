#pragma once
#include "macro.h"
#include "board.h"
#include "attackTable.h"
#include <stdint.h>
#include <iostream>



class Movelist {
public: 
	Movelist() = default; 
	/*Movelist(const Movelist& rhs);
	Movelist& operator=(Movelist& rhs);
	~Movelist(); */

	move* getMovelist(); 
	move getMove(const int index); 
	const int getIndex() const; 
	void moveGen(const Board& board, BoardState& boardState);
	constexpr void pushBack(const move m); 
	void print(); 

private:
	//void swap(Movelist& m1, Movelist& m2); 
	void moveGenWhite(const Board& board, BoardState& boardState);
	void moveGenBlack(const Board& board, BoardState& boardState);

	//move* movelist;
	move movelist[256];
	int index = 0; 
};