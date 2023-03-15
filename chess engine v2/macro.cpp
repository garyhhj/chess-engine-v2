
#include "macro.h"

/********************
*
*	Utility
*
*********************/

typedef uint64_t bit;
typedef uint64_t map;
typedef uint64_t board;


//forces compile time evaluation 
//error, e3160, nothing on stackoverflow and documentation

/*
consteval auto compileTime(auto val) {
	return val;
}
*/

consteval auto compileTime(auto val) {
	return val;
}

void printBit(const uint64_t bit) {
	std::cout << "\n";
	for (int row = 0; row < 8; ++row) {

		std::cout << 8 - row << "   ";

		for (int file = 0; file < 8; ++file) {

			const int index = row * 8 + file;
			if (bit & indexSquare[index]) std::cout << '1';
			else std::cout << '0';

			std::cout << " ";
		}
		std::cout << '\n';
	}
	std::cout << "\n";
	std::cout << "    a b c d e f g h";
	std::cout << "\n";

	std::flush(std::cout);
}



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

move Move::makemove(int sourceSquare, int targetSquare, int piece, int promotePiece, bool captureFlag, bool doublePushFlag, bool enpassantFlag, bool castlingFlag) {
	return (sourceSquare & 0x3f) | ((targetSquare << 6) & 0xfc0) | ((piece << 12) & 0xf000) | ((promotePiece << 16) & 0xf0000) |
		(captureFlag ? 0x1 << 20 : 0x0) | (doublePushFlag ? 0x1 << 21 : 0x0) | (enpassantFlag ? 0x1 << 22 : 0x0) | (castlingFlag ? 0x1 << 23 : 0x0);

}

move Move::makemove(uint64_t sourceSquare, uint64_t targetSquare, int piece, int promotePiece, bool captureFlag, bool doublePushFlag, bool enpassantFlag, bool castlingFlag) {
	return (getlsbBitIndex(sourceSquare) & 0x3f) | ((getlsbBitIndex(targetSquare) << 6) & 0xfc0) | ((piece << 12) & 0xf000) | ((promotePiece << 16) & 0xf0000) |
		(captureFlag ? 0x1 << 20 : 0x0) | (doublePushFlag ? 0x1 << 21 : 0x0) | (enpassantFlag ? 0x1 << 22 : 0x0) | (castlingFlag ? 0x1 << 23 : 0x0);
}
 
void Move::decode(const move m) {

	//sourceSquare 
	std::cout << "SS: ";
	//std::cout << Move::sourceSquare(m) << std::endl; 

	//this is wrong and needs to change 

	//A8 is 0
	//H1 is 63 
	std::cout << char('A' + Move::sourceSquare(m) % 8);
	std::cout << 8 - Move::sourceSquare(m) / 8 << " ";

	//target Square 
	std::cout << "TS: ";
	std::cout << char('A' + Move::targetSquare(m) % 8);
	std::cout << 8 - Move::targetSquare(m) / 8 << " ";

	//piece
	std::cout << pieceChar[Move::piece(m)] << " ";

	//promote piece 
	std::cout << pieceChar[Move::promotePiece(m)] << " ";

	//capture 
	std::cout << (Move::captureFlag(m) ? 'c' : '-');
	std::cout << " ";

	//double push 
	std::cout << (Move::doublePushFlag(m) ? "dp" : "--");
	std::cout << " ";

	//enpassant 
	std::cout << (Move::enpassantFlag(m) ? 'e' : '-');
	std::cout << " ";

	//castle 
	std::cout << (Move::castlingFlag(m) ? 'c' : '-');
	std::cout << " ";
}

constexpr int Move::sourceSquare(const move m) { return m & 0x3f; }
constexpr int Move::targetSquare(const move m) { return (m & 0xfc0) >> 6; }
constexpr int Move::piece(const move m) { return (m & 0xf000) >> 12; }
constexpr int Move::promotePiece(const move m) { return (m & 0xf0000) >> 16; }

constexpr bool Move::captureFlag(const move m) { return m & 0x100000; }
constexpr bool Move::doublePushFlag(const move m) { return m & 0x200000; }
constexpr bool Move::enpassantFlag(const move m) { return m & 0x400000; }
constexpr bool Move::castlingFlag(const move m) { return m & 0x800000; }

const std::string Move::moveString(const move m) {
	const int sourceSquare = Move::sourceSquare(m);
	const int targetSquare = Move::targetSquare(m); 
	const int promotePiece = Move::promotePiece(m); 

	std::string move = ""; 
	move += static_cast<char>(sourceSquare % 8 + 'a');
	move += static_cast<char>('8' - sourceSquare / 8);

	move += static_cast<char>(targetSquare % 8 + 'a');
	move += static_cast<char>('8' - targetSquare/ 8);

	if (promotePiece == wKnight || promotePiece == bKnight) move += 'k';
	else if (promotePiece == wBishop || promotePiece == bBishop) move += 'b';
	else if (promotePiece == wRook || promotePiece == bRook) move += 'r';
	else if (promotePiece == wQueen || promotePiece == wQueen) move += 'q'; 

	return move; 
}



/********************
*
*	Bit Macro
*
*********************/

//
//
//constexpr bool getBit(const uint64_t bitBoard, const int index) {
//	return bitBoard & (0x8000000000000000 >> index);
//}
//
//constexpr void setBit(uint64_t& bitBoard, const uint64_t square) {
//	//bitBoard |= (0x8000000000000000 >> square);
//	bitBoard |= square;
//}
//
//constexpr void popBit(uint64_t& bitBoard, const uint64_t square) {
//	//bitBoard &= ~(0x8000000000000000 >> square);
//	bitBoard &= ~square;
//}
//
//constexpr uint64_t getLsbBit(const uint64_t bitBoard) {
//	return bitBoard & ~bitBoard + 1;
//}
//
//constexpr int getNumBit(uint64_t bitboard) {
//	return std::popcount(bitboard); 
//	/*int count = 0; 
//	while (bitboard) {
//		++count;
//
//		bitboard &= bitboard - 1;
//	}
//	return count;*/
//}
//
//constexpr int getlsbBitIndex(uint64_t bitboard) {
//
//	//make sure bitboard is not zero 
//	if (bitboard) {
//		return 63 - getNumBit((bitboard & (~bitboard + 1)) - 1);
//	}
//
//	else {
//		//invalid index 
//		return -1;
//	}
//}


/********************
*
*	Enumerations
*
*********************/

/*
enum Squares : const bit {
	A8 = 0x8000000000000000ull,
	B8 = 0x4000000000000000ull,
	C8 = 0x2000000000000000ull,
	D8 = 0x1000000000000000ull,
	E8 = 0x0800000000000000ull,
	F8 = 0x0400000000000000ull,
	G8 = 0x0200000000000000ull,
	H8 = 0x0100000000000000ull,

	A7 = 0x0080000000000000ull,
	B7 = 0x0040000000000000ull,
	C7 = 0x0020000000000000ull,
	D7 = 0x0010000000000000ull,
	E7 = 0x0008000000000000ull,
	F7 = 0x0004000000000000ull,
	G7 = 0x0002000000000000ull,
	H7 = 0x0001000000000000ull,

	A6 = 0x0000800000000000ull,
	B6 = 0x0000400000000000ull,
	C6 = 0x0000200000000000ull,
	D6 = 0x0000100000000000ull,
	E6 = 0x0000080000000000ull,
	F6 = 0x0000040000000000ull,
	G6 = 0x0000020000000000ull,
	H6 = 0x0000010000000000ull,

	A5 = 0x0000008000000000ull,
	B5 = 0x0000004000000000ull,
	C5 = 0x0000002000000000ull,
	D5 = 0x0000001000000000ull,
	E5 = 0x0000000800000000ull,
	F5 = 0x0000000400000000ull,
	G5 = 0x0000000200000000ull,
	H5 = 0x0000000100000000ull,

	A4 = 0x0000000080000000ull,
	B4 = 0x0000000040000000ull,
	C4 = 0x0000000020000000ull,
	D4 = 0x0000000010000000ull,
	E4 = 0x0000000008000000ull,
	F4 = 0x0000000004000000ull,
	G4 = 0x0000000002000000ull,
	H4 = 0x0000000001000000ull,

	A3 = 0x0000000000800000ull,
	B3 = 0x0000000000400000ull,
	C3 = 0x0000000000200000ull,
	D3 = 0x0000000000100000ull,
	E3 = 0x0000000000080000ull,
	F3 = 0x0000000000040000ull,
	G3 = 0x0000000000020000ull,
	H3 = 0x0000000000010000ull,

	A2 = 0x0000000000008000ull,
	B2 = 0x0000000000004000ull,
	C2 = 0x0000000000002000ull,
	D2 = 0x0000000000001000ull,
	E2 = 0x0000000000000800ull,
	F2 = 0x0000000000000400ull,
	G2 = 0x0000000000000200ull,
	H2 = 0x0000000000000100ull,

	A1 = 0x0000000000000080ull,
	B1 = 0x0000000000000040ull,
	C1 = 0x0000000000000020ull,
	D1 = 0x0000000000000010ull,
	E1 = 0x0000000000000008ull,
	F1 = 0x0000000000000004ull,
	G1 = 0x0000000000000002ull,
	H1 = 0x0000000000000001ull,
}; */

//constexpr bit indexSquare[64] = {
//	A8, B8, C8, D8, E8, F8, G8, H8,
//	A7, B7, C7, D7, E7, F7, G7, H7,
//	A6, B6, C6, D6, E6, F6, G6, H6,
//	A5, B5, C5, D5, E5, F5, G5, H5,
//	A4, B4, C4, D4, E4, F4, G4, H4,
//	A3, B3, C3, D3, E3, F3, G3, H3,
//	A2, B2, C2, D2, E2, F2, G2, H2,
//	A1, B1, C1, D1, E1, F1, G1, H1,
//};

/*
enum side : const int {
	white = 0,
	black = 1,
	both = 2,
};*/

/*
enum piece : const int {
	wPawn = 0,
	wKnight = 1,
	wKing = 2,
	wBishop = 3,
	wRook = 4,
	wQueen = 5,

	bPawn = 6,
	bKnight = 7,
	bKing = 8,
	bBishop = 9,
	bRook = 10,
	bQueen = 11,
}; */

extern const char pieceChar[12] = {
	'P', 'N', 'K', 'B', 'R', 'Q',
	'p', 'n', 'k', 'b', 'r', 'q',
};
