#pragma once
#include "macro.h"
#include <string>

//singleton
class BoardState {
	friend class Fen; 
public: 
	BoardState() = default; 
	BoardState(const BoardState&) = delete; 

	static BoardState& Get();
	static void debug();

private: 
	void Idebug();


	int side; 
	map enpassant; 
	//declare castling right with bool? and just have many of them 
	bool castleRightWQ; 
	bool castleRightWK; 
	bool castleRightBQ; 
	bool castleRightBK; 
};

class Board {
	friend class Fen; 
public: 
	Board() = default; 
	Board(const Board&) = delete; 

	static Board& Get(); 
	
private: 
	//leaper pieces 
	map piece[12]; 
	map occupancy[2]; 
};


class Fen {

public: 
	static Fen& Get(); 

	static constexpr void clear(); 
	static constexpr void parse(const std::string& fen);
	static constexpr void parseStartPosition();

private: 
	constexpr void Iclear(); 
	constexpr void Iparse(const std::string& fen); 
	constexpr void IparseStartPosition(); 

};
