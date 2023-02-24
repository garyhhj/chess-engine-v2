#pragma once
#include "macro.h"

/********************
*
*	Pawn
*
*********************/

//precalculated pawn attack table 
map pawnAttack[2][64]; 

//function to initialize 
void initPawnAttack() {
	//init wPawn attack 
	for (int i = 0; i < 64; ++i) {
		const map pos = indexSquare[i]; 

		
		if(pos & ~AFile) pawnAttack[white][i] |= pos << 9;  //left
		if(pos & ~HFile) pawnAttack[white][i] |= pos << 7;  //right 
	}

	//init bPawn attack 
	for (int i = 0; i < 64; ++i) {
		const map pos = indexSquare[i]; 

		if(pos & ~AFile) pawnAttack[black][i] |= pos >> 7; //left  
		if(pos & ~HFile) pawnAttack[black][i] |= pos >> 9; //right
	}
}




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


