#include "movelist.h"

/********************
*
*Move
*
*********************/

/*
		 binary move bits                               hexidecimal constants

   0000 0000 0000 0000 0011 1111    source square       0x3f
   0000 0000 0000 1111 1100 0000    target square       0xfc0
   0000 0000 1111 0000 0000 0000    piece               0xf000
   0000 1111 0000 0000 0000 0000    promoted piece      0xf0000
   0001 0000 0000 0000 0000 0000    capture flag        0x100000
   0010 0000 0000 0000 0000 0000    double push flag    0x200000
   0100 0000 0000 0000 0000 0000    enpassant flag      0x400000
   1000 0000 0000 0000 0000 0000    castling flag       0x800000
*/

Move::Move() : move(0x0ul) {}
Move::Move(int sourceSquare, int targetSquare, int piece, int promotePiece, bool captureFlag, bool doublePushFlag, bool enpassantFlag, bool castlingFlag) :
	move((sourceSquare & 0x3f) | (targetSquare << 6 & 0xfc0) | (piece << 12 & 0xf000) | (promotePiece << 16 & 0xf0000) |
		(captureFlag ? 0x1 << 20 : 0x0) | (doublePushFlag ? 0x1 << 21 : 0x0) | (enpassantFlag ? 0x1 << 22 : 0x0) | (castlingFlag ? 0x1 << 23 : 0x0)) {}

Move::Move(uint64_t sourceSquare, uint64_t targetSquare, int piece, int promotePiece, bool captureFlag, bool doublePushFlag, bool enpassantFlag, bool castlingFlag) :
	move((getlsbBitIndex(sourceSquare) & 0x3f) | (getlsbBitIndex(targetSquare) << 6 & 0xfc0) | (piece << 12 & 0xf000) | (promotePiece << 16 & 0xf0000) |
		(captureFlag ? 0x1 << 20 : 0x0) | (doublePushFlag ? 0x1 << 21 : 0x0) | (enpassantFlag ? 0x1 << 22 : 0x0) | (castlingFlag ? 0x1 << 23 : 0x0)) {}


void Move::decode() {
	//print out everything in order?
	
	using namespace std;
	std::cout << "move: ";

	//sourceSquare 
	std::cout << "SS: "; 
	std::cout << 'A' + Move::sourceSquare() % 8; 
	std::cout << Move::sourceSquare() / 8; 

	//target Square 
	std::cout << "TS: "; 
	std::cout << 'A' + Move::targetSquare() % 8; 
	std::cout << Move::targetSquare() / 8; 

	//piece
	std::cout << pieceChar[Move::piece()]; 

	//promote piece 
	std::cout << pieceChar[Move::promotePiece()]; 

	//capture 
	std::cout << (Move::captureFlag() ? 'c' : '-');
	std::cout << " "; 

	//double push 
	std::cout << (Move::doublePushFlag() ? "dp" : "--");
	std::cout << " "; 

	//enpassant 
	std::cout << (Move::enpassantFlag() ? 'e' : '-');
	std::cout << " "; 

	//castle 
	std::cout << (Move::castlingFlag() ? 'c' : '-'); 
	std::cout << " "; 
}

constexpr int Move::sourceSquare() { return move & 0x3f; }
constexpr int Move::targetSquare() { return move & 0xfc0 >> 6; }
constexpr int Move::piece() { return move & 0xf000 >> 12; }
constexpr int Move::promotePiece() { return move & 0xf0000 >> 16; }

constexpr bool Move::captureFlag() { return move & 0x100000; }
constexpr bool Move::doublePushFlag() { return move & 0x200000; }
constexpr bool Move::enpassantFlag() { return move & 0x400000; }
constexpr bool Move::castlingFlag() { return move & 0x800000; }

/********************
*
*Movelist
*
*********************/

Movelist::Movelist() : movelist(new uint64_t[256]), index(0) {}


//rule of 3.5
Movelist::Movelist(const Movelist& rhs) : movelist(new uint64_t[256]), index(rhs.index) {
	std::cout << "copy constructor" << std::endl; 
	for (int i = 0; i < index; ++i) {
		movelist[i] = rhs.movelist[i]; 
	}
}

Movelist& Movelist::operator=(Movelist& rhs) {
	std::cout << "copy assignment operator" << std::endl; 
	Movelist::swap(*this, rhs); 
	rhs.movelist = nullptr; 
	return *this; 
}


Movelist::~Movelist() {
	std::cout << "destructor" << std::endl; 
	delete[] movelist; 
}

consteval void Movelist::pushBack(uint64_t move) {
	movelist[index++] = move; 
}

void Movelist::print() {
	
}


void Movelist::swap(Movelist& m1, Movelist& m2) {
	uint64_t* ptr = m1.movelist; 
	m1.movelist = m2.movelist; 
	m2.movelist = ptr; 

	int temp = m1.index; 
	m1.index = m2.index; 
	m2.index = temp; 
}


/********************
*Generate Moves
*********************/
constexpr void Movelist::moveGen(const Board& board, const BoardState& boardState) {
	boardState.Get().side == white ? 
		Movelist::moveGenWhite(board, boardState) : 
		Movelist::moveGenBlack(board, boardState);
}

constexpr void Movelist::moveGenWhite(const Board& board, const BoardState& boardState) {
	//wPawn 

	//new pawn position 
	board.Get().piece[wPawn] << 8;
	//iterate through bits and get which one 
	//now to check for blocked pawns 

	//resulting white pawns 
	board.Get().piece[wPawn] << 8 & ~(board.Get().occupancy[white] | board.Get().occupancy[black]); 

	//can use this for double pushing pawns 
	(board.Get().piece[wPawn] << 8 & ~(board.Get().occupancy[white] | board.Get().occupancy[black])) << 8 & ~(board.Get().occupancy[white] | board.Get().occupancy[black]) & Row4;

	//non promotion 
	board.Get().piece[wPawn] << 8 & ~(board.Get().occupancy[white] | board.Get().occupancy[black]) & ~Row8; 

	//promotion 
	board.Get().piece[wPawn] << 8 & ~(board.Get().occupancy[white] | board.Get().occupancy[black]) & Row8;



	//now to iterate through both of those maps and fill into moveGen 

}

constexpr void Movelist::moveGenBlack(const Board& board, const BoardState& boardState) {

}




