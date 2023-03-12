#include "uci.h"


/********************
*
*Util
*
*********************/
static int charPiece[2][128];

/********************
*
*UCI
*
*********************/

UCI& UCI::get() {
	static UCI instance;
	return instance; 
}

int UCI::parseMove(const std::string& move, Movelist& ml) {
	return UCI::get().parseMove(move, ml);
}

int UCI::IparseMove(const std::string& move, Movelist& ml) {
	const int sourceSquare = (8 - (move[1] - '0')) * 8 + (move[0] - 'a');
	const int targetSquare = (8 - (move[3] - '0')) * 8 + (move[2] - 'a'); 

	//something to do with promote piece 

	const int promotePiece = wPawn; 
	//big if statement, can I speed this up with an array? 
	//need to change it ? idk how to tell white or black 

	for (int i = 0; i < ml.getIndex(); ++i) {
		uint32_t move = ml.getMove(i); 
		if(sourceSquare == Move::sourceSquare(ml.getMove(i)) && 
			targetSquare == Move::targetSquare(ml.getMove(i)) && 
			promotePiece == Move::promotePiece(ml.getMove(i))) {
			return i; 
		}
	}

	return -1; 


	//  8   r n b q k b n r
	//	7   p p p p r p p p
	//	6   0 0 0 0 0 0 0 0
	//	5   0 0 0 0 0 0 0 0	
	//	4   0 0 0 0 0 0 0 b
	//	3   0 0 0 0 0 0 0 0
	//	2   0 0 0 0 R R 0 0
	//	1   0 N 0 Q K 0 N 0

	//		a b c d e f g h
}
