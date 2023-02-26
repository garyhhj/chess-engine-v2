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


	initRelevantBishopBlocker();
	initBishopAttack(); 


	//std::string s; 
	//for (int i = 0; i < 64; ++i) {
	//	printBit(initBishopAttackRunTime(indexSquare[i], B7));

	//	std::cin >> s; 
	//}

	

	//std::string s; 
	//Magic a; 

	//for (int j = 0; j < 64; ++j) {
	//	const bit pos = indexSquare[j];

	//	const bit relBlockers = relevantBishopBlocker[j]; 
	//	std::cout << "next position: " << j << std::endl; 
	//	//loop through combination of each blocker 
	//	int numBit = getNumBit(relBlockers); 
	//	for (int i = 0; i < numBit; ++i) {
	//		const bit comb = a.mapCombination(i, relBlockers); 

	//		const bit attackRt = initBishopAttackRunTime(pos, comb); 

	//		std::cout << "blockers, attacks:" << std::endl; 
	//		printBit(comb);
	//		printBit(attackRt); 
	//		std::cout << std::endl; 
	//	}


	//}
	//



	//generatingMagicNumBishop(); 
	


}