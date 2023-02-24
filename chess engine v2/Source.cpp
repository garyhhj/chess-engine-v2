#include "macro.h"
#include "precalculated.h"

#include <iostream>
#include <bitset>
#include <map>

int main() {

	initPawnAttack(); 
	initKnightAttack(); 
	initKingAttack(); 


	//std::string s; 
	//for (int i = 0; i < 64; ++i) {
	//	std::cout << std::endl; 
	//	std::cout << "pos: " << i << std::endl; 
	//	
	//	printBit(kingAttack[i]); 

	//	std::cin >> s; 

	//}


	Magic m; 
	std::map<uint64_t, int> cnt; 

	for (int i = 0; i < 100000; ++i) {

		cnt[m.generateMagicNumCandidate()]++; 
	}

	int count = 0; 
	for (auto it = cnt.begin(); it != cnt.end(); ++it) {
		if (it->second > 1) {
			++count; 
			std::cout << "noo" << std::endl; 
		}
	}

	std::cout << "count: " << count << std::endl; 
	std::cout << "finished" << std::endl; 
	



}