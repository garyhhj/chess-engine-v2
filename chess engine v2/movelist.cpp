#include "movelist.h"

static consteval auto compileTime(auto val) {
	return val;
}

/********************
*
*Move
*
*********************/

/*
		 binary move bits                               hexidecimal constants

   0000 0000 0000 0000 0011 1111    source square       0x3f
   0000 0000 0000 1111 1100 0000    target square       0xfc0
   0000 0000 1111 0000 0000 0000    piece               0xf000
   0000 1111 0000 0000 0000 0000    promoted piece      0xf0000
   0001 0000 0000 0000 0000 0000    capture flag        0x100000
   0010 0000 0000 0000 0000 0000    double push flag    0x200000
   0100 0000 0000 0000 0000 0000    enpassant flag      0x400000
   1000 0000 0000 0000 0000 0000    castling flag       0x800000
*/

move Move::makemove(int sourceSquare, int targetSquare, int piece, int promotePiece, bool captureFlag, bool doublePushFlag, bool enpassantFlag, bool castlingFlag) {
	return (sourceSquare & 0x3f) | ((targetSquare << 6) & 0xfc0) | ((piece << 12) & 0xf000) | ((promotePiece << 16) & 0xf0000) |
		(captureFlag ? 0x1 << 20 : 0x0) | (doublePushFlag ? 0x1 << 21 : 0x0) | (enpassantFlag ? 0x1 << 22 : 0x0) | (castlingFlag ? 0x1 << 23 : 0x0);

}

move Move::makemove(uint64_t sourceSquare, uint64_t targetSquare, int piece, int promotePiece, bool captureFlag, bool doublePushFlag, bool enpassantFlag, bool castlingFlag) {
	return (getlsbBitIndex(sourceSquare) & 0x3f) | ((getlsbBitIndex(targetSquare) << 6) & 0xfc0) | ((piece << 12) & 0xf000) | ((promotePiece << 16) & 0xf0000) |
		(captureFlag ? 0x1 << 20 : 0x0) | (doublePushFlag ? 0x1 << 21 : 0x0) | (enpassantFlag ? 0x1 << 22 : 0x0) | (castlingFlag ? 0x1 << 23 : 0x0);
}

void Move::decode(const move m) {
	
	std::cout << "move: ";

	//sourceSquare 
	std::cout << "SS: "; 
	//std::cout << Move::sourceSquare(m) << std::endl; 

	//this is wrong and needs to change 

	//A8 is 0
	//H1 is 63 
	std::cout << char('A' + Move::sourceSquare(m) % 8); 
	std::cout << 8 - Move::sourceSquare(m) / 8 << " ";

	//target Square 
	std::cout << "TS: "; 
	std::cout << char('A' + Move::targetSquare(m) % 8); 
	std::cout << 8 - Move::targetSquare(m) / 8 << " ";

	//piece
	std::cout << pieceChar[Move::piece(m)] << " ";

	//promote piece 
	std::cout << pieceChar[Move::promotePiece(m)] << " ";

	//capture 
	std::cout << (Move::captureFlag(m) ? 'c' : '-');
	std::cout << " "; 

	//double push 
	std::cout << (Move::doublePushFlag(m) ? "dp" : "--");
	std::cout << " "; 

	//enpassant 
	std::cout << (Move::enpassantFlag(m) ? 'e' : '-');
	std::cout << " "; 

	//castle 
	std::cout << (Move::castlingFlag(m) ? 'c' : '-'); 
	std::cout << " "; 
}

constexpr int Move::sourceSquare(const move m) { return m & 0x3f; }
constexpr int Move::targetSquare(const move m) { return (m & 0xfc0) >> 6; }
constexpr int Move::piece(const move m) { return (m & 0xf000) >> 12; }
constexpr int Move::promotePiece(const move m) { return (m & 0xf0000) >> 16; }

constexpr bool Move::captureFlag(const move m) { return m & 0x100000; }
constexpr bool Move::doublePushFlag(const move m) { return m & 0x200000; }
constexpr bool Move::enpassantFlag(const move m) { return m & 0x400000; }
constexpr bool Move::castlingFlag(const move m) { return m & 0x800000; }

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
	std::cout << "index: " << index << std::endl; 

	for (int i = 0; i < Movelist::index; ++i) {
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
			map attacks = knightAttack[getlsbBitIndex(knights)] & checkMask;
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

}

void Movelist::moveGenBlack(const Board& board, BoardState& boardState) {
	return; 
}





