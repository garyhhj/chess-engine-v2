#pragma once
#include "macro.h"
#include <fstream>
#include <string>
#include <iostream>

/********************
*
*	Pawn
*
*********************/

namespace pawn{
	map pawnAttack[2][64]; //pawnAttack[side][index]

	//function to initialize 
	void initPawnAttack() {
		//init wPawn attack 
		for (int i = 0; i < 64; ++i) {
			const map pos = indexSquare[i];

			if (pos & ~AFile) pawnAttack[white][i] |= pos << 9;  //left
			if (pos & ~HFile) pawnAttack[white][i] |= pos << 7;  //right 
		}

		//init bPawn attack 
		for (int i = 0; i < 64; ++i) {
			const map pos = indexSquare[i];

			if (pos & ~AFile) pawnAttack[black][i] |= pos >> 7; //left  
			if (pos & ~HFile) pawnAttack[black][i] |= pos >> 9; //right
		}
	}
}

/********************
*
*	Knight
*
*********************/

namespace knight {
	map knightAttack[64]; //knightAttack[index]

	void initKnightAttack() {

		for (int i = 0; i < 64; ++i) {
			const map pos = indexSquare[i];

			//up 
			if (pos & ~AFile) knightAttack[i] |= pos << 17; //left  
			if (pos & ~HFile) knightAttack[i] |= pos << 15; //right 

			//down 
			if (pos & ~AFile) knightAttack[i] |= pos >> 15; //left
			if (pos & ~HFile) knightAttack[i] |= pos >> 17; //right 

			//left 
			if (pos & ~AFile & ~BFile) knightAttack[i] |= pos << 10; //up 
			if (pos & ~AFile & ~BFile) knightAttack[i] |= pos >> 6; //down

			//right 
			if (pos & ~GFile & ~HFile) knightAttack[i] |= pos << 6; //up  
			if (pos & ~GFile & ~HFile) knightAttack[i] |= pos >> 10; //down  

		}
	}
}

/********************
*
*	King
*
*********************/

namespace king {
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

namespace bishop {

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

		map visitedReference[512]{};
		map attackMapReference[512]{};
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

		map magicNums[64]{};
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
}


/********************
*
*	Rook
*
*********************/

namespace rook {

	uint64_t rookAttack[64][4096]; //rookAttack[index][magicIndex]
	uint64_t relevantRookBlocker[64];

	//number taken from v1 
	const map rookMagicNum[64] = {
		0x1010549228402,
	0x2000011002080084, 0x11004802040005, 0x406002014300842, 0x104a00080440a012, 0x2001200095004009, 0x882004883210112, 0x100c020800417, 0x41048c0200,
	0x4106002144080600, 0x800a800a00040080, 0x426000420109a00, 0x8000680310008080, 0x822008060401600, 0x406400088200080, 0x202208001104100, 0x501224008e460001,
	0x4802010802640010, 0x400102040080104, 0x2011001008030004, 0x1001009000090020, 0x201e420080320020, 0x9869008040010020, 0x810308040028008, 0x220081020001c4,
	0x2000101704000a08, 0x216000802009004, 0x28820400800800, 0x2012002046001008, 0x60450231002000, 0x405024001002080, 0x800400030800084, 0x5060010824c,
	0x242100010012003c, 0x4800c0080120080, 0x9004040080080081, 0x1200b00100100, 0x800910100402000, 0x24a00040100040, 0x8500802080024000, 0x202020004004087,
	0x440021702248, 0x540080800c000200, 0x2088008004028088, 0x808008001002, 0x828020053000, 0x802000c0100222c0, 0x281808004c00020, 0x8006000225009044,
	0x9005001402000100, 0x40800600440080, 0x24808084000800, 0x200801002802800, 0xa2002056008042, 0x1003040008101, 0x401002080010240, 0x280104063000480,
	0x9000c0200058300, 0x400881020040012, 0x4580080007801400, 0xc800c0800100082, 0x80200080900088, 0x840002000405008, 0x80014000802034,
	};


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

	//todo: find bug in generating magic number 
	uint64_t generatingMagicNumAtIndexRook(int index) {

		Magic magic;
		const map rookBlocker = relevantRookBlocker[index];
		const int rookBlockerBits = getNumBit(relevantRookBlocker[index]);

		map visitedReference[4096]{};
		map attackMapReference[4096]{};
		//precalculate some values for comparions later 
		for (int i = 0; i < (0x1 << rookBlockerBits); ++i) {
			visitedReference[i] = magic.mapCombination(i, rookBlocker);
			attackMapReference[i] = initRookAttackRunTime(indexSquare[index], visitedReference[i]);
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

		map magicNums[64]{};
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


	void initRookAttack() {
		//initRelevantBishopBlocker must be called before this function 
		//bishopAttack[index][magicindex]

		Magic magic;

		for (int index = 0; index < 64; ++index) {
			const map blockers = relevantRookBlocker[index];
			const int numBlockerBits = getNumBit(relevantRookBlocker[index]);

			//iterate through combinations 
			for (int i = 0; i < (0x1 << numBlockerBits); ++i) {
				const map combination = magic.mapCombination(i, blockers);
				int magicIndex = (combination * rookMagicNum[index]) >> (64 - numBlockerBits);

				//sanity check for correct magic number generation 
				const map attackMap = initRookAttackRunTime(indexSquare[index], combination);
				if (rookAttack[index][magicIndex] == 0x0 || rookAttack[index][magicIndex] == attackMap) {
					//success 
					rookAttack[index][magicIndex] = attackMap;
				}
				else {
					std::cout << "conflicting index at index: " << index << std::endl;
					return;
				}
			}

		}
	}
}

/********************
*
*	 
*
*********************/


/********************
*
*	printing
*
*********************/

namespace precalculation {

	//prints preinitialized array into txt (to be copied into another file for compile time initialization)
	std::string T = "static constexpr uint64_t"; 
	std::string P = "[]";	
	std::string Space = "\n\n"; 
	void print() {

		std::fstream fstr("precalculated.txt"); 

		//pawn 
		fstr << T << " pawnAttack[2][64] = \n";
		fstr << "{ "; 
		for (int side = 0; side < 2; ++side) {
			fstr << "{"; 
			for (int index = 0; index < 64; ++index) {
				fstr << "0x" << std::hex << pawn::pawnAttack[side][index] << ", ";
				if (!(index % 16)) fstr << "\n"; 
			}
			fstr << "}, \n"; 
		}
		fstr << "};\n";
		fstr << Space; 

		//knight 
		fstr << T << " knightAttack[64] = \n"; 
		fstr << "{ "; 
		for (int index = 0; index < 64; ++index) {
			fstr << "0x" << std::hex << knight::knightAttack[index] << ", "; 
			if (!(index % 16)) fstr << "\n"; 
		}
		fstr << "};\n";
		fstr << Space; 

		//king 
		fstr << T << " kingAttack[64] = \n"; 
		fstr << "{ "; 
		for (int index = 0; index < 64; ++index) {
			fstr << "0x" << std::hex << king::kingAttack[index] << ", "; 
			if (!(index % 16)) fstr << "\n";
		}
		fstr << "};\n"; 
		fstr << Space; 

	}


}