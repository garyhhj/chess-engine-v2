#include "macro.h"
#include "precalculated.h"

#include <iostream>
#include <bitset>


int main() {

	initPawnAttack(); 
	initKnightAttack(); 
	initKingAttack(); 


	std::string s; 
	for (int i = 0; i < 64; ++i) {
		std::cout << std::endl; 
		std::cout << "pos: " << i << std::endl; 
		
		printBit(kingAttack[i]); 

		std::cin >> s; 

	}

	
	



}