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
		std::cout << "new mask " << std::endl; 
		for (int i = 0; i < 0x1 << getNumBit(relevantBishopBlocker[j]); ++i) {

			map mask = a.mapCombination(i, relevantBishopBlocker[j]);
			printBit(mask);

			std::cin >> s; 
		}
	}
	




}