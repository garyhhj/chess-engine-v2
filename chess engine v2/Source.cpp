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


	std::string s; 

	for (int i = 0; i < 64; ++i) {
		printBit(relevantRookBlocker[i]); 
		if (getNumBit(relevantRookBlocker[i]) != rookOccupancyCount[i]) {
			std::cout << "oof something is wrong" << std::endl; 
		}

		std::cin >> s; 
	}



	//generatingMagicNumBishop(); 
	


}