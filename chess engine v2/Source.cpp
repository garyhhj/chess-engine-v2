#include "macro.h"
#include "precalculated.h"

#include <iostream>
#include <bitset>
#include <map>

#include <vector>



static constexpr uint64_t kingAttack[64] =
{ 0x40c0000000000000,
0xa0e0000000000000, 0x5070000000000000, 0x2838000000000000, 0x141c000000000000, 0xa0e000000000000, 0x507000000000000, 0x203000000000000, 0xc040c00000000000, 0xe0a0e00000000000, 0x7050700000000000, 0x3828380000000000, 0x1c141c0000000000, 0xe0a0e0000000000, 0x705070000000000, 0x302030000000000, 0xc040c000000000,
0xe0a0e000000000, 0x70507000000000, 0x38283800000000, 0x1c141c00000000, 0xe0a0e00000000, 0x7050700000000, 0x3020300000000, 0xc040c0000000, 0xe0a0e0000000, 0x705070000000, 0x382838000000, 0x1c141c000000, 0xe0a0e000000, 0x70507000000, 0x30203000000, 0xc040c00000,
0xe0a0e00000, 0x7050700000, 0x3828380000, 0x1c141c0000, 0xe0a0e0000, 0x705070000, 0x302030000, 0xc040c000, 0xe0a0e000, 0x70507000, 0x38283800, 0x1c141c00, 0xe0a0e00, 0x7050700, 0x3020300, 0xc040c0,
0xe0a0e0, 0x705070, 0x382838, 0x1c141c, 0xe0a0e, 0x70507, 0x30203, 0xc040, 0xe0a0, 0x7050, 0x3828, 0x1c14, 0xe0a, 0x705, 0x302, };



int main() {

	pawn::initPawnAttack();
	knight::initKnightAttack();
	king::initKingAttack();


	bishop::initRelevantBishopBlocker();
	//generatingMagicNumBishop(); 
	bishop::initBishopAttack(); 


	rook::initRelevantRookBlocker();
	//generatingMagicNumRook(); 
	rook::initRookAttack(); 


//	precalculation::print(); 

	std::string s; 
	for (int i = 0; i < 64; ++i) {
		printBit(kingAttack[i]); 
		std::cin >> s; 
		
	}

	//now need to init rook run time 

	 
	
	//todo: 

	//initialize array at compile time using lambda 
	
	//parsing fen 
	
	//check what gigantua did 
	//board state 
	//chess board 


}