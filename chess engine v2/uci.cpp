#include "uci.h"


/********************
*
*Util
*
*********************/
static constexpr int charPiece[2][128] =
{ {0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
5, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
{0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0,
11, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
};

/********************
*
*UCI
*
*********************/

UCI& UCI::get() {
	static UCI instance;
	return instance; 
}

int UCI::parseMove(const std::string& move, Movelist& ml, const BoardState& boardState) {
	return UCI::get().IparseMove(move, ml, boardState.getSide());
}

int UCI::IparseMove(const std::string& move, Movelist& ml, int side) {
	const int sourceSquare = (8 - (move[1] - '0')) * 8 + (move[0] - 'a');
	const int targetSquare = (8 - (move[3] - '0')) * 8 + (move[2] - 'a'); 
	const int promotePiece = (move.size() == 5 ? charPiece[side][move[4]] : wPawn); 

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
