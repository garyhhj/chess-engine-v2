#pragma once
#include "movelist.h"
#include "macro.h"
#include "board.h"

#include <string>

class UCI {
public: 
	UCI() = default; 
	UCI& get();
	int parseMove(const std::string& move, Movelist& ml, const BoardState& boardState);
private: 
	int IparseMove(const std::string& move, Movelist& ml, int side); 

	//parse move input e7e8q  
	//return whether that move is legal within the board state 

	//parse position 
	//soemthing idk 

	// parse UCI "go" command
	// go depth something 

};