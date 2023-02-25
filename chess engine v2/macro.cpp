
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
*	Bit Macro
*
*********************/



constexpr bool getBit(const uint64_t bitBoard, const int index) {
	return bitBoard & (0x8000000000000000 >> index);
}

constexpr void setBit(uint64_t& bitBoard, const uint64_t square) {
	//bitBoard |= (0x8000000000000000 >> square);
	bitBoard |= square;
}

constexpr void popBit(uint64_t& bitBoard, const uint64_t square) {
	//bitBoard &= ~(0x8000000000000000 >> square);
	bitBoard &= ~square;
}

constexpr uint64_t lsbBit(const uint64_t bitBoard) {
	return bitBoard & ~bitBoard + 1;
}

constexpr int getNumBit(uint64_t bitboard) {
	int count = 0;

	while (bitboard) {
		++count;

		bitboard &= bitboard - 1;
	}
	return count;
}

constexpr int lsbBitIndex(uint64_t bitboard) {

	//make sure bitboard is not zero 
	if (bitboard) {
		return 63 - getNumBit((bitboard & (~bitboard + 1)) - 1);
	}

	else {
		//invalid index 
		return -1;
	}
}


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

const bit indexSquare[64] = {
	A8, B8, C8, D8, E8, F8, G8, H8,
	A7, B7, C7, D7, E7, F7, G7, H7,
	A6, B6, C6, D6, E6, F6, G6, H6,
	A5, B5, C5, D5, E5, F5, G5, H5,
	A4, B4, C4, D4, E4, F4, G4, H4,
	A3, B3, C3, D3, E3, F3, G3, H3,
	A2, B2, C2, D2, E2, F2, G2, H2,
	A1, B1, C1, D1, E1, F1, G1, H1,
};

/*
enum side : const int {
	white = 0,
	black = 1,
	both = 2,
};*/
