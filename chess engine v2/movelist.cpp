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
	return ((occ & relevantBishopBlocker[index]) * bishopMagicNum[index]) >> (64 - getNumBit(relevantBishopBlocker[index]));
}

static inline int rookMagicIndex(const uint64_t occ, int index) {
	return ((occ & relevantRookBlocker[index]) * rookMagicNum[index]) >> (64 - getNumBit(relevantRookBlocker[index]));


	//return int((occ & relevantRookBlocker[index]) * rookMagicNum[index]) >> (64 - getNumBit(relevantRookBlocker[index]));
}


/********************
*
*Movelist
*
*********************/

Movelist::Movelist() : movelist(new move[256]), index(0) {}


//rule of 3.5
Movelist::Movelist(const Movelist& rhs) : movelist(new move[256]), index(rhs.index) {
	std::cout << "copy constructor" << std::endl; 
	for (int i = 0; i < index; ++i) {
		movelist[i] = rhs.movelist[i]; 
	}
}

Movelist& Movelist::operator=(Movelist& rhs) {
	std::cout << "copy assignment operator" << std::endl; 
	Movelist::swap(*this, rhs); 
	rhs.movelist = nullptr; 
	return *this; 
}


Movelist::~Movelist() {
	//std::cout << "destructor" << std::endl; 
	delete[] movelist; 
}

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
}

void Movelist::swap(Movelist& m1, Movelist& m2) {
	move* ptr = m1.movelist; 
	m1.movelist = m2.movelist; 
	m2.movelist = ptr; 

	int temp = m1.index; 
	m1.index = m2.index; 
	m2.index = temp; 
}

move Movelist::getMove(const int index) {
	return Movelist::movelist[index]; 
}

/********************
*Generate Moves
*********************/

void Movelist::moveGen(const Board& board, BoardState& boardState) {
	*boardState.side == white ? 
		Movelist::moveGenWhite(board, boardState) : 
		Movelist::moveGenBlack(board, boardState);
}

void Movelist::moveGenWhite(const Board& board, BoardState& boardState) {

	std::cout << "move gen white" << std::endl; 
	//get the check mask 
	//get the pin masks 
	const map checkMask = board.checkMask(boardState); 
	//const bool doubleCheck = BoardState::Get().doubleCheck; 
	
	const map pinMaskHV = board.pinMaskHV(boardState); 
	const map pinMaskDiagonal = board.pinMaskDiagonal(boardState); 

	const map occ = board.occupancy[white] | board.occupancy[black]; 

	std::cout << "checkMask: " << std::endl; 
	printBit(checkMask); 

	std::cout << "pinMaskHV: " << std::endl; 
	printBit(pinMaskHV); 

	std::cout << "pinMaskDiagonal: " << std::endl; 
	printBit(pinMaskDiagonal); 

	//start generating moves 
	//now generate moves 


	//for checkMask... it means that the pieces must be resulting in the checkmask 
	//if there are double checks, then can only move king  

	//for pin masks... it means that the pinned piece can only move within the pinned area 
	//prevent pawn captures, certain bishop diagonals, and certain rook moves in pinned pieces 
	//for leaper pieces, can just not let them move (except king) /but king won't be pinned, it will be checked 
	//for slider pieces, if same pin as move eg. bishop and diagonal, allow movement and only check against diagonal 
	//for slider pieces, if different pin, then it cannot be moved 



	//double check - can only move king 
	if (*boardState.doubleCheck) {
		const int kingIndex = getlsbBitIndex(board.piece[wKing]); 

		//king move noncapture 
		map potentialKingMoveNonCapture = kingAttack[kingIndex] & ~(board.occupancy[black] & board.occupancy[white]); 
		while (potentialKingMoveNonCapture) {
			//target square not attacked 
			if (!board.attacked(getLsbBit(potentialKingMoveNonCapture), boardState)) {
				Movelist::pushBack(Move::makemove(indexSquare[kingIndex], getLsbBit(potentialKingMoveNonCapture), wKing, wPawn, false, false, false, false)); 
			}
			potentialKingMoveNonCapture &= potentialKingMoveNonCapture - 1; 
		}


		//king move capture 
		map potentialKingMoveCapture = kingAttack[kingIndex] & board.occupancy[black];
		while (potentialKingMoveCapture) {
			//target square not attacked 
			if (!board.attacked(getLsbBit(potentialKingMoveCapture), boardState)) {
				Movelist::pushBack(Move::makemove(indexSquare[kingIndex], getLsbBit(potentialKingMoveNonCapture), wKing, wPawn, true, false, false, false));
			}
			potentialKingMoveCapture &= potentialKingMoveCapture - 1; 
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
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) >> 8, getLsbBit(pawns), wPawn, wPawn, false, false, false, false));
			
			pawns &= pawns - 1; 
		}

		//HV pinned 
		pawns = (board.piece[wPawn] & ~pinMaskDiagonal & pinMaskHV) << 8 & pinMaskHV & ~(board.occupancy[white] | board.occupancy[black]) & ~Row8 & checkMask;
		while (pawns){
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) >> 8, getLsbBit(pawns), wPawn, wPawn, false, false, false, false));
		
			pawns &= pawns - 1; 
		}
	}

	//pawn push promotion 
	{
		//non HV pinned 
		map pawns = (board.piece[wPawn] & ~pinMaskDiagonal & ~pinMaskHV) << 8 & ~(board.occupancy[white] | board.occupancy[black]) & Row8 & checkMask;
		while (pawns) {
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) >> 8, getLsbBit(pawns), wPawn, wKnight, false, false, false, false));
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) >> 8, getLsbBit(pawns), wPawn, wRook, false, false, false, false));
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) >> 8, getLsbBit(pawns), wPawn, wBishop, false, false, false, false));
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) >> 8, getLsbBit(pawns), wPawn, wQueen, false, false, false, false));

			pawns &= pawns - 1;
		}

		//HV pinned 
		pawns = (board.piece[wPawn] & ~pinMaskDiagonal & pinMaskHV) << 8 & pinMaskHV & ~(board.occupancy[white] | board.occupancy[black]) & Row8 & checkMask;
		while (pawns) {
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) >> 8, getLsbBit(pawns), wPawn, wKnight, false, false, false, false));
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) >> 8, getLsbBit(pawns), wPawn, wRook, false, false, false, false));
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) >> 8, getLsbBit(pawns), wPawn, wBishop, false, false, false, false));
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) >> 8, getLsbBit(pawns), wPawn, wQueen, false, false, false, false));

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
		map pawns = board.piece[wPawn] & ~pinMaskHV & ~pinMaskDiagonal & ~Row7 & checkMask;
		while (pawns) {
			map captures = pawnAttack[white][getlsbBitIndex(pawns)] & board.occupancy[black];
			while (captures) {
				Movelist::pushBack(Move::makemove(getLsbBit(pawns), getLsbBit(captures), wPawn, wPawn, true, false, false, false));

				captures &= captures - 1; 
			}

			pawns &= pawns - 1; 
		}
		
		//diagonally pinned 
		pawns = board.piece[wPawn] & ~pinMaskHV & pinMaskDiagonal & ~Row7 & checkMask; 
		while (pawns) {
			map captures = pawnAttack[white][getlsbBitIndex(pawns)] & board.occupancy[black] & pinMaskDiagonal; 
			while (captures) {
				Movelist::pushBack(Move::makemove(getLsbBit(pawns), getLsbBit(captures), wPawn, wPawn, true, false, false, false));

				captures &= captures - 1;
			}

			pawns &= pawns - 1; 
		}
	}

	//pawn catpures with promotion 
	{
		//non Diagonally pinned 
		map pawns = board.piece[wPawn] & ~pinMaskHV & ~pinMaskDiagonal & Row7 & checkMask; 
		while (pawns) {
			map captures = pawnAttack[white][getlsbBitIndex(pawns)] & board.occupancy[black]; 
			while (captures) {
				Movelist::pushBack(Move::makemove(getLsbBit(pawns), getLsbBit(captures), wPawn, wKnight, true, false, false, false)); 
				Movelist::pushBack(Move::makemove(getLsbBit(pawns), getLsbBit(captures), wPawn, wRook, true, false, false, false));
				Movelist::pushBack(Move::makemove(getLsbBit(pawns), getLsbBit(captures), wPawn, wBishop, true, false, false, false));
				Movelist::pushBack(Move::makemove(getLsbBit(pawns), getLsbBit(captures), wPawn, wQueen, true, false, false, false));

				captures &= captures - 1; 
			}

			pawns &= pawns - 1; 
		}

		//Diagonally pinned 
		pawns = board.piece[wPawn] & ~pinMaskHV & pinMaskDiagonal & Row7 & checkMask; 
		while (pawns) {
			map captures = pawnAttack[white][getlsbBitIndex(pawns)] & board.occupancy[black] & pinMaskDiagonal; 
			Movelist::pushBack(Move::makemove(getLsbBit(pawns), getLsbBit(captures), wPawn, wKnight, true, false, false, false));
			Movelist::pushBack(Move::makemove(getLsbBit(pawns), getLsbBit(captures), wPawn, wRook, true, false, false, false));
			Movelist::pushBack(Move::makemove(getLsbBit(pawns), getLsbBit(captures), wPawn, wBishop, true, false, false, false));
			Movelist::pushBack(Move::makemove(getLsbBit(pawns), getLsbBit(captures), wPawn, wQueen, true, false, false, false));

			pawns &= pawns - 1; 
		}
	}
	
	//enpassant capture 
	{
		if (*boardState.enpassant && (*boardState.enpassant & checkMask)) {
			const int targetIndex = getlsbBitIndex(*boardState.enpassant); 
			
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
			while (attacks) {
				Movelist::pushBack(Move::makemove(getLsbBit(knights), getLsbBit(attacks), wKnight, wPawn, getLsbBit(attacks) & board.occupancy[black], false, false, false)); 

				attacks &= attacks - 1; 
			}
			knights &= knights - 1; 
		}
	}
	
	//king moves 
	//check if square is safe before moving 
	//pins and check mask do no apply to the king 
	
	{
		map attacks = kingAttack[getlsbBitIndex(board.piece[wKing])] & ~board.occupancy[white]; 
		std::cout << "attacks: " << std::endl; 
		printBit(attacks); 
		while (attacks) {
			if (!board.attacked(getLsbBit(attacks), boardState)) {
				Movelist::pushBack(Move::makemove(board.piece[wKing], getLsbBit(attacks), wKing, wPawn, getLsbBit(attacks) & board.occupancy[black], false, false, false));
			}
			attacks &= attacks - 1; 
		}
	}

	//bishop moves 
	//hv pinned bishop cannot move 
	//diagonally pinned bishop can move along the pin 
	{
		//non diagonally pinned 
		map bishops = board.piece[wBishop] & ~pinMaskHV & ~pinMaskDiagonal;
		while (bishops) {
			map targetSquares = bishopAttack[getlsbBitIndex(bishops)][bishopMagicIndex(occ, getlsbBitIndex(bishops))] & checkMask & ~board.occupancy[white];
			while (targetSquares) {
				Movelist::pushBack(Move::makemove(getLsbBit(bishops), getLsbBit(targetSquares), wBishop, wPawn, getLsbBit(targetSquares)& board.occupancy[black], false, false, false)); 

				targetSquares &= targetSquares - 1; 
			}

			bishops &= bishops - 1; 
		}

		//diagonally pinned 
		bishops = board.piece[wBishop] & ~pinMaskHV & pinMaskDiagonal; 
		while (bishops) {
			map targetSquares = bishopAttack[getlsbBitIndex(bishops)][bishopMagicIndex(occ, getlsbBitIndex(bishops))] & checkMask & ~board.occupancy[white] & pinMaskDiagonal;
			while (targetSquares) {
				Movelist::pushBack(Move::makemove(getLsbBit(bishops), getLsbBit(targetSquares), wBishop, wPawn, getLsbBit(targetSquares) & board.occupancy[black], false, false, false));

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
			map targetSquares = rookAttack[getlsbBitIndex(rooks)][rookMagicIndex(occ, getlsbBitIndex(rooks))] & checkMask & ~board.occupancy[white]; 
			while (targetSquares) {
				Movelist::pushBack(Move::makemove(getLsbBit(rooks), getLsbBit(targetSquares), wRook, wPawn, getLsbBit(targetSquares)& board.occupancy[black], false, false, false)); 

				targetSquares &= targetSquares - 1; 
			}

			rooks &= rooks - 1; 
		}
		
		//HV pinned 
		rooks = board.piece[wRook] & ~pinMaskDiagonal & pinMaskHV; 
		while (rooks) {
			map targetSquares = rookAttack[getlsbBitIndex(rooks)][rookMagicIndex(occ, getlsbBitIndex(rooks))] & checkMask & ~board.occupancy[white] & pinMaskHV; 
			while (targetSquares) {
				Movelist::pushBack(Move::makemove(getLsbBit(rooks), getLsbBit(targetSquares), wRook, wPawn, getLsbBit(targetSquares)& board.occupancy[black], false, false, false));

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
			map targetSquares = bishopAttack[getlsbBitIndex(queens)][bishopMagicIndex(occ, getlsbBitIndex(queens))] & checkMask & ~board.occupancy[white];
			while (targetSquares) {
				Movelist::pushBack(Move::makemove(getLsbBit(queens), getLsbBit(targetSquares), wQueen, wPawn, getLsbBit(targetSquares) & board.occupancy[black], false, false, false));

				targetSquares &= targetSquares - 1;
			}

			queens &= queens - 1;
		}

		//diagonally pinned 
		queens = board.piece[wQueen] & ~pinMaskHV & pinMaskDiagonal;
		while (queens) {
			map targetSquares = bishopAttack[getlsbBitIndex(queens)][bishopMagicIndex(occ, getlsbBitIndex(queens))] & checkMask & ~board.occupancy[white] & pinMaskDiagonal;
			while (targetSquares) {
				Movelist::pushBack(Move::makemove(getLsbBit(queens), getLsbBit(targetSquares), wQueen, wPawn, getLsbBit(targetSquares) & board.occupancy[black], false, false, false));

				targetSquares &= targetSquares - 1;
			}

			queens &= queens - 1;
		}


		//HVs 
		//non HV pinned 
		queens = board.piece[wQueen] & ~pinMaskDiagonal & ~pinMaskHV;
		while (queens) {
			map targetSquares = rookAttack[getlsbBitIndex(queens)][rookMagicIndex(occ, getlsbBitIndex(queens))] & checkMask & ~board.occupancy[white];
			while (targetSquares) {
				Movelist::pushBack(Move::makemove(getLsbBit(queens), getLsbBit(targetSquares), wQueen, wPawn, getLsbBit(targetSquares) & board.occupancy[black], false, false, false));

				targetSquares &= targetSquares - 1;
			}

			queens &= queens - 1;
		}

		//HV pinned 
		queens = board.piece[wQueen] & ~pinMaskDiagonal & pinMaskHV;
		while (queens) {
			map targetSquares = rookAttack[getlsbBitIndex(queens)][rookMagicIndex(occ, getlsbBitIndex(queens))] & checkMask & ~board.occupancy[white] & pinMaskHV;
			while (targetSquares) {
				Movelist::pushBack(Move::makemove(getLsbBit(queens), getLsbBit(targetSquares), wRook, wPawn, getLsbBit(targetSquares) & board.occupancy[black], false, false, false));

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
	std::cout << "move gen black" << std::endl;
	//get the check mask 
	//get the pin masks 
	const map checkMask = board.checkMask(boardState);
	//const bool doubleCheck = BoardState::Get().doubleCheck; 

	const map pinMaskHV = board.pinMaskHV(boardState);
	const map pinMaskDiagonal = board.pinMaskDiagonal(boardState);

	const map occ = board.occupancy[white] | board.occupancy[black];

	std::cout << "checkMask: " << std::endl;
	printBit(checkMask);

	std::cout << "pinMaskHV: " << std::endl;
	printBit(pinMaskHV);

	std::cout << "pinMaskDiagonal: " << std::endl;
	printBit(pinMaskDiagonal);

	//start generating moves 
	//now generate moves 


	//for checkMask... it means that the pieces must be resulting in the checkmask 
	//if there are double checks, then can only move king  

	//for pin masks... it means that the pinned piece can only move within the pinned area 
	//prevent pawn captures, certain bishop diagonals, and certain rook moves in pinned pieces 
	//for leaper pieces, can just not let them move (except king) /but king won't be pinned, it will be checked 
	//for slider pieces, if same pin as move eg. bishop and diagonal, allow movement and only check against diagonal 
	//for slider pieces, if different pin, then it cannot be moved 



	//double check - can only move king 
	if (*boardState.doubleCheck) {
		const int kingIndex = getlsbBitIndex(board.piece[bKing]);

		//king move noncapture 
		map potentialKingMoveNonCapture = kingAttack[kingIndex] & ~(board.occupancy[black] & board.occupancy[white]);
		while (potentialKingMoveNonCapture) {
			//target square not attacked 
			if (!board.attacked(getLsbBit(potentialKingMoveNonCapture), boardState)) {
				Movelist::pushBack(Move::makemove(indexSquare[kingIndex], getLsbBit(potentialKingMoveNonCapture), bKing, wPawn, false, false, false, false));
			}
			potentialKingMoveNonCapture &= potentialKingMoveNonCapture - 1;
		}


		//king move capture 
		map potentialKingMoveCapture = kingAttack[kingIndex] & board.occupancy[black];
		while (potentialKingMoveCapture) {
			//target square not attacked 
			if (!board.attacked(getLsbBit(potentialKingMoveCapture), boardState)) {
				Movelist::pushBack(Move::makemove(indexSquare[kingIndex], getLsbBit(potentialKingMoveNonCapture), bKing, wPawn, true, false, false, false));
			}
			potentialKingMoveCapture &= potentialKingMoveCapture - 1;
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
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) << 8, getLsbBit(pawns), bPawn, wPawn, false, false, false, false));

			pawns &= pawns - 1;
		}

		//HV pinned 
		pawns = (board.piece[bPawn] & ~pinMaskDiagonal & pinMaskHV) >> 8 & pinMaskHV & ~(board.occupancy[white] | board.occupancy[black]) & ~Row1 & checkMask;
		while (pawns) {
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) << 8, getLsbBit(pawns), bPawn, wPawn, false, false, false, false));

			pawns &= pawns - 1;
		}
	}

	//pawn push promotion 
	{
		//non HV pinned 
		map pawns = (board.piece[bPawn] & ~pinMaskDiagonal & ~pinMaskHV) >> 8 & ~(board.occupancy[white] | board.occupancy[black]) & Row1 & checkMask;
		while (pawns) {
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) << 8, getLsbBit(pawns), bPawn, bKnight, false, false, false, false));
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) << 8, getLsbBit(pawns), bPawn, bRook, false, false, false, false));
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) << 8, getLsbBit(pawns), bPawn, bBishop, false, false, false, false));
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) << 8, getLsbBit(pawns), bPawn, bQueen, false, false, false, false));

			pawns &= pawns - 1;
		}

		//HV pinned 
		pawns = (board.piece[bPawn] & ~pinMaskDiagonal & pinMaskHV) >> 8 & pinMaskHV & ~(board.occupancy[white] | board.occupancy[black]) & Row1 & checkMask;
		while (pawns) {
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) << 8, getLsbBit(pawns), bPawn, bKnight, false, false, false, false));
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) << 8, getLsbBit(pawns), bPawn, bRook, false, false, false, false));
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) << 8, getLsbBit(pawns), bPawn, bBishop, false, false, false, false));
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) << 8, getLsbBit(pawns), bPawn, bQueen, false, false, false, false));

			pawns &= pawns - 1;
		}
	}

	//pawn double push 
	{
		//non HV pinned 
		map pawns = ((board.piece[bPawn] & ~pinMaskDiagonal & ~pinMaskHV) >> 8 & ~(board.occupancy[white] | board.occupancy[black])) >> 8 & ~(board.occupancy[white] | board.occupancy[black]) & Row5 & checkMask;
		while (pawns) {
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) << 16, getLsbBit(pawns), bPawn, wPawn, false, true, false, false));

			pawns &= pawns - 1;
		}

		//HV pinned 
		pawns = ((board.piece[bPawn] & ~pinMaskDiagonal & pinMaskHV) >> 8 & ~(board.occupancy[white] | board.occupancy[black])) >> 8 & pinMaskHV & ~(board.occupancy[white] | board.occupancy[black]) & Row5 & checkMask;
		while (pawns) {
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) << 16, getLsbBit(pawns), bPawn, wPawn, false, true, false, false));

			pawns &= pawns - 1;
		}
	}

	//pawn captures 
	//diagonally pinned pawns can move along the pin  
	//HV pinned pawns cannot move 

	//pawn captures non promotion 
	{
		//non Diagonally pinned 
		map pawns = board.piece[bPawn] & ~pinMaskHV & ~pinMaskDiagonal & ~Row2 & checkMask;
		while (pawns) {
			map captures = pawnAttack[black][getlsbBitIndex(pawns)] & board.occupancy[white];
			while (captures) {
				Movelist::pushBack(Move::makemove(getLsbBit(pawns), getLsbBit(captures), bPawn, wPawn, true, false, false, false));

				captures &= captures - 1;
			}

			pawns &= pawns - 1;
		}

		//diagonally pinned 
		pawns = board.piece[bPawn] & ~pinMaskHV & pinMaskDiagonal & ~Row2 & checkMask;
		while (pawns) {
			map captures = pawnAttack[black][getlsbBitIndex(pawns)] & board.occupancy[white] & pinMaskDiagonal;
			while (captures) {
				Movelist::pushBack(Move::makemove(getLsbBit(pawns), getLsbBit(captures), bPawn, wPawn, true, false, false, false));

				captures &= captures - 1;
			}

			pawns &= pawns - 1;
		}
	}

	//pawn catpures with promotion 
	{
		//non Diagonally pinned 
		map pawns = board.piece[bPawn] & ~pinMaskHV & ~pinMaskDiagonal & Row2 & checkMask;
		while (pawns) {
			map captures = pawnAttack[black][getlsbBitIndex(pawns)] & board.occupancy[white];
			while (captures) {
				Movelist::pushBack(Move::makemove(getLsbBit(pawns), getLsbBit(captures), bPawn, bKnight, true, false, false, false));
				Movelist::pushBack(Move::makemove(getLsbBit(pawns), getLsbBit(captures), bPawn, bRook, true, false, false, false));
				Movelist::pushBack(Move::makemove(getLsbBit(pawns), getLsbBit(captures), bPawn, bBishop, true, false, false, false));
				Movelist::pushBack(Move::makemove(getLsbBit(pawns), getLsbBit(captures), bPawn, bQueen, true, false, false, false));

				captures &= captures - 1;
			}

			pawns &= pawns - 1;
		}

		//Diagonally pinned 
		pawns = board.piece[bPawn] & ~pinMaskHV & pinMaskDiagonal & Row2 & checkMask;
		while (pawns) {
			map captures = pawnAttack[black][getlsbBitIndex(pawns)] & board.occupancy[white] & pinMaskDiagonal;
			Movelist::pushBack(Move::makemove(getLsbBit(pawns), getLsbBit(captures), bPawn, bKnight, true, false, false, false));
			Movelist::pushBack(Move::makemove(getLsbBit(pawns), getLsbBit(captures), bPawn, bRook, true, false, false, false));
			Movelist::pushBack(Move::makemove(getLsbBit(pawns), getLsbBit(captures), bPawn, bBishop, true, false, false, false));
			Movelist::pushBack(Move::makemove(getLsbBit(pawns), getLsbBit(captures), bPawn, bQueen, true, false, false, false));

			pawns &= pawns - 1;
		}
	}

	//enpassant capture 
	{
		if (*boardState.enpassant && (*boardState.enpassant & checkMask)) {
			const int targetIndex = getlsbBitIndex(*boardState.enpassant);

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
		map attacks = kingAttack[getlsbBitIndex(board.piece[bKing])] & ~board.occupancy[black];
		std::cout << "attacks: " << std::endl;
		printBit(attacks);
		while (attacks) {
			if (!board.attacked(getLsbBit(attacks), boardState)) {
				Movelist::pushBack(Move::makemove(board.piece[bKing], getLsbBit(attacks), bKing, wPawn, getLsbBit(attacks) & board.occupancy[white], false, false, false));
			}
			attacks &= attacks - 1;
		}
	}

	//bishop moves 
	//hv pinned bishop cannot move 
	//diagonally pinned bishop can move along the pin 
	{
		//non diagonally pinned 
		map bishops = board.piece[bBishop] & ~pinMaskHV & ~pinMaskDiagonal;
		while (bishops) {
			map targetSquares = bishopAttack[getlsbBitIndex(bishops)][bishopMagicIndex(occ, getlsbBitIndex(bishops))] & checkMask & ~board.occupancy[black];
			while (targetSquares) {
				Movelist::pushBack(Move::makemove(getLsbBit(bishops), getLsbBit(targetSquares), bBishop, wPawn, getLsbBit(targetSquares) & board.occupancy[white], false, false, false));

				targetSquares &= targetSquares - 1;
			}

			bishops &= bishops - 1;
		}

		//diagonally pinned 
		bishops = board.piece[bBishop] & ~pinMaskHV & pinMaskDiagonal;
		while (bishops) {
			map targetSquares = bishopAttack[getlsbBitIndex(bishops)][bishopMagicIndex(occ, getlsbBitIndex(bishops))] & checkMask & ~board.occupancy[black] & pinMaskDiagonal;
			while (targetSquares) {
				Movelist::pushBack(Move::makemove(getLsbBit(bishops), getLsbBit(targetSquares), bBishop, wPawn, getLsbBit(targetSquares) & board.occupancy[white], false, false, false));

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
			map targetSquares = rookAttack[getlsbBitIndex(rooks)][rookMagicIndex(occ, getlsbBitIndex(rooks))] & checkMask & ~board.occupancy[black];
			while (targetSquares) {
				Movelist::pushBack(Move::makemove(getLsbBit(rooks), getLsbBit(targetSquares), bRook, wPawn, getLsbBit(targetSquares) & board.occupancy[white], false, false, false));

				targetSquares &= targetSquares - 1;
			}

			rooks &= rooks - 1;
		}

		//HV pinned 
		rooks = board.piece[bRook] & ~pinMaskDiagonal & pinMaskHV;
		while (rooks) {
			map targetSquares = rookAttack[getlsbBitIndex(rooks)][rookMagicIndex(occ, getlsbBitIndex(rooks))] & checkMask & ~board.occupancy[black] & pinMaskHV;
			while (targetSquares) {
				Movelist::pushBack(Move::makemove(getLsbBit(rooks), getLsbBit(targetSquares), bRook, wPawn, getLsbBit(targetSquares) & board.occupancy[white], false, false, false));

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
			map targetSquares = bishopAttack[getlsbBitIndex(queens)][bishopMagicIndex(occ, getlsbBitIndex(queens))] & checkMask & ~board.occupancy[black];
			while (targetSquares) {
				Movelist::pushBack(Move::makemove(getLsbBit(queens), getLsbBit(targetSquares), bQueen, wPawn, getLsbBit(targetSquares) & board.occupancy[white], false, false, false));

				targetSquares &= targetSquares - 1;
			}

			queens &= queens - 1;
		}

		//diagonally pinned 
		queens = board.piece[bQueen] & ~pinMaskHV & pinMaskDiagonal;
		while (queens) {
			map targetSquares = bishopAttack[getlsbBitIndex(queens)][bishopMagicIndex(occ, getlsbBitIndex(queens))] & checkMask & ~board.occupancy[black] & pinMaskDiagonal;
			while (targetSquares) {
				Movelist::pushBack(Move::makemove(getLsbBit(queens), getLsbBit(targetSquares), bQueen, wPawn, getLsbBit(targetSquares) & board.occupancy[white], false, false, false));

				targetSquares &= targetSquares - 1;
			}

			queens &= queens - 1;
		}


		//HVs 
		//non HV pinned 
		queens = board.piece[bQueen] & ~pinMaskDiagonal & ~pinMaskHV;
		while (queens) {
			map targetSquares = rookAttack[getlsbBitIndex(queens)][rookMagicIndex(occ, getlsbBitIndex(queens))] & checkMask & ~board.occupancy[black];
			while (targetSquares) {
				Movelist::pushBack(Move::makemove(getLsbBit(queens), getLsbBit(targetSquares), bQueen, wPawn, getLsbBit(targetSquares) & board.occupancy[white], false, false, false));

				targetSquares &= targetSquares - 1;
			}

			queens &= queens - 1;
		}

		//HV pinned 
		queens = board.piece[bQueen] & ~pinMaskDiagonal & pinMaskHV;
		while (queens) {
			map targetSquares = rookAttack[getlsbBitIndex(queens)][rookMagicIndex(occ, getlsbBitIndex(queens))] & checkMask & ~board.occupancy[black] & pinMaskHV;
			while (targetSquares) {
				Movelist::pushBack(Move::makemove(getLsbBit(queens), getLsbBit(targetSquares), bQueen, wPawn, getLsbBit(targetSquares) & board.occupancy[white], false, false, false));

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





