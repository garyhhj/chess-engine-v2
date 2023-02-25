#pragma once
#include "macro.h"

/********************
*
*	Pawn
*
*********************/

map pawnAttack[2][64]; //pawnAttack[side][index]

//function to initialize 
void initPawnAttack() {
	//init wPawn attack 
	for (int i = 0; i < 64; ++i) {
		const map pos = indexSquare[i]; 
		
		if(pos & ~AFile) pawnAttack[white][i] |= pos << 9;  //left
		if(pos & ~HFile) pawnAttack[white][i] |= pos << 7;  //right 
	}

	//init bPawn attack 
	for (int i = 0; i < 64; ++i) {
		const map pos = indexSquare[i]; 

		if(pos & ~AFile) pawnAttack[black][i] |= pos >> 7; //left  
		if(pos & ~HFile) pawnAttack[black][i] |= pos >> 9; //right
	}
}




/********************
*
*	Knight
*
*********************/

map knightAttack[64]; //knightAttack[index]

void initKnightAttack() {

	for (int i = 0; i < 64; ++i) {
		const map pos = indexSquare[i]; 

		//up 
		if(pos & ~AFile) knightAttack[i] |= pos << 17; //left  
		if(pos & ~HFile) knightAttack[i] |= pos << 15; //right 

		//down 
		if(pos & ~AFile) knightAttack[i] |= pos >> 15; //left
		if(pos & ~HFile) knightAttack[i] |= pos >> 17; //right 

		//left 
		if(pos & ~AFile & ~BFile) knightAttack[i] |= pos << 10; //up 
		if(pos & ~AFile & ~BFile) knightAttack[i] |= pos >> 6; //down

		//right 
		if(pos & ~GFile & ~HFile) knightAttack[i] |= pos << 6; //up  
		if(pos & ~GFile & ~HFile) knightAttack[i] |= pos >> 10; //down  

	}
}


/********************
*
*	King
*
*********************/

map kingAttack[64]; 

void initKingAttack() {
	
	for (int i = 0; i < 64; ++i) {
		const map pos = indexSquare[i]; 

		//up down 
		kingAttack[i] |= pos << 8; 
		kingAttack[i] |= pos >> 8; 
		
		//left right 
		if (pos & ~AFile) kingAttack[i] |= pos << 1;
		if (pos & ~HFile) kingAttack[i] |= pos >> 1;

		// '/' diagonal 
		if (pos & ~HFile) kingAttack[i] |= pos << 7; 
		if (pos & ~AFile) kingAttack[i] |= pos >> 7; 

		// '\' diagonal
		if (pos & ~AFile) kingAttack[i] |= pos << 9; 
		if (pos & ~HFile) kingAttack[i] |= pos >> 9; 

	}
}



/********************
*
*	Magic 
*
*********************/

class Magic {
	
public: 
	
	//generate candidate magic number 
	uint64_t generateMagicNumCandidate() {
		return generateRandomUint64() & generateRandomUint64() & generateRandomUint64();
	}

	//bitmask combination based on num 
	uint64_t mapCombination(int num, uint64_t occupancy) {
		int numBits = getNumBit(occupancy); 
		
		uint64_t res = 0x0ull; 

		int currBit = 0x1; 
		while (occupancy) {
			uint64_t lsbBit = getLsbBit(occupancy); 

			if (currBit & num) {
				res |= lsbBit; 
			}

			currBit <<= 1; 
			occupancy &= ~lsbBit; 
		}

		return res; 
	}
	
private: 
	uint64_t generateRandomUint64() {
		//defining numbers; 
		uint64_t num1, num2, num3, num4;

		num1 = generateRandomUint32() & 0xffff; 
		num2 = generateRandomUint32() & 0xffff;
		num3 = generateRandomUint32() & 0xffff;
		num4 = generateRandomUint32() & 0xffff;

		return num1 | num2 << 16 | num3 << 32 | num4 << 48;
	}

	static uint32_t generateRandomUint32() {
		//defining next state 
		uint32_t nxtState = magicNumberState;

		//xor shifts 
		nxtState ^= nxtState << 13;
		nxtState ^= nxtState >> 17;
		nxtState ^= nxtState << 5;

		//storing next state 
		magicNumberState = nxtState;

		return nxtState;
	}

	inline static uint32_t magicNumberState = 1804289383;

};

/********************
*
*	Bishop 
*
*********************/

//to generate all combinations, not very practical to store so just bitmask the necessary bits 

//first need to generate all relevent bishop bits 

uint64_t bishopAttack[64][512]; //bishopAttack[index][magicIndex]
uint64_t relevantBishopBlocker[64];

constexpr void initRelevantBishopBlocker() {
	
	for (int i = 0; i < 64; ++i) {

		const map pos = indexSquare[i]; 

		// '/' diagonal 
		for (int step = 1;
			pos << step * 7 & ~Edge; 
			++step) {
			relevantBishopBlocker[i] |= pos << (step * 7); 
		}

		for (int step = 1;
			pos >> step * 7 & ~Edge;
			++step) {
			relevantBishopBlocker[i] |= pos >> (step * 7); 
		}


		// '\' diagonal 
		for (int step = 1;
			pos << step * 9 & ~Edge;
			++step) {
			relevantBishopBlocker[i] |= pos << (step * 9); 
		}

		for (int step = 1;
			pos >> step * 9 & ~Edge;
			++step) {
			relevantBishopBlocker[i] |= pos >> (step * 9); 
		}
	}
}

//return attack map given occupancy 
constexpr uint64_t initBishopAttackRunTime(const uint64_t pos, const uint64_t occ) {

	uint64_t res = 0x0ull; 

	// '/' diagonal 
	for (int step = 1;
		pos << step * 7 & ~Edge;
		++step) {
		res |= pos << (step * 7);
		if ((pos << (step * 7)) & occ) break; 
	}

	for (int step = 1;
		pos >> step * 7 & ~Edge;
		++step) {
		res |= pos >> (step * 7);
		if ((pos >> (step * 7)) & occ) break; 
	}


	// '\' diagonal 
	for (int step = 1;
		pos << step * 9 & ~Edge;
		++step) {
		res |= pos << (step * 9);
		if ((pos << (step * 9)) & occ)  break; 
	}

	for (int step = 1;
		pos >> step * 9 & ~Edge;
		++step) {
		res |= pos >> (step * 9);
		if ((pos >> (step * 9)) & occ) break; 
	}

	return res; 
}

uint64_t testingMagicNumAtIndex(int index) {

	Magic magic;
	const map bishopBlocker = relevantBishopBlocker[index];
	const int bishopBlockerBits = getNumBit(relevantBishopBlocker[index]);

	map visitedReference[512]{};
	//precalculate some values for comparions later 
	for (int i = 0; i < (0x1 << bishopBlockerBits); ++i) {
		visitedReference[i] = magic.mapCombination(i, bishopBlocker);
	}


	for (int trial = 0; trial < 10000000; ++trial) {

		bool validNum = true; 
		map visited[512] = { 0x0ull };
			
		//generate a magic number 
		const map magicNumCandidate = magic.generateMagicNumCandidate(); 

		//iterate through combination of blocker 
		for (int i = 0; i < (0x1 << bishopBlockerBits); ++i) {
			const map combination = magic.mapCombination(i, bishopBlocker);

			int magicIndex = (combination * magicNumCandidate) >> (64 - bishopBlockerBits);

			if (visited[magicIndex] == 0x0 || visited[magicIndex] == visitedReference[i]) {
				visited[magicIndex] == visitedReference[i]; 
			}
			else {
				validNum = false;
				break; 
			}
		}

		if (validNum) return magicNumCandidate;
		else continue; 
	}

	//invalid magicNum 
	return 0x0; 
}

void testingMagicNumBishop() {

	map magicNums[64];
	for (int i = 0; i < 64; ++i) {
		map validMagicNumber = testingMagicNumAtIndex(i);
		if (validMagicNumber == 0x0) {
			std::cout << "Didn't find valid magic number at index: " << i << std::endl;
			break;
		}
		else {
			magicNums[i] = validMagicNumber;
		}
	}

	//print out magic number 
	for (int i = 0; i < 64; ++i) {
		std::cout << std::hex << "0x" << magicNums[i] << ",\n";
	}
}
/********************
*
*	Rook
*
*********************/

/********************
*
*	Queen? 
*
*********************/


