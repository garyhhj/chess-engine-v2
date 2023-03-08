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
	~BoardState(); 
	

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
	Board(); 
	Board(const Board& rhs);
	Board& operator=(Board& rhs); //uses swap idom 
	~Board(); 

	void print(const BoardState& boardState) const;
	bool attacked(const uint64_t square, const BoardState& boardState) const;
	const uint64_t checkMask(BoardState& boardState) const; 
	const uint64_t pinMaskDiagonal(const BoardState& boardState) const;
	const uint64_t pinMaskHV(const BoardState& boardState) const; 

private:
	void Iprint(const BoardState& boardState) const;

	bool IattackedWhite(const uint64_t square) const;
	bool IattackedBlack(const uint64_t square) const;
	const uint64_t IcheckMaskWhite(BoardState& boardState) const;
	const uint64_t IcheckMaskBlack(BoardState& boardState) const;
	const uint64_t IpinMaskDiagonalWhite() const; 
	const uint64_t IpinMaskDiagonalBlack() const; 
	const uint64_t IpinMaskHVWhite() const;
	const uint64_t IpinMaskHVBlack() const; 

	//leaper pieces 
	map* piece; //always size 12 
	map* occupancy; //always size 2 
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
