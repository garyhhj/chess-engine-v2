#include "macro.h"
#include "precalculated.h"

#include <iostream>
#include <bitset>
#include <map>

#include <vector>

std::vector<map> vs;

int main() {

	/*initPawnAttack();
	initKnightAttack();
	initKingAttack();*/


	//initRelevantBishopBlocker();
	//initBishopAttack(); 

	const int rookOccupancyCount[64] = {
	12, 11, 11, 11, 11, 11, 11, 12,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	12, 11, 11, 11, 11, 11, 11, 12,
	};

	initRelevantRookBlocker();


	const map occ = E4; 
	std::string s; 


	for (int i = 0; i < 64; ++i) {
		printBit(initRookAttackRunTime(indexSquare[i], occ)); 
		std::cin >> s; 
	}


	//now need to init rook run time 

	//generatingMagicNumBishop(); 
	


}