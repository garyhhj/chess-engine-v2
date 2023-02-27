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
	//generatingMagicNumBishop(); 
	//initBishopAttack(); 


	initRelevantRookBlocker();
	//generatingMagicNumRook(); 
	initRookAttack(); 


	//now need to init rook run time 

	 


}