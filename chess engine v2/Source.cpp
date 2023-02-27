#include "macro.h"
#include "precalculated.h"

#include <iostream>
#include <bitset>
#include <map>

#include <vector>

std::vector<map> vs;

int main() {

	pawn::initPawnAttack();
	knight::initKnightAttack();
	king::initKingAttack();


	initRelevantBishopBlocker();
	//generatingMagicNumBishop(); 
	initBishopAttack(); 


	initRelevantRookBlocker();
	//generatingMagicNumRook(); 
	initRookAttack(); 




	//now need to init rook run time 

	 
	
	//todo: 

	//initialize array at compile time using lambda 
	
	//parsing fen 
	
	//check what gigantua did 
	//board state 
	//chess board 


}