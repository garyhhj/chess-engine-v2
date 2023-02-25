#include "macro.h"
#include "precalculated.h"

#include <iostream>
#include <bitset>
#include <map>

int main() {

	initPawnAttack();
	initKnightAttack();
	initKingAttack();

	initRelevantBishopBlocker();

	std::string s; 
	Magic a; 

	for (int j = 0; j < 64; ++j) {
		const bit pos = indexSquare[j];

		const bit relBlockers = relevantBishopBlocker[j]; 
		std::cout << "next position: " << j << std::endl; 
		//loop through combination of each blocker 
		int numBit = getNumBit(relBlockers); 
		for (int i = 0; i < numBit; ++i) {
			const bit comb = a.mapCombination(i, relBlockers); 

			const bit attackRt = initBishopAttackRunTime(pos, comb); 

			std::cout << "blockers, attacks:" << std::endl; 
			printBit(comb);
			printBit(attackRt); 
			std::cout << std::endl; 
		}


	}
	




}