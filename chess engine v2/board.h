#pragma once
#include "macro.h"
#include <string>

class BoardState {
	friend class Fen; 
	friend class Board; 
public: 
	BoardState() = default; 
	BoardState(const BoardState&) = delete; 

	static BoardState& Get();
	static void debug();

private: 
	void Idebug();


	int side; 
	map enpassant; 
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
	static void print();

private:
public: 
	void Iprint(); 
	//leaper pieces 
	map piece[12]; 
	map occupancy[2]; 
};


class Fen {

public: 
	static Fen& Get(); 

	static void clear(); 
	static void parse(const std::string& fen);
	static void parseStartPosition();

private: 
	void Iclear(); 
	void Iparse(const std::string& fen); 
	void IparseStartPosition(); 

};
