#pragma once
#include "macro.h"

/********************
*
*	Pawn
*
*********************/

map pawnAttack[2][64]; //pawnAttack[side][index]

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

map knightAttack[64]; //knightAttack[index]

void initKnightAttack() {

	for (int i = 0; i < 64; ++i) {
		const map pos = indexSquare[i]; 

		//up 
		if(pos & ~AFile) knightAttack[i] |= pos << 17; //left  
		if(pos & ~HFile) knightAttack[i] |= pos << 15; //right 

		//down 
		if(pos & ~AFile) knightAttack[i] |= pos >> 15; //left
		if(pos & ~HFile) knightAttack[i] |= pos >> 17; //right 

		//left 
		if(pos & ~AFile & ~BFile) knightAttack[i] |= pos << 10; //up 
		if(pos & ~AFile & ~BFile) knightAttack[i] |= pos >> 6; //down

		//right 
		if(pos & ~GFile & ~HFile) knightAttack[i] |= pos << 6; //up  
		if(pos & ~GFile & ~HFile) knightAttack[i] |= pos >> 10; //down  

	}
}


/********************
*
*	King
*
*********************/

map kingAttack[64]; 

void initKingAttack() {
	
	for (int i = 0; i < 64; ++i) {
		const map pos = indexSquare[i]; 

		//up down 
		kingAttack[i] |= pos << 8; 
		kingAttack[i] |= pos >> 8; 
		
		//left right 
		if (pos & ~AFile) kingAttack[i] |= pos << 1;
		if (pos & ~HFile) kingAttack[i] |= pos >> 1;

		// '/' diagonal 
		if (pos & ~HFile) kingAttack[i] |= pos << 7; 
		if (pos & ~AFile) kingAttack[i] |= pos >> 7; 

		// '\' diagonal
		if (pos & ~AFile) kingAttack[i] |= pos << 9; 
		if (pos & ~HFile) kingAttack[i] |= pos >> 9; 

	}
}

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


