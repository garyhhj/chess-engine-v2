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
	for (int i = 0; i < 64; ++i) {
		std::cout << std::endl; 
		std::cout << "pos: " << i << std::endl; 
		
		printBit(relevantBishopBlocker[i]); 

		std::cin >> s; 

	}




}