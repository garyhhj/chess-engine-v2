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

void Move::decode() {
	//print out everything in order? 
	
	using namespace std;
	std::cout << "move: ";

	//sourceSquare 
	//std::cout << 'A' + 

	////sourceIndex
	//cout << "(SI)" << positionStr[decodeMoveSourceIndex(move)] << " ";

	////targetIndex 
	//cout << "(TI)" << positionStr[decodeMoveTargetIndex(move)] << " ";

	////piece 
	//cout << pieceStr[decodeMovePiece(move)] << " ";

	////padding 
	//int padding = 8 - pieceStr[decodeMovePiece(move)].size();
	//while (padding--) cout << " ";

	////promote piece 
	//cout << pieceStr[decodeMovePromotePiece(move)] << " ";

	////padding 
	//padding = 10 - pieceStr[decodeMovePromotePiece(move)].size();
	//while (padding--) cout << " ";
	//cout << "| ";

	////capture 
	//if (decodeMoveCapture(move)) cout << "c ";
	//else cout << "- ";

	////double push 
	//if (decodeMoveDoublePush(move)) {
	//	cout << "dp ";
	//}
	//else cout << "-- ";

	////enpassant 
	//if (decodeMoveEnpassant(move)) cout << "e ";
	//else cout << "- ";

	////castle 
	//if (decodeMoveCastle(move)) cout << "c";
	//else cout << "-";

	//cout << '\n';
}



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
