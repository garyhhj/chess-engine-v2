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

uint64_t bishopAttack[64][512]; //bishopAttack[index][magicIndex]
uint64_t relevantBishopBlocker[64];
const map bishopMagicNum[] = { 0x40040822862081,
0xa168041010224880, 0x400102020020885, 0x212001220042403, 0x20900041108c, 0x400000049082100, 0x111110105100308, 0x22010108410402, 0x200c100228c10a80,
0x91020885040090, 0x80000c88900d0000, 0x2003002020400, 0x800e100284140002, 0x6311401040228, 0x2040440209102440, 0x1102923002200165, 0x1011020082020910,
0x8002080827804118, 0x44008082022100, 0x400812000041, 0x2000040c1811e404, 0x80a2940208000101, 0x2401008250002000, 0x34042a40048840, 0x1040028010500,
0x40100204c8121100, 0x881010200040a02, 0x40008020020260, 0x8008a080a1680200, 0x2010242100081, 0x8582400080500, 0xc224202004040400, 0x1824009005220100,
0x4004084641011041, 0x88004002011580, 0x840080802000, 0x42011000802400c, 0x1204100009104080, 0x1010010a020641, 0x20210124084600, 0xc01100020103c208,
0x2133000411821020, 0x10200202102200, 0x8021004820180000, 0x4009204404008019, 0x4018102100c40080, 0x340c6001040104, 0x9009020010c00, 0x2440908401081209,
0x48089808088404, 0x1220110080465, 0x408020210580380, 0x45c00800200, 0x1080801002004, 0x8002102c11004201, 0x81301010030940, 0x8c02008200901400,
0x2081a23101154, 0xa410900420520809, 0x4004242000030048, 0x204241a80000420, 0x211802044e048220, 0x310340810882000, 0x221204040c004600,
};

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
		pos << step * 7 & ~AFile && pos << step * 7 & ~Row1;
		++step) {
		res |= pos << (step * 7);
		if ((pos << (step * 7)) & occ) break; 
	}

	for (int step = 1;
		pos >> step * 7 & ~HFile && pos >> step * 7 & ~Row8;
		++step) {
		res |= pos >> (step * 7);
		if ((pos >> (step * 7)) & occ) break; 
	}


	// '\' diagonal 
	for (int step = 1;
		pos << step * 9 & ~HFile && pos << step * 9 & ~Row1;
		++step) {
		res |= pos << (step * 9);
		if ((pos << (step * 9)) & occ)  break; 
	}

	for (int step = 1;
		pos >> step * 9 & ~AFile && pos >> step * 9 & ~Row8;
		++step) {
		res |= pos >> (step * 9);
		if ((pos >> (step * 9)) & occ) break; 
	}

	return res; 
}

uint64_t generatingMagicNumAtIndexBishop(int index) {

	Magic magic;
	const map bishopBlocker = relevantBishopBlocker[index];
	const int bishopBlockerBits = getNumBit(relevantBishopBlocker[index]);

	map visitedReference[512];
	map attackMapReference[512]; 
	//precalculate some values for comparions later 
	for (int i = 0; i < (0x1 << bishopBlockerBits); ++i) {
		visitedReference[i] = magic.mapCombination(i, bishopBlocker);
		attackMapReference[i] = initBishopAttackRunTime(indexSquare[index], visitedReference[i]); 
	}


	for (int trial = 0; trial < 10000000; ++trial) {

		bool validNum = true; 
		map visited[512];
		for (int count = 0; count < 512; ++count) visited[count] = 0x0ull; 
			
		//generate a magic number 
		const map magicNumCandidate = magic.generateMagicNumCandidate(); 

		//iterate through combination of blocker 
		for (int i = 0; i < (0x1 << bishopBlockerBits); ++i) {
			const map combination = magic.mapCombination(i, bishopBlocker);
			int magicIndex = (combination * magicNumCandidate) >> (64 - bishopBlockerBits);

			if (visited[magicIndex] == 0x0) {
				visited[magicIndex] = attackMapReference[i];
			}
			else if (visited[magicIndex] == attackMapReference[i]) {
				//do nothing  
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
	return 0x0ull; 
}

void generatingMagicNumBishop() {

	map magicNums[64];
	for (int i = 0; i < 64; ++i) {
		map validMagicNumber = generatingMagicNumAtIndexBishop(i);
		if (validMagicNumber == 0x0ull) {
			std::cout << "Didn't find valid magic number at index: " << i << std::endl;
			break;
		}
		else {
			magicNums[i] = validMagicNumber;
		}
	}

	//print out magic number 
	for (int i = 0; i < 64; ++i) {
		std::cout << std::hex << "0x" << magicNums[i] << ", ";
		if (!(i % 8)) std::cout << "\n";
	}
}

void initBishopAttack() {
	//initRelevantBishopBlocker must be called before this function 
	//bishopAttack[index][magicindex]

	Magic magic; 

	for (int index = 0; index < 64; ++index) {
		const map blockers = relevantBishopBlocker[index]; 
		const int numBlockerBits = getNumBit(relevantBishopBlocker[index]); 
	
		//iterate through combinations 
		for (int i = 0; i < (0x1 << numBlockerBits); ++i) {
			const map combination = magic.mapCombination(i, blockers); 
			int magicIndex = (combination * bishopMagicNum[index]) >> (64 - numBlockerBits); 

			//sanity check for correct magic number generation 
			const map attackMap = initBishopAttackRunTime(indexSquare[index], combination); 
			if (bishopAttack[index][magicIndex] == 0x0 || bishopAttack[index][magicIndex] == attackMap) {
				//success 
				bishopAttack[index][magicIndex] = attackMap; 
			}
			else {
				std::cout << "conflicting index at index: " << index << std::endl; 
				return; 
			}
		}
	
	}
}

/********************
*
*	Rook
*
*********************/

uint64_t rookAttack[64][4096]; //rookAttack[index][magicIndex]
uint64_t relevantRookBlocker[64];
const map rookMagicNum[64] = {
	0x60800004203010,
0x2000380201000008, 0x4409008012, 0x402002305c080008, 0x10c4048800000082, 0x80080000000, 0x30c140024400080, 0x801000802000, 0x8001810200420,
0x780a080080080056, 0x204800e1010000, 0x40105202020100, 0x8002a200024200, 0x244440108208000, 0x4010010000080001, 0xc10140000002840, 0x8000000800a400,
0x480040511122002, 0x128000100040, 0x9002041009000030, 0x108800020040010, 0x50000060400300, 0x2000041320800210, 0x2000630420000000, 0x5400880202010800,
0x8000805c00400000, 0x10020480e410000, 0x10480100226009a8, 0x10040000000400, 0x8000488008200080, 0x8000850000040440, 0x240420005810, 0x9300070000204002,
0x1000430100100082, 0x1001400000140840, 0x88c44004002000, 0x1208000001, 0x808008040002020, 0x500000280004004, 0x1a04002000001043, 0x9080540080300301,
0x40a00200840082, 0x80402000100880, 0x3000a000010000, 0x801a0105000424, 0x2000100000402, 0x824c220000060680, 0x12000018200000a0, 0x6008011004000800,
0x40180210122000c2, 0x5000500208000008, 0x42082000000030a, 0x4441004002904008, 0xa000c444a0925000, 0x400090400c004181, 0x2000020800220040, 0x206300a200c50600,
0xac000030000082, 0x4080000040000800, 0x40801910d4004, 0x2008416400210, 0x8b40800050200, 0x1020210000040180, 0x102000028023100,
}


constexpr void initRelevantRookBlocker() {


	for (int i = 0; i < 64; ++i) {

		const map pos = indexSquare[i];
		
		//up and down 
		for (int step = 1; pos << 8 * step & ~Row8; ++step) {
			relevantRookBlocker[i] |= pos << 8 * step; 
		}

		for (int step = 1; pos >> 8 * step & ~Row1; ++step) {
			relevantRookBlocker[i] |= pos >> 8 * step; 
		}


		//left anf right 
		for (int step = 1; pos << step & ~AFile && pos << step & ~HFile; ++step) {
			relevantRookBlocker[i] |= pos << step; 
		}
		for (int step = 1; pos >> step & ~HFile && pos >> step & ~AFile; ++step) {
			relevantRookBlocker[i] |= pos >> step; 
		}
		
	}
}


//return attack map given occupancy 
constexpr uint64_t initRookAttackRunTime(const uint64_t pos, const uint64_t occ) {

	uint64_t res = 0x0ull;


	//up and down 
	for (int step = 1; 8 * step != 64 && pos << 8 * step; ++step) {
		res |= pos << 8 * step;
		if (pos << 8 * step & occ) break;
	}
	for (int step = 1; 8 * step != 64 && pos >> 8 * step; ++step) {
		res |= pos >> 8 * step;
		if (pos >> 8 * step & occ) break;
	}

	//left and right 
	for (int step = 1; pos << step & ~HFile; ++step) {
		res |= pos << step;
		if (pos << step & occ) break;
	}
	for (int step = 1; pos >> step & ~AFile; ++step) {
		res |= pos >> step;
		if (pos >> step & occ) break;
	}

	return res;
}


uint64_t generatingMagicNumAtIndexRook(int index) {

	Magic magic;
	const map rookBlocker = relevantRookBlocker[index];
	const int rookBlockerBits = getNumBit(relevantRookBlocker[index]);

	map visitedReference[4096];
	map attackMapReference[4096];
	//precalculate some values for comparions later 
	for (int i = 0; i < (0x1 << rookBlockerBits); ++i) {
		visitedReference[i] = magic.mapCombination(i, rookBlocker);
		attackMapReference[i] = initBishopAttackRunTime(indexSquare[index], visitedReference[i]);
	}


	for (int trial = 0; trial < 10000000; ++trial) {

		bool validNum = true;
		map visited[4096];
		for (int count = 0; count < 4096; ++count) visited[count] = 0x0ull;

		//generate a magic number 
		const map magicNumCandidate = magic.generateMagicNumCandidate();

		//iterate through combination of blocker 
		for (int i = 0; i < (0x1 << rookBlockerBits); ++i) {
			const map combination = magic.mapCombination(i, rookBlocker);
			int magicIndex = (combination * magicNumCandidate) >> (64 - rookBlockerBits);

			if (visited[magicIndex] == 0x0) {
				visited[magicIndex] = attackMapReference[i];
			}
			else if (visited[magicIndex] == attackMapReference[i]) {
				//do nothing  
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
	return 0x0ull;
}

void generatingMagicNumRook() {

	map magicNums[64];
	for (int i = 0; i < 64; ++i) {
		map validMagicNumber = generatingMagicNumAtIndexRook(i);
		if (validMagicNumber == 0x0ull) {
			std::cout << "Didn't find valid magic number at index: " << i << std::endl;
			break;
		}
		else {
			magicNums[i] = validMagicNumber;
		}
	}

	//print out magic number 
	for (int i = 0; i < 64; ++i) {
		std::cout << std::hex << "0x" << magicNums[i] << ", ";
		if (!(i % 8)) std::cout << "\n";
	}
}


/********************
*
*	Queen? 
*
*********************/


