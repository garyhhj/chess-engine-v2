#pragma once
#include "macro.h"
#include "attackTable.h"

#include <string>

class BoardState {
	friend class Fen; 
	friend class Board; 
	friend class Movelist; 
public: 
	BoardState() = default; 
	BoardState(const BoardState&) = delete; 

	//static BoardState& Get();
	//static void debug();
	void debug(); 

private: 
	void Idebug();


	int side; 
	map enpassant; 
	bool castleRightWQ; 
	bool castleRightWK; 
	bool castleRightBQ; 
	bool castleRightBK; 

	bool doubleCheck; 
};

class Board {
	friend class Fen; 
	friend class Movelist; 
public: 
	Board() = default; 
	Board(const Board&) = delete; 

	static Board& Get(); 
	static void print();
	bool attacked(const uint64_t square); 
	const uint64_t checkMask(); 
	const uint64_t pinMaskDiagonal();
	const uint64_t pinMaskHV(); 

private:
public: 
	bool IattackedWhite(const uint64_t square); 
	bool IattackedBlack(const uint64_t square);
	const uint64_t IcheckMaskWhite(); 
	const uint64_t IcheckMaskBlack(); 
	const uint64_t IpinMaskDiagonalWhite(); 
	const uint64_t IpinMaskDiagonalBlack(); 
	const uint64_t IpinMaskHVWhite();
	const uint64_t IpinMaskHVBlack(); 

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
