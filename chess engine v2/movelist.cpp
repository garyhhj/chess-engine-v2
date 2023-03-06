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
	return (sourceSquare & 0x3f) | (targetSquare << 6 & 0xfc0) | (piece << 12 & 0xf000) | (promotePiece << 16 & 0xf0000) |
		(captureFlag ? 0x1 << 20 : 0x0) | (doublePushFlag ? 0x1 << 21 : 0x0) | (enpassantFlag ? 0x1 << 22 : 0x0) | (castlingFlag ? 0x1 << 23 : 0x0);

}

move Move::makemove(uint64_t sourceSquare, uint64_t targetSquare, int piece, int promotePiece, bool captureFlag, bool doublePushFlag, bool enpassantFlag, bool castlingFlag) {
	return (getlsbBitIndex(sourceSquare) & 0x3f) | (getlsbBitIndex(targetSquare) << 6 & 0xfc0) | (piece << 12 & 0xf000) | (promotePiece << 16 & 0xf0000) |
		(captureFlag ? 0x1 << 20 : 0x0) | (doublePushFlag ? 0x1 << 21 : 0x0) | (enpassantFlag ? 0x1 << 22 : 0x0) | (castlingFlag ? 0x1 << 23 : 0x0);
}

void Move::decode(const move m) {
	//print out everything in order?
	
	using namespace std;
	std::cout << "move: ";

	//sourceSquare 
	std::cout << "SS: "; 
	std::cout << 'A' + Move::sourceSquare(m) % 8; 
	std::cout << Move::sourceSquare(m) / 8; 

	//target Square 
	std::cout << "TS: "; 
	std::cout << 'A' + Move::targetSquare(m) % 8; 
	std::cout << Move::targetSquare(m) / 8; 

	//piece
	std::cout << pieceChar[Move::piece(m)]; 

	//promote piece 
	std::cout << pieceChar[Move::promotePiece(m)]; 

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
constexpr int Move::targetSquare(const move m) { return m & 0xfc0 >> 6; }
constexpr int Move::piece(const move m) { return m & 0xf000 >> 12; }
constexpr int Move::promotePiece(const move m) { return m & 0xf0000 >> 16; }

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
	std::cout << "destructor" << std::endl; 
	delete[] movelist; 
}

constexpr void Movelist::pushBack(const move m) {
	movelist[index++] = m; 
}

void Movelist::print() {
	
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
void Movelist::moveGen(const Board& board, const BoardState& boardState) {
	boardState.Get().side == white ? 
		Movelist::moveGenWhite(board, boardState) : 
		Movelist::moveGenBlack(board, boardState);
}

void Movelist::moveGenWhite(const Board& board, const BoardState& boardState) {

	//get the check mask 
	//get the pin masks 
	const map checkMask = Board::Get().checkMask(); 
	//const bool doubleCheck = BoardState::Get().doubleCheck; 
	
	const map pinMaskHV = Board::Get().pinMaskHV(); 
	const map pinMaskDiagonal = Board::Get().pinMaskDiagonal(); 

	//start generating moves 
	//now generate moves 


	//for checkMask... it means that the pieces must be resulting in the checkmask 
	//if there are double checks, then can only move king  

	//for pin masks... it means that the pinned piece can only move within the pinned area 
	//prevent pawn captures, certain bishop diagonals, and certain rook moves in pinned pieces 
	//for leaper pieces, can just not let them move (except king) /but king won't be pinned, it will be checked 
	//for slider pieces, if same pin as move eg. bishop and diagonal, allow movement and only check against diagonal 
	//for slider pieces, if different pin, then it cannot be moved 

	//anyways write it piece by piece and lets see what happens 

	//double check - can only move king 
	if (BoardState::Get().doubleCheck) {

		const int kingIndex = getlsbBitIndex(Board::Get().piece[wKing]); 

		//do something that checks other attacked square 

		//king move noncapture 
		map potentialKingMoveNonCapture = kingAttack[kingIndex] & ~(Board::Get().occupancy[black] & Board::Get().occupancy[white]); 
		while (potentialKingMoveNonCapture) {
			//need to check the square for attack... if no attack then can add to stuff 
			if (!Board::Get().attacked(getLsbBit(potentialKingMoveNonCapture))) {
				Movelist::pushBack(Move::makemove(indexSquare[kingIndex], getLsbBit(potentialKingMoveNonCapture), wKing, wPawn, false, false, false, false)); 
			}
			potentialKingMoveNonCapture &= potentialKingMoveNonCapture - 1; 
		}


		//king move capture 
		map potentialKingMoveCapture = kingAttack[kingIndex] & Board::Get().occupancy[black];
		while (potentialKingMoveCapture) {
			//need to check the square for attack... if no attack then can add to stuff 
			if (!Board::Get().attacked(getLsbBit(potentialKingMoveCapture))) {
				Movelist::pushBack(Move::makemove(indexSquare[kingIndex], getLsbBit(potentialKingMoveNonCapture), wKing, wPawn, true, false, false, false));
			}


			potentialKingMoveCapture &= potentialKingMoveCapture - 1; 
		}



		//move to another square 
		//then I need to check if the other square is being attacked? 
		

		return;
	}


	//wPawn 
	//resulting white pawns 
	//board.Get().piece[wPawn] << 8 & ~(board.Get().occupancy[white] | board.Get().occupancy[black]); 

	//create attack map for pieces that can't be moved/pinned? 
	
	//pawn push non promotion 
	{
		map pawns = board.Get().piece[wPawn] << 8 & ~(board.Get().occupancy[white] | board.Get().occupancy[black]) & ~Row8;
		while (pawns){
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) << 8, getLsbBit(pawns), wPawn, wPawn, false, false, false, false));

			pawns &= pawns - 1; 
		}
	}

	//pawn push promotion 
	{
		map pawns = board.Get().piece[wPawn] << 8 & ~(board.Get().occupancy[white] | board.Get().occupancy[black]) & Row8;
		while (pawns) {
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) << 8, getLsbBit(pawns), wPawn, wKnight, false, false, false, false));
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) << 8, getLsbBit(pawns), wPawn, wRook, false, false, false, false));
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) << 8, getLsbBit(pawns), wPawn, wBishop, false, false, false, false));
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) << 8, getLsbBit(pawns), wPawn, wQueen, false, false, false, false));

			pawns &= pawns - 1;
		}
	}

	//pawn double push 
	{
		map pawns = (board.Get().piece[wPawn] << 8 & ~(board.Get().occupancy[white] | board.Get().occupancy[black])) << 8 & ~(board.Get().occupancy[white] | board.Get().occupancy[black]) & Row4;
		while (pawns) {
			Movelist::pushBack(Move::makemove(getLsbBit(pawns) << 8, getLsbBit(pawns), wPawn, wPawn, false, true, false, false));
			
			pawns &= pawns - 1; 
		}
	}

	//pawn captures without promotion 
	//iterate through pawns and check for captures? 
	//current set of pawns would be on source square 
	//create copy of current set of pawns and just iterate? 

	//pawn captures with promotion 
	
	//knight moves 
	{
		map knights = board.Get().piece[wKnight]; 
		while (knights) {
			map attacks = knightAttack[getlsbBitIndex(knights)];
			while (attacks) {
				Movelist::pushBack(Move::makemove(getLsbBit(knights), getLsbBit(attacks), wKnight, wPawn, getLsbBit(attacks) & Board::Get().occupancy[black], false, false, false)); 

				attacks &= attacks - 1; 
			}
			knights &= knights - 1; 
		}
	}
	
	//king moves 
	{
		map attacks = kingAttack[getlsbBitIndex(Board::Get().piece[wKing])]; 
		while (attacks) {
			Movelist::pushBack(Move::makemove(Board::Get().piece[wKing], getLsbBit(attacks), wKing, wPawn, getLsbBit(attacks) & Board::Get().occupancy[black], false, false, false)); 

			attacks &= attacks - 1; 
		}
	}

}

void Movelist::moveGenBlack(const Board& board, const BoardState& boardState) {

}





