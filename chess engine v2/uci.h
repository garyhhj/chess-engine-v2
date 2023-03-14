#pragma once
#include "movelist.h"
#include "macro.h"
#include "board.h"
#include "evaluation.h"

#include <iostream>
#include <string>
#include <sstream>

class UCI {
public: 
	UCI() = default; 
	static UCI& get();
	static int parseMove(const std::string& move, Movelist& ml, const BoardState& boardState); //eg. e7e8q returns index to move in movelist or -1 
	static void parsePosition(const std::string& position, Board& board, BoardState& boardState); 
	static void parseGo(const std::string& command, Board& board, BoardState& boardState);
	static void uciRun(Board& board, BoardState& boardState); 
private: 
	int IparseMove(const std::string& move, Movelist& ml, int side); 
	void IparsePosition(const std::string& position, Board& board, BoardState& boardState);
	void IparseGo(const std::string& command, Board& board, BoardState& boardState);
	void IuciRun(Board& board, BoardState& boardState); 





};