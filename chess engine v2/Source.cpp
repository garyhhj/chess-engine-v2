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
	int arr[64]; 
	for (int i = 0; i < 64; ++i) {
		std::cout << std::endl; 
		std::cout << "pos: " << i << std::endl; 
		
		arr[i] = getNumBit(relevantBishopBlocker[i]);
		printBit(relevantBishopBlocker[i]); 

		//std::cin >> s; 

	}

	for (int i = 0; i < 64; ++i) {
		if (arr[i] != bishopOccupancyCount[i]) {
			std::cout << "something is wrong at index: " << i << std::endl; 
		}
	}
	std::cout << "finsihed" << std::endl; 




}