#pragma once
#include "macro.h"

/********************
*
*	Pawn
*
*********************/

//precalculated pawn attack table 
map pawnAttack[2][64]; 

/*
//function to initialize 
void initPawnAttack() {
	//init wPawn attack 
	for (int i = 0; i < 64; ++i) {
		const map pos = indexSquare[i]; 

		pawnAttack[white][i] |= pos << 9; 
		pawnAttack[white][i] |= pos << 7; 
	}

	//init bPawn attack 
	for (int i = 0; i < 64; ++i) {
		const map pos = indexSquare[i]; 

		pawnAttack[black][i] |= pos >> 7; 
		pawnAttack[black][i] |= pos >> 9; 
	}
}
*/



/********************
*
*	Knight
*
*********************/

/********************
*
*	King
*
*********************/

/********************
*
*	Bishop 
*
*********************/

/********************
*
*	Rook
*
*********************/

/********************
*
*	Queen? 
*
*********************/


