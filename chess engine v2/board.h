#pragma once
#include "macro.h"
#include "attackTable.h"
#include <string>

class Movelist; 

class BoardState {
	friend class Fen; 
	friend class Board; 
	friend class Movelist; 
public: 
	BoardState() = default;
	//BoardState(const BoardState& rhs); 
	//BoardState& operator=(BoardState rhs) noexcept; //uses swap idiom 
	//~BoardState(); 
	
	int getSide() const; 
	void debug(); 

private: 
	void swap(BoardState& lhs, BoardState& rhs); 

	int IgetSide() const; 
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
	//Board(const Board& rhs);
	//Board& operator=(Board rhs) noexcept; //uses swap idom 
	//~Board(); 

	//move generation 
	void print(const BoardState& boardState) const;
	bool attacked(const uint64_t square, const BoardState& boardState) const;
	const uint64_t safeSquares(const BoardState& boardState) const ; 
	const uint64_t checkMask(BoardState& boardState) const; 
	const uint64_t pinMaskDiagonal(const BoardState& boardState) const;
	const uint64_t pinMaskHV(const BoardState& boardState) const; 

	void makemove(move move, BoardState& boardState); 

	//evaluation 
	void test(); 

private:
//public: 
	//move generation 
	void Iprint(const BoardState& boardState) const;

	bool IattackedWhite(const uint64_t square) const;
	bool IattackedBlack(const uint64_t square) const;
	const uint64_t IsafeSquaresWhite(const BoardState& boardState) const ;
	const uint64_t IsafeSquaresBlack(const BoardState& boardState) const; 
	const uint64_t IcheckMaskWhite(BoardState& boardState) const;
	const uint64_t IcheckMaskBlack(BoardState& boardState) const;
	const uint64_t IpinMaskDiagonalWhite() const; 
	const uint64_t IpinMaskDiagonalBlack() const; 
	const uint64_t IpinMaskHVWhite() const;
	const uint64_t IpinMaskHVBlack() const; 
	void Imakemovewhite(move move, BoardState& boardState);
	void Imakemoveblack(move move, BoardState& boardState);

	//evaluation
	void Itest(); 

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
