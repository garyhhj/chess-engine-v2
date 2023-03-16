#include "movelist.h"



/********************
*
*Utils
*
*********************/

static consteval auto compileTime(auto val) {
	return val;
}

static inline int bishopMagicIndex(const uint64_t occ, int index) {
	return static_cast<int>(((occ & relevantBishopBlocker[index]) * bishopMagicNum[index]) >> (64 - getNumBit(relevantBishopBlocker[index])));
}

static inline int rookMagicIndex(const uint64_t occ, int index) {
	return static_cast<int>(((occ & relevantRookBlocker[index]) * rookMagicNum[index]) >> (64 - getNumBit(relevantRookBlocker[index])));
}


/********************
*
*Movelist
*
*********************/

//Movelist::Movelist() : movelist(new move[256]), index(0) {}


//rule of 3.5
//Movelist::Movelist(const Movelist& rhs) : movelist(new move[256]), index(rhs.index) {
//	std::cout << "copy constructor" << std::endl; 
//	for (int i = 0; i < index; ++i) {
//		movelist[i] = rhs.movelist[i]; 
//	}
//}
//
//Movelist& Movelist::operator=(Movelist& rhs) {
//	std::cout << "copy assignment operator" << std::endl; 
//	Movelist::swap(*this, rhs); 
//	rhs.movelist = nullptr; 
//	return *this; 
//}
//
//
//Movelist::~Movelist() {
//	//std::cout << "destructor" << std::endl; 
//	delete[] movelist; 
//}

constexpr void Movelist::pushBack(const move m) {
	movelist[index++] = m; 
}

void Movelist::print() {
	std::cout << "printing movelist" << std::endl;
	std::cout << "num moves: " << index << std::endl;

	for (int i = 0; i < Movelist::index; ++i) {
		//padding/move index 
		std::cout << "move: " << i;
		if (i < 10) std::cout << " ";
		std::cout << " "; 

		Move::decode(Movelist::movelist[i]);
		std::cout << "\n";
	}
	std::flush(std::cout); 
}
//
//void Movelist::swap(Movelist& m1, Movelist& m2) {
//	move* ptr = m1.movelist; 
//	m1.movelist = m2.movelist; 
//	m2.movelist = ptr; 
//
//	int temp = m1.index; 
//	m1.index = m2.index; 
//	m2.index = temp; 
//}

move* Movelist::getMovelist() {
	return Movelist::movelist; 
}

move Movelist::getMove(const int index) {
	return Movelist::movelist[index]; 
}

const int Movelist::getIndex() const {
	return Movelist::index; 
}


/********************
*Generate Moves
*********************/

void Movelist::moveGen(const Board& board, BoardState& boardState) {
	boardState.side == white ? 
		Movelist::moveGenWhite(board, boardState) : 
		Movelist::moveGenBlack(board, boardState);
}

void Movelist::moveGenWhite(const Board& board, BoardState& boardState) {

	const map checkMask = board.checkMask(boardState); 
	const map kingSafeSquares = board.safeSquares(boardState);
	const map pinMaskHV = board.pinMaskHV(boardState); 
	const map pinMaskDiagonal = board.pinMaskDiagonal(boardState); 

	const map occ = board.occupancy[white] | board.occupancy[black]; 


	//start generating moves 

	//double check - can only move king 
	if (boardState.doubleCheck) {
		map targetSquares = kingSafeSquares;
		while (targetSquares) {
			const map targetSquare = getLsbBit(targetSquares); 
			Movelist::pushBack(Move::makemove(board.piece[wKing], targetSquare, wKing, wPawn, targetSquare & board.occupancy[black], false, false, false));

			targetSquares &= targetSquares - 1; 
		}
		return; 
	}


	//wPawn
	// diagonally pinned pawns cannot push 
	// HV pinned pawns can be pushed along the pin  
	
	//pawn push non promotion 
	{
		//non HV pinned 
		map pawns = ((board.piece[wPawn] & ~pinMaskDiagonal & ~pinMaskHV) << 8) & ~(board.occupancy[white] | board.occupancy[black]) & ~Row8 & checkMask;
		while (pawns) {
			const map targetSquare = getLsbBit(pawns); 
			Movelist::pushBack(Move::makemove(targetSquare >> 8, targetSquare, wPawn, wPawn, false, false, false, false));
			
			pawns &= pawns - 1; 
		}

		//HV pinned 
		pawns = (board.piece[wPawn] & ~pinMaskDiagonal & pinMaskHV & ~Row5) << 8 & pinMaskHV & ~(board.occupancy[white] | board.occupancy[black]) & ~Row8 & checkMask;
		while (pawns){
			const map targetSquare = getLsbBit(pawns);
			Movelist::pushBack(Move::makemove(targetSquare >> 8, targetSquare, wPawn, wPawn, false, false, false, false));
		
			pawns &= pawns - 1; 
		}

		pawns = (board.piece[wPawn] & ~pinMaskDiagonal & pinMaskHV & Row5) << 8 /* & pinMaskHV*/ & ~(board.occupancy[white] | board.occupancy[black]) & ~Row8 & checkMask;
		//std::cout << "pawns: " << std::endl;                                       //might need to remove this and make sure it works special 
		//printBit(pawns); 
		while (pawns) {
			const map targetSquare = getLsbBit(pawns);
			const int enpassantIndex = getlsbBitIndex(boardState.enpassant);
			if ((enpassantIndex != -1 && (pawnAttack[black][enpassantIndex] & (targetSquare >> 8)))
				|| (targetSquare & pinMaskHV))
				Movelist::pushBack(Move::makemove(targetSquare >> 8, targetSquare, wPawn, wPawn, false, false, false, false));

			pawns &= pawns - 1;
		}
	}

	//pawn push promotion 
	{
		//non HV pinned 
		map pawns = (board.piece[wPawn] & ~pinMaskDiagonal & ~pinMaskHV) << 8 & ~(board.occupancy[white] | board.occupancy[black]) & Row8 & checkMask;
		while (pawns) {
			const map sourceSquare = getLsbBit(pawns) >> 8; 
			const map targetSquare = getLsbBit(pawns); 
			Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, wPawn, wKnight, false, false, false, false));
			Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, wPawn, wRook, false, false, false, false));
			Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, wPawn, wBishop, false, false, false, false));
			Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, wPawn, wQueen, false, false, false, false));

			pawns &= pawns - 1;
		}

		//HV pinned 
		pawns = (board.piece[wPawn] & ~pinMaskDiagonal & pinMaskHV) << 8 & pinMaskHV & ~(board.occupancy[white] | board.occupancy[black]) & Row8 & checkMask;
		while (pawns) {
			const map sourceSquare = getLsbBit(pawns) >> 8;
			const map targetSquare = getLsbBit(pawns);
			Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, wPawn, wKnight, false, false, false, false));
			Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, wPawn, wRook, false, false, false, false));
			Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, wPawn, wBishop, false, false, false, false));
			Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, wPawn, wQueen, false, false, false, false));

			pawns &= pawns - 1;
		}
	}

	//pawn double push 
	{
		//non HV pinned 
		map pawns = ((board.piece[wPawn] & ~pinMaskDiagonal & ~pinMaskHV) << 8 & ~(board.occupancy[white] | board.occupancy[black])) << 8 & ~(board.occupancy[white] | board.occupancy[black]) & Row4 & checkMask;
		while (pawns) {
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) >> 16, getLsbBit(pawns), wPawn, wPawn, false, true, false, false));

			pawns &= pawns - 1;
		}

		//HV pinned 
		pawns = ((board.piece[wPawn] & ~pinMaskDiagonal & pinMaskHV) << 8 & ~(board.occupancy[white] | board.occupancy[black])) << 8 & pinMaskHV & ~(board.occupancy[white] | board.occupancy[black]) & Row4 & checkMask;
		while (pawns) {
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) >> 16, getLsbBit(pawns), wPawn, wPawn, false, true, false, false));

			pawns &= pawns - 1; 
		}
	}

	//pawn captures 
	//diagonally pinned pawns can move along the pin  
	//HV pinned pawns cannot move 

	//pawn captures non promotion 
	{
		//non Diagonally pinned 
		map pawns = board.piece[wPawn] & ~pinMaskHV & ~pinMaskDiagonal & ~Row7;
		while (pawns) {
			map captures = pawnAttack[white][getlsbBitIndex(pawns)] & board.occupancy[black] & checkMask;
			while (captures) {
				Movelist::pushBack(Move::makemove(getLsbBit(pawns), getLsbBit(captures), wPawn, wPawn, true, false, false, false));

				captures &= captures - 1; 
			}

			pawns &= pawns - 1; 
		}
		
		//diagonally pinned 
		pawns = board.piece[wPawn] & ~pinMaskHV & pinMaskDiagonal & ~Row7; 
		while (pawns) {
			map captures = pawnAttack[white][getlsbBitIndex(pawns)] & board.occupancy[black] & pinMaskDiagonal & checkMask; 
			while (captures) {
				Movelist::pushBack(Move::makemove(getLsbBit(pawns), getLsbBit(captures), wPawn, wPawn, true, false, false, false));

				captures &= captures - 1;
			}

			pawns &= pawns - 1; 
		}

		//horizontally pinned pawns(for enpassant capture) 
		//only have this pawn mask allowed if king is horizontally pinned meaning king must be one row 5 

		pawns = (board.piece[wKing] & Row5 ? (board.piece[wPawn] & pinMaskHV & Row5) : 0x0ull);
		while (pawns) {
			map captures = pawnAttack[white][getlsbBitIndex(pawns)] & (board.occupancy[black] | boardState.enpassant);
			if (getNumBit(captures) == 2 && boardState.enpassant) {
				//we are allowed to capture the non enpassant square 
				while (captures) {
					const map targetSquare = getLsbBit(captures);
					if (~(targetSquare & boardState.enpassant) == AllOne) {
						Movelist::pushBack(Move::makemove(getLsbBit(pawns), targetSquare, wPawn, wPawn, true, false, false, false));
					}

					captures &= captures - 1;
				}

			}
			pawns &= pawns - 1;
		}
	}

	//pawn catpures with promotion 
	{
		//non Diagonally pinned 
		map pawns = board.piece[wPawn] & ~pinMaskHV & ~pinMaskDiagonal & Row7; 
		while (pawns) {
			map captures = pawnAttack[white][getlsbBitIndex(pawns)] & board.occupancy[black] & checkMask; 
			const map sourceSquare = getLsbBit(pawns); 
			while (captures) {
				const map targetSquare = getLsbBit(captures); 
				Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, wPawn, wKnight, true, false, false, false)); 
				Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, wPawn, wRook, true, false, false, false));
				Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, wPawn, wBishop, true, false, false, false));
				Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, wPawn, wQueen, true, false, false, false));

				captures &= captures - 1; 
			}

			pawns &= pawns - 1; 
		}

		//Diagonally pinned 
		pawns = board.piece[wPawn] & ~pinMaskHV & pinMaskDiagonal & Row7; 
		while (pawns) {
			map captures = pawnAttack[white][getlsbBitIndex(pawns)] & board.occupancy[black] & pinMaskDiagonal & checkMask; 
			const map sourceSquare = getLsbBit(pawns);
			while (captures) {
				const map targetSquare = getLsbBit(captures); 
				Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, wPawn, wKnight, true, false, false, false));
				Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, wPawn, wRook, true, false, false, false));
				Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, wPawn, wBishop, true, false, false, false));
				Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, wPawn, wQueen, true, false, false, false));
				captures &= captures - 1; 
			}
			
			pawns &= pawns - 1; 
		}
	}
	
	//enpassant capture 
	{
		if (boardState.enpassant && ((boardState.enpassant & checkMask) || (boardState.enpassant & (checkMask << 8)))) {
			const int targetIndex = getlsbBitIndex(boardState.enpassant); 
			
			//non Diagonally pinned 
			map sourceSquares = pawnAttack[black][targetIndex] & board.piece[wPawn] & ~pinMaskHV & ~pinMaskDiagonal;
			while (sourceSquares) {
				Movelist::pushBack(Move::makemove(getLsbBit(sourceSquares), indexSquare[targetIndex], wPawn, wPawn, true, false, true, false)); 

				sourceSquares &= sourceSquares - 1; 
			}

			//Diagonally pinned - no move possible 
		}
	}
	


	//knight moves 
	//pinned knights cannot move 
	
	{
		//source squares 
		map knights = board.piece[wKnight] & ~pinMaskDiagonal & ~pinMaskHV; 
		while (knights) {
			//target squares 
			map attacks = knightAttack[getlsbBitIndex(knights)] & checkMask & ~board.occupancy[white];
			const map sourceSquare = getLsbBit(knights); 
			while (attacks) {
				const map targetSquare = getLsbBit(attacks); 
				Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, wKnight, wPawn, targetSquare & board.occupancy[black], false, false, false)); 

				attacks &= attacks - 1; 
			}
			knights &= knights - 1; 
		}
	}
	
	//king moves 
	//check if square is safe before moving 
	//pins and check mask do no apply to the king 
	{
		map targetSquares = kingSafeSquares;
		while (targetSquares) {
			const map targetSquare = getLsbBit(targetSquares);
			Movelist::pushBack(Move::makemove(board.piece[wKing], targetSquare, wKing, wPawn, targetSquare & board.occupancy[black], false, false, false)); 

			targetSquares &= targetSquares - 1; 
		}
	}

	//bishop moves 
	//hv pinned bishop cannot move 
	//diagonally pinned bishop can move along the pin 
	{
		//non diagonally pinned 
		map bishops = board.piece[wBishop] & ~pinMaskHV & ~pinMaskDiagonal;
		//std::cout << "bishops non diagonally pinned:" << std::endl; 
		//printBit(bishops); 
		while (bishops) {
			map targetSquares = bishopAttack[getlsbBitIndex(bishops)][bishopMagicIndex(occ, getlsbBitIndex(bishops))] & checkMask & ~board.occupancy[white];
			//std::cout << "targetSquares: " << std::endl; 
			//printBit(targetSquares); 
			const map sourceSquare = getLsbBit(bishops); 
			while (targetSquares) {
				const map targetSquare = getLsbBit(targetSquares); 
				Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, wBishop, wPawn, targetSquare & board.occupancy[black], false, false, false)); 

				targetSquares &= targetSquares - 1; 
			}

			bishops &= bishops - 1; 
		}

		//diagonally pinned 
		bishops = board.piece[wBishop] & ~pinMaskHV & pinMaskDiagonal; 
		//std::cout << "bishops non diagonally pinned:" << std::endl;
		//printBit(bishops);
		while (bishops) {
			map targetSquares = bishopAttack[getlsbBitIndex(bishops)][bishopMagicIndex(occ, getlsbBitIndex(bishops))] & checkMask & ~board.occupancy[white] & pinMaskDiagonal;
			//std::cout << "targetSquares: " << std::endl; 
			//printBit(targetSquares); 
			const map sourceSquare = getLsbBit(bishops); 
			while (targetSquares) {
				const map targetSquare = getLsbBit(targetSquares); 
				Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, wBishop, wPawn, targetSquare & board.occupancy[black], false, false, false));

				targetSquares &= targetSquares - 1;
			}

			bishops &= bishops - 1;
		}
	}

	//rook moves 
	//diagonally pinned rook cannot move 
	//HV pinned rook can move along the pin 
	{
		//non HV pinned 
		map rooks = board.piece[wRook] & ~pinMaskDiagonal & ~pinMaskHV;
		while (rooks) {
			const int rookIndex = getlsbBitIndex(rooks);
			map targetSquares = rookAttack[rookIndex][rookMagicIndex(occ, rookIndex)] & checkMask & ~board.occupancy[white]; 
			while (targetSquares) {
				const map targetSquare = getLsbBit(targetSquares); 
				Movelist::pushBack(Move::makemove(getLsbBit(rooks), targetSquares, wRook, wPawn, targetSquare& board.occupancy[black], false, false, false)); 

				targetSquares &= targetSquares - 1; 
			}

			rooks &= rooks - 1; 
		}
		
		//HV pinned 
		rooks = board.piece[wRook] & ~pinMaskDiagonal & pinMaskHV; 

		while (rooks) {
			const int rookIndex = getlsbBitIndex(rooks); 
			map targetSquares = rookAttack[rookIndex][rookMagicIndex(occ, rookIndex)] & checkMask & ~board.occupancy[white] & pinMaskHV; 
			while (targetSquares) {
				const map targetSquare = getLsbBit(targetSquares);
				Movelist::pushBack(Move::makemove(getLsbBit(rooks), targetSquare, wRook, wPawn, targetSquare & board.occupancy[black], false, false, false));

				targetSquares &= targetSquares - 1; 
			}

			rooks &= rooks - 1; 
		}
	}

	//queen moves 
	//same as rook and bishop moves 
	{
		//diagonals 
		//non diagonally pinned 
		map queens = board.piece[wQueen] & ~pinMaskHV & ~pinMaskDiagonal;
		while (queens) {
			const int queenIndex = getlsbBitIndex(queens); 
			map targetSquares = bishopAttack[queenIndex][bishopMagicIndex(occ, queenIndex)] & checkMask & ~board.occupancy[white];
			while (targetSquares) {
				const map targetSquare = getLsbBit(targetSquares); 
				Movelist::pushBack(Move::makemove(getLsbBit(queens), targetSquare, wQueen, wPawn, targetSquare & board.occupancy[black], false, false, false));

				targetSquares &= targetSquares - 1;
			}

			queens &= queens - 1;
		}

		//diagonally pinned 
		queens = board.piece[wQueen] & ~pinMaskHV & pinMaskDiagonal;
		while (queens) {
			const int queenIndex = getlsbBitIndex(queens); 
			map targetSquares = bishopAttack[queenIndex][bishopMagicIndex(occ, queenIndex)] & checkMask & ~board.occupancy[white] & pinMaskDiagonal;
			while (targetSquares) {
				const map targetSquare = getLsbBit(targetSquares);
				Movelist::pushBack(Move::makemove(getLsbBit(queens), targetSquare, wQueen, wPawn, targetSquare & board.occupancy[black], false, false, false));

				targetSquares &= targetSquares - 1;
			}

			queens &= queens - 1;
		}


		//HVs 
		//non HV pinned 
		queens = board.piece[wQueen] & ~pinMaskDiagonal & ~pinMaskHV;
		while (queens) {
			const int queenIndex = getlsbBitIndex(queens); 
			map targetSquares = rookAttack[queenIndex][rookMagicIndex(occ, queenIndex)] & checkMask & ~board.occupancy[white];
			while (targetSquares) {
				const map targetSquare = getLsbBit(targetSquares); 
				Movelist::pushBack(Move::makemove(getLsbBit(queens), targetSquare, wQueen, wPawn, targetSquare & board.occupancy[black], false, false, false));

				targetSquares &= targetSquares - 1;
			}

			queens &= queens - 1;
		}

		//HV pinned 
		queens = board.piece[wQueen] & ~pinMaskDiagonal & pinMaskHV;
		while (queens) {
			const int queenIndex = getlsbBitIndex(queens); 
			map targetSquares = rookAttack[queenIndex][rookMagicIndex(occ, queenIndex)] & checkMask & ~board.occupancy[white] & pinMaskHV;
			while (targetSquares) {
				const map targetSquare = getLsbBit(targetSquares); 
				Movelist::pushBack(Move::makemove(getLsbBit(queens), targetSquare, wRook, wPawn, targetSquare & board.occupancy[black], false, false, false));

				targetSquares &= targetSquares - 1;
			}

			queens &= queens - 1;
		}
	}

	
	//castle moves 
	//Neither the king nor the rook has previously moved. - kept track through castle flag 
	//There are no pieces between the king and the rook. - check occupancy 
	//The king is not currently in check. - check if all bits in check mask is 1... therefore 0xffff....  
	//The king does not pass through or finish on a square that is attacked by an enemy piece. - check attacked on certain squares 
	{
		if (boardState.castleRightWK && !(occ & (F1 | G1)) && checkMask == ~0x0ull && !board.attacked(F1, boardState) && !board.attacked(G1, boardState)) {
			Movelist::pushBack(Move::makemove(map(E1), map(G1), wKing, wPawn, false, false, false, true)); 
		}

		if (boardState.castleRightWQ && !(occ & (D1 | C1 | B1)) && checkMask == ~0x0ull && !board.attacked(D1, boardState) && !board.attacked(C1, boardState)) {
			Movelist::pushBack(Move::makemove(map(E1), map(C1), wKing, wPawn, false, false, false, true)); 
		}

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

}

void Movelist::moveGenBlack(const Board& board, BoardState& boardState) {
	
	const map checkMask = board.checkMask(boardState);
	const map kingSafeSquares = board.safeSquares(boardState);
	const map pinMaskHV = board.pinMaskHV(boardState);
	const map pinMaskDiagonal = board.pinMaskDiagonal(boardState);
	const map occ = board.occupancy[white] | board.occupancy[black];

	//start generating moves 

	//double check - can only move king 
	if (boardState.doubleCheck) {
		map targetSquares = kingSafeSquares;
		while (targetSquares) {
			const map targetSquare = getLsbBit(targetSquares);
			Movelist::pushBack(Move::makemove(board.piece[bKing], targetSquare, bKing, wPawn, targetSquare & board.occupancy[white], false, false, false));

			targetSquares &= targetSquares - 1;
		}
		return;
	}


	//wPawn
	// diagonally pinned pawns cannot push 
	// HV pinned pawns can be pushed along the pin  

	//pawn push non promotion 
	{
		//non HV pinned 
		map pawns = ((board.piece[bPawn] & ~pinMaskDiagonal & ~pinMaskHV) >> 8) & ~(board.occupancy[white] | board.occupancy[black]) & ~Row1 & checkMask;
		while (pawns) {
			const map targetSquare = getLsbBit(pawns); 
			Movelist::pushBack(Move::makemove(targetSquare << 8, targetSquare, bPawn, wPawn, false, false, false, false));

			pawns &= pawns - 1;
		}

		//HV pinned 
		pawns = (board.piece[bPawn] & ~pinMaskDiagonal & pinMaskHV & ~Row4) >> 8 & pinMaskHV & ~(board.occupancy[white] | board.occupancy[black]) & ~Row1 & checkMask;
		while (pawns) {
			const map targetSquare = getLsbBit(pawns); 
			Movelist::pushBack(Move::makemove(targetSquare << 8, targetSquare, bPawn, wPawn, false, false, false, false));

			pawns &= pawns - 1;
		}

		pawns = (board.piece[bPawn] & ~pinMaskDiagonal & pinMaskHV & Row4) >> 8 /* & pinMaskHV*/ & ~(board.occupancy[white] | board.occupancy[black]) & ~Row1 & checkMask;
		//std::cout << "pawns: " << std::endl;                                       //might need to remove this and make sure it works special 
		//printBit(pawns); 
		while (pawns) {
			const map targetSquare = getLsbBit(pawns);
			const int enpassantIndex = getlsbBitIndex(boardState.enpassant);
			if ((enpassantIndex != -1 && (pawnAttack[white][enpassantIndex] & (targetSquare << 8)))
				|| (targetSquare & pinMaskHV))
				Movelist::pushBack(Move::makemove(targetSquare << 8, targetSquare, bPawn, wPawn, false, false, false, false));
			pawns &= pawns - 1;
		}
	}

	//pawn push promotion 
	{
		//non HV pinned 
		map pawns = (board.piece[bPawn] & ~pinMaskDiagonal & ~pinMaskHV) >> 8 & ~(board.occupancy[white] | board.occupancy[black]) & Row1 & checkMask;
		while (pawns) {
			const map targetSquare = getLsbBit(pawns); 
			Movelist::pushBack(Move::makemove(targetSquare << 8, targetSquare, bPawn, bKnight, false, false, false, false));
			Movelist::pushBack(Move::makemove(targetSquare << 8, targetSquare, bPawn, bRook, false, false, false, false));
			Movelist::pushBack(Move::makemove(targetSquare << 8, targetSquare, bPawn, bBishop, false, false, false, false));
			Movelist::pushBack(Move::makemove(targetSquare << 8, targetSquare, bPawn, bQueen, false, false, false, false));

			pawns &= pawns - 1;
		}

		//HV pinned 
		pawns = (board.piece[bPawn] & ~pinMaskDiagonal & pinMaskHV) >> 8 & pinMaskHV & ~(board.occupancy[white] | board.occupancy[black]) & Row1 & checkMask;
		while (pawns) {
			const map targetSquare = getLsbBit(pawns);
			Movelist::pushBack(Move::makemove(targetSquare << 8, targetSquare, bPawn, bKnight, false, false, false, false));
			Movelist::pushBack(Move::makemove(targetSquare << 8, targetSquare, bPawn, bRook, false, false, false, false));
			Movelist::pushBack(Move::makemove(targetSquare << 8, targetSquare, bPawn, bBishop, false, false, false, false));
			Movelist::pushBack(Move::makemove(targetSquare << 8, targetSquare, bPawn, bQueen, false, false, false, false));

			pawns &= pawns - 1;
		}
	}

	//pawn double push 
	{
		//non HV pinned 
		map pawns = ((board.piece[bPawn] & ~pinMaskDiagonal & ~pinMaskHV) >> 8 & ~(board.occupancy[white] | board.occupancy[black])) >> 8 & ~(board.occupancy[white] | board.occupancy[black]) & Row5 & checkMask;
		while (pawns) {
			const map targetSquare = getLsbBit(pawns);
			Movelist::pushBack(Move::makemove(targetSquare << 16, targetSquare, bPawn, wPawn, false, true, false, false));

			pawns &= pawns - 1;
		}

		//HV pinned 
		pawns = ((board.piece[bPawn] & ~pinMaskDiagonal & pinMaskHV) >> 8 & ~(board.occupancy[white] | board.occupancy[black])) >> 8 & pinMaskHV & ~(board.occupancy[white] | board.occupancy[black]) & Row5 & checkMask;
		while (pawns) {
			const map targetSquare = getLsbBit(pawns);
			Movelist::pushBack(Move::makemove(targetSquare << 16, targetSquare, bPawn, wPawn, false, true, false, false));

			pawns &= pawns - 1;
		}
	}

	//pawn captures 
	//diagonally pinned pawns can move along the pin  
	//HV pinned pawns cannot move 

	//pawn captures non promotion 
	{
		//non Diagonally pinned 
		map pawns = board.piece[bPawn] & ~pinMaskHV & ~pinMaskDiagonal & ~Row2;

		while (pawns) {
			map captures = pawnAttack[black][getlsbBitIndex(pawns)] & board.occupancy[white] & checkMask;
			const map sourceSquare = getLsbBit(pawns);
			while (captures) {
				Movelist::pushBack(Move::makemove(sourceSquare, getLsbBit(captures), bPawn, wPawn, true, false, false, false));

				captures &= captures - 1;
			}

			pawns &= pawns - 1;
		}

		//diagonally pinned 
		pawns = board.piece[bPawn] & ~pinMaskHV & pinMaskDiagonal & ~Row2;
		while (pawns) {
			map captures = pawnAttack[black][getlsbBitIndex(pawns)] & board.occupancy[white] & pinMaskDiagonal & checkMask;
			const map sourceSquare = getLsbBit(pawns);
			while (captures) {
				Movelist::pushBack(Move::makemove(sourceSquare, getLsbBit(captures), bPawn, wPawn, true, false, false, false));

				captures &= captures - 1;
			}

			pawns &= pawns - 1;
		}

		//horizontally pinned pawns(for enpassant capture) 
		pawns = (board.piece[bKing] & Row4 ? (board.piece[bPawn] & pinMaskHV & Row4) : 0x0ull); 
		while (pawns) {
			map captures = pawnAttack[black][getlsbBitIndex(pawns)] & (board.occupancy[white] | boardState.enpassant);
			if (getNumBit(captures) == 2 && boardState.enpassant) {
				//we are allowed to capture the non enpassant square 
				while (captures) {
					const map targetSquare = getLsbBit(captures); 
					if (~(targetSquare & boardState.enpassant) == AllOne) {
						Movelist::pushBack(Move::makemove(getLsbBit(pawns), targetSquare, bPawn, wPawn, true, false, false, false)); 
					}

					captures &= captures - 1; 
				}

			}
			pawns &= pawns - 1; 
		}
	}

	//pawn captures with promotion 
	{
		//non Diagonally pinned 
		map pawns = board.piece[bPawn] & ~pinMaskHV & ~pinMaskDiagonal & Row2;
		while (pawns) {
			map captures = pawnAttack[black][getlsbBitIndex(pawns)] & board.occupancy[white] & checkMask;
			const map sourceSquare = getLsbBit(pawns);
			while (captures) {
				const map targetSquare = getLsbBit(captures);
				Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, bPawn, bKnight, true, false, false, false));
				Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, bPawn, bRook, true, false, false, false));
				Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, bPawn, bBishop, true, false, false, false));
				Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, bPawn, bQueen, true, false, false, false));

				captures &= captures - 1;
			}

			pawns &= pawns - 1;
		}

		//Diagonally pinned 
		pawns = board.piece[bPawn] & ~pinMaskHV & pinMaskDiagonal & Row2;
		while (pawns) {
			map captures = pawnAttack[black][getlsbBitIndex(pawns)] & board.occupancy[white] & pinMaskDiagonal & checkMask;
			const map sourceSquare = getLsbBit(pawns);
			while (captures) {
				const map targetSquare = getLsbBit(captures);
				Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, bPawn, bKnight, true, false, false, false));
				Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, bPawn, bRook, true, false, false, false));
				Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, bPawn, bBishop, true, false, false, false));
				Movelist::pushBack(Move::makemove(sourceSquare, targetSquare, bPawn, bQueen, true, false, false, false));
				captures &= captures - 1; 
			}

			pawns &= pawns - 1;
		}
	}

	//enpassant capture 
	{
		//std::cout << "checkmask : " << std::endl; 
		//printBit(checkMask); 

		if (boardState.enpassant && ((boardState.enpassant & checkMask) || (boardState.enpassant & (checkMask >> 8)))) {
			const int targetIndex = getlsbBitIndex(boardState.enpassant);

			//non Diagonally pinned 
			map sourceSquares = pawnAttack[white][targetIndex] & board.piece[bPawn] & ~pinMaskHV & ~pinMaskDiagonal;
			while (sourceSquares) {
				Movelist::pushBack(Move::makemove(getLsbBit(sourceSquares), indexSquare[targetIndex], bPawn, wPawn, true, false, true, false));

				sourceSquares &= sourceSquares - 1;
			}

			//Diagonally pinned - no move possible 
		}


	}



	//knight moves 
	//pinned knights cannot move 

	{
		//source squares 
		map knights = board.piece[bKnight] & ~pinMaskDiagonal & ~pinMaskHV;
		while (knights) {
			//target squares 
			map attacks = knightAttack[getlsbBitIndex(knights)] & checkMask & ~board.occupancy[black];
			while (attacks) {
				Movelist::pushBack(Move::makemove(getLsbBit(knights), getLsbBit(attacks), bKnight, wPawn, getLsbBit(attacks) & board.occupancy[white], false, false, false));

				attacks &= attacks - 1;
			}
			knights &= knights - 1;
		}
	}

	//king moves 
	//check if square is safe before moving 
	//pins and check mask do no apply to the king 
	{
		map targetSquares = kingSafeSquares;
		while (targetSquares) {
			const map targetSquare = getLsbBit(targetSquares);
			Movelist::pushBack(Move::makemove(board.piece[bKing], targetSquare, bKing, wPawn, targetSquare & board.occupancy[white], false, false, false));

			targetSquares &= targetSquares - 1;
		}
	}

	//bishop moves 
	//hv pinned bishop cannot move 
	//diagonally pinned bishop can move along the pin 
	{
		//non diagonally pinned 
		map bishops = board.piece[bBishop] & ~pinMaskHV & ~pinMaskDiagonal;
		while (bishops) {
			const int bishopIndex = getlsbBitIndex(bishops);
			map targetSquares = bishopAttack[bishopIndex][bishopMagicIndex(occ, bishopIndex)] & checkMask & ~board.occupancy[black];
			while (targetSquares) {
				const map targetSquare = getLsbBit(targetSquares);
				Movelist::pushBack(Move::makemove(getLsbBit(bishops), targetSquare, bBishop, wPawn, targetSquare & board.occupancy[white], false, false, false));

				targetSquares &= targetSquares - 1;
			}

			bishops &= bishops - 1;
		}

		//diagonally pinned 
		bishops = board.piece[bBishop] & ~pinMaskHV & pinMaskDiagonal;
		while (bishops) {
			const int bishopIndex = getlsbBitIndex(bishops);
			map targetSquares = bishopAttack[bishopIndex][bishopMagicIndex(occ, bishopIndex)] & checkMask & ~board.occupancy[black] & pinMaskDiagonal;
			while (targetSquares) {
				const map targetSquare = getLsbBit(targetSquares);
				Movelist::pushBack(Move::makemove(getLsbBit(bishops), targetSquare, bBishop, wPawn, targetSquare & board.occupancy[white], false, false, false));

				targetSquares &= targetSquares - 1;
			}

			bishops &= bishops - 1;
		}
	}

	//rook moves 
	//diagonally pinned rook cannot move 
	//HV pinned rook can move along the pin 
	{
		//non HV pinned 
		map rooks = board.piece[bRook] & ~pinMaskDiagonal & ~pinMaskHV;
		while (rooks) {
			const int rookIndex = getlsbBitIndex(rooks);
			map targetSquares = rookAttack[rookIndex][rookMagicIndex(occ, rookIndex)] & checkMask & ~board.occupancy[black];
			while (targetSquares) {
				const map targetSquare = getLsbBit(targetSquares);
				Movelist::pushBack(Move::makemove(getLsbBit(rooks), targetSquare, bRook, wPawn, targetSquare & board.occupancy[white], false, false, false));

				targetSquares &= targetSquares - 1;
			}

			rooks &= rooks - 1;
		}

		//HV pinned 
		rooks = board.piece[bRook] & ~pinMaskDiagonal & pinMaskHV;
		while (rooks) {
			const int rookIndex = getlsbBitIndex(rooks);
			map targetSquares = rookAttack[rookIndex][rookMagicIndex(occ, rookIndex)] & checkMask & ~board.occupancy[black] & pinMaskHV;
			while (targetSquares) {
				const map targetSquare = getLsbBit(targetSquares);
				Movelist::pushBack(Move::makemove(getLsbBit(rooks), targetSquare, bRook, wPawn, targetSquare & board.occupancy[white], false, false, false));

				targetSquares &= targetSquares - 1;
			}

			rooks &= rooks - 1;
		}
	}

	//queen moves 
	//same as rook and bishop moves 
	{
		//diagonals 
		//non diagonally pinned 
		map queens = board.piece[bQueen] & ~pinMaskHV & ~pinMaskDiagonal;
		while (queens) {
			const int queenIndex = getlsbBitIndex(queens);
			map targetSquares = bishopAttack[queenIndex][bishopMagicIndex(occ, queenIndex)] & checkMask & ~board.occupancy[black];
			while (targetSquares) {
				const map targetSquare = getLsbBit(targetSquares);
				Movelist::pushBack(Move::makemove(getLsbBit(queens), targetSquare, bQueen, wPawn, targetSquare & board.occupancy[white], false, false, false));

				targetSquares &= targetSquares - 1;
			}

			queens &= queens - 1;
		}

		//diagonally pinned 
		queens = board.piece[bQueen] & ~pinMaskHV & pinMaskDiagonal;
		while (queens) {
			const int queenIndex = getlsbBitIndex(queens);
			map targetSquares = bishopAttack[queenIndex][bishopMagicIndex(occ, queenIndex)] & checkMask & ~board.occupancy[black] & pinMaskDiagonal;
			while (targetSquares) {
				const map targetSquare = getLsbBit(targetSquares);
				Movelist::pushBack(Move::makemove(getLsbBit(queens), targetSquare, bQueen, wPawn, targetSquare & board.occupancy[white], false, false, false));

				targetSquares &= targetSquares - 1;
			}

			queens &= queens - 1;
		}


		//HVs 
		//non HV pinned 
		queens = board.piece[bQueen] & ~pinMaskDiagonal & ~pinMaskHV;
		while (queens) {
			const int queenIndex = getlsbBitIndex(queens);
			map targetSquares = rookAttack[queenIndex][rookMagicIndex(occ, queenIndex)] & checkMask & ~board.occupancy[black];
			while (targetSquares) {
				const map targetSquare = getLsbBit(targetSquares);
				Movelist::pushBack(Move::makemove(getLsbBit(queens), targetSquare, bQueen, wPawn, targetSquare & board.occupancy[white], false, false, false));

				targetSquares &= targetSquares - 1;
			}

			queens &= queens - 1;
		}

		//HV pinned 
		queens = board.piece[bQueen] & ~pinMaskDiagonal & pinMaskHV;
		while (queens) {
			const int queenIndex = getlsbBitIndex(queens);
			map targetSquares = rookAttack[queenIndex][rookMagicIndex(occ, queenIndex)] & checkMask & ~board.occupancy[black] & pinMaskHV;
			while (targetSquares) {
				const map targetSquare = getLsbBit(targetSquares);
				Movelist::pushBack(Move::makemove(getLsbBit(queens), targetSquare, bQueen, wPawn, targetSquare & board.occupancy[white], false, false, false));

				targetSquares &= targetSquares - 1;
			}

			queens &= queens - 1;
		}
	}


	//castle moves 
	//Neither the king nor the rook has previously moved. - kept track through castle flag 
	//There are no pieces between the king and the rook. - check occupancy 
	//The king is not currently in check. - check if all bits in check mask is 1... therefore 0xffff....  
	//The king does not pass through or finish on a square that is attacked by an enemy piece. - check attacked on certain squares 
	{
		if (boardState.castleRightBK && !(occ & (F8 | G8)) && checkMask == ~0x0ull && !board.attacked(F8, boardState) && !board.attacked(G8, boardState)) {
			Movelist::pushBack(Move::makemove(map(E8), map(G8), bKing, wPawn, false, false, false, true));
		}

		if (boardState.castleRightBQ && !(occ & (D8 | C8 | B8)) && checkMask == ~0x0ull && !board.attacked(D8, boardState) && !board.attacked(C8, boardState)) {
			Movelist::pushBack(Move::makemove(map(E8), map(C8), bKing, wPawn, false, false, false, true));
		}

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

}


