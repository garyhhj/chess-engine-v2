#pragma once
#include "macro.h"
#include "attackTable.h"

#include <string>

class BoardState {
	friend class Fen; 
	friend class Board; 
	friend class Movelist; 
public: 
	BoardState();
	BoardState(const BoardState& rhs); 
	BoardState& operator=(BoardState rhs) noexcept; //uses swap idiom 

	void debug(); 

private: 
	void swap(BoardState& lhs, BoardState& rhs); 
	void Idebug();


	int* side; 
	map* enpassant; 
	bool* castleRightWQ; 
	bool* castleRightWK; 
	bool* castleRightBQ; 
	bool* castleRightBK; 

	bool* doubleCheck; 
};

class Board {
	friend class Fen; 
	friend class Movelist; 
public: 
	Board() = default; 
	Board(const Board&) = delete; 

	//static Board& Get(); 
	static void print();
	bool attacked(const uint64_t square); 
	const uint64_t checkMask(); 
	const uint64_t pinMaskDiagonal();
	const uint64_t pinMaskHV(); 

private:
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

	static void clear(Board& board, BoardState& boardState); 
	static void parse(const std::string& fen, Board& board, BoardState& boardState);
	static void parseStartPosition(Board& board, BoardState& boardState);

private: 
	void Iclear(Board& board, BoardState& boardState);
	void Iparse(const std::string& fen, Board& board, BoardState& boardState);
	void IparseStartPosition(Board& board, BoardState& boardState);

};
