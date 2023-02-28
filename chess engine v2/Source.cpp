#include "macro.h"
#include "precalculated.h"

#include <iostream>
#include <bitset>
#include <map>

#include <vector>





int main() {

	pawn::initPawnAttack();
	knight::initKnightAttack();
	king::initKingAttack();


	bishop::initRelevantBishopBlocker();
	//generatingMagicNumBishop(); 
	bishop::initBishopAttack(); 


	rook::initRelevantRookBlocker();
	//generatingMagicNumRook(); 
	rook::initRookAttack(); 


	precalculation::print(); 

	//now need to init rook run time 

	 
	
	//todo: 

	//initialize array at compile time using lambda 
	
	//parsing fen 
	
	//check what gigantua did 
	//board state 
	//chess board 


}