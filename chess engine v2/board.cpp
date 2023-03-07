#include "board.h"


/********************
*
*Utils
*
*********************/

static inline int bishopMagicIndex(const uint64_t occ, int index) {
	return ((occ & relevantBishopBlocker[index]) * bishopMagicNum[index]) >> (64 - getNumBit(relevantBishopBlocker[index]));
}

static inline int rookMagicIndex(const uint64_t occ, int index) {
	return ((occ & relevantRookBlocker[index]) * rookMagicNum[index]) >> (64 - getNumBit(relevantRookBlocker[index]));


	//return int((occ & relevantRookBlocker[index]) * rookMagicNum[index]) >> (64 - getNumBit(relevantRookBlocker[index]));
}


/********************
*
*BoardState
*
*********************/

BoardState::BoardState() 
	: side(new int{}), enpassant(new map{}), 
	castleRightWQ(new bool{}), castleRightWK(new bool{}), castleRightBQ(new bool{}), castleRightBK(new bool{}), 
	doubleCheck(new bool{}) {}

BoardState::BoardState(const BoardState& rhs) 
	: side(new int{*(rhs.side)}), enpassant(new map{*(rhs.enpassant)}), 
	castleRightWQ(new bool{*(rhs.castleRightWK)}), castleRightWK(new bool{*(rhs.castleRightWK)}), 
	castleRightBQ(new bool{*(rhs.castleRightBQ)}), castleRightBK(new bool{*(rhs.castleRightBK)}), 
	doubleCheck(new bool{*(rhs.doubleCheck)}) {}


BoardState& BoardState::operator=(BoardState rhs) noexcept {
	BoardState::swap(*this, rhs);


	rhs.side == nullptr; 
	rhs.enpassant = nullptr; 
	rhs.castleRightWQ = nullptr;
	rhs.castleRightWK = nullptr;
	rhs.castleRightBQ = nullptr;
	rhs.castleRightBK = nullptr;

	rhs.doubleCheck = nullptr;
}

void BoardState::debug() {
	Idebug();
}

void BoardState::Idebug() {
	std::cout << "side: " << *BoardState::side << "\n" <<
		"enpassant: " <<  char('A' + getlsbBitIndex(*BoardState::enpassant) % 8) << 8 - getlsbBitIndex(*BoardState::enpassant) / 8 << "\n" <<
		"doublecheck: " << (*BoardState::doubleCheck ? "true" : "false") << "\n"; 
	std::flush(std::cout);
}

void BoardState::swap(BoardState& lhs, BoardState& rhs) {
	std::swap(lhs.side, rhs.side); 
	std::swap(lhs.enpassant, rhs.enpassant); 
	std::swap(lhs.castleRightWK, rhs.castleRightWK);
	std::swap(lhs.castleRightWQ, rhs.castleRightWQ);
	std::swap(lhs.castleRightBK, rhs.castleRightBK); 
	std::swap(lhs.castleRightBQ, rhs.castleRightBQ); 
	std::swap(lhs.doubleCheck, rhs.doubleCheck); 
}


/********************
*
*Board
*singleton
*
*********************/

Board& Board::Get() {
	static Board instance; 
	return instance; 
}

void Board::print() {
	Get().Iprint(); 
}

void Board::Iprint() {

	std::cout << "\n";
	//board 
	for (int row = 0; row < 8; ++row) {

		std::cout << 8 - row << "   ";

		for (int file = 0; file < 8; ++file) {

			const int index = row * 8 + file;

			//pieces 
			for (int i = 0; i < 12; ++i) {
				if (piece[i] & indexSquare[index]) {
					std::cout << pieceChar[i];
					break; 
				}
			}
			
			if (!(indexSquare[index] & (occupancy[white] | occupancy[black]))) {
				std::cout << "0"; 
			}
			std::cout << " ";
		}
		std::cout << '\n';
	}
	std::cout << "\n";
	std::cout << "    a b c d e f g h";
	std::cout << "\n";

	//side 
	std::cout << "side: "; 
	if (BoardState::Get().side == white) std::cout << "white";
	else std::cout << "black"; 
	std::cout << "\n"; 

	//enpassant 
	std::cout << "enpassant: ";
	if (BoardState::Get().enpassant) {
		std::cout << 'A' + BoardState::Get().enpassant % 8;
		std::cout << BoardState::Get().enpassant / 8;
	}
	else std::cout << "--"; 
	std::cout << "\n"; 
	
	//castle rights 
	std::cout << "castle right: "; 
	if (BoardState::Get().castleRightWK) std::cout << "K";
	else std::cout << "-";
	if (BoardState::Get().castleRightWQ) std::cout << "Q";
	else std::cout << "-"; 
	if (BoardState::Get().castleRightBK) std::cout << "k";
	else std::cout << "-"; 
	if (BoardState::Get().castleRightBQ) std::cout << "q";
	else std::cout << "-"; 
	std::cout << "\n\n"; 

	std::flush(std::cout);
}


bool Board::attacked(const uint64_t square) {
	return (BoardState::Get().side == white ? Board::Get().IattackedWhite(square) : Board::Get().IattackedBlack(square) ); 
}

//square is attack while white's turn 
bool Board::IattackedWhite(const uint64_t square) {
	const int index = getlsbBitIndex(square); 


	return
		//leapers
		pawnAttack[white][index] & Board::Get().piece[bPawn] |
		knightAttack[index] & Board::Get().piece[bKnight] |
		kingAttack[index] & Board::Get().piece[bKing] |
		
		//sliders
		bishopAttack[index][bishopMagicIndex(Board::Get().occupancy[white] | Board::Get().occupancy[black], index)] & (Board::Get().piece[bBishop] | Board::Get().piece[bQueen]) | 
		rookAttack[index][rookMagicIndex(Board::Get().occupancy[white] | Board::Get().occupancy[black], index)] & (Board::Get().piece[bRook] | Board::Get().piece[bQueen]); 
}

//square is attacked while black's turn 
bool Board::IattackedBlack(const uint64_t square) {
	const int index = getlsbBitIndex(square); 
	return
		//leapers
		pawnAttack[black][index] & Board::Get().piece[wPawn] |
		knightAttack[index] & Board::Get().piece[wKnight] |
		kingAttack[index] & Board::Get().piece[wKing] |

		//sliders 
		bishopAttack[index][bishopMagicIndex(Board::Get().occupancy[white] | Board::Get().occupancy[black], index)] & (Board::Get().piece[wBishop] | Board::Get().piece[wQueen]) |
		rookAttack[index][rookMagicIndex(Board::Get().occupancy[white] | Board::Get().occupancy[black], index)] & (Board::Get().piece[wRook] | Board::Get().piece[wQueen]);
}

const uint64_t Board::checkMask() {
	return (BoardState::Get().side == white ? Board::Get().IcheckMaskWhite() : Board::Get().IcheckMaskBlack()); 
}

//return check mask for white's turn 
const uint64_t Board::IcheckMaskWhite() {
	//if (!Board::attacked(Board::Get().piece[wKing])) return 0x0ull; 

	const int index = getlsbBitIndex(Board::Get().piece[wKing]);
	const uint64_t occ = Board::Get().occupancy[white] | Board::Get().occupancy[black]; 
	uint64_t res = 0x0ull;
	int numChecks = 0; 

	//leaper pieces 
	res |= (pawnAttack[white][index] & Board::Get().piece[bPawn]);
	res |= (knightAttack[index] & Board::Get().piece[bKnight]);
	res |= (kingAttack[index] & Board::Get().piece[bKing]);
	if (res) numChecks += 1; 

	//slider pieces 
	//bishop (and queen) 
	{
		map mask = bishopAttack[index][bishopMagicIndex(occ, index)];
		map bishopMask = bishopAttack[index][bishopMagicIndex(occ, index)] & (Board::Get().piece[bBishop] | Board::Get().piece[bQueen]);
		if (bishopMask) numChecks += 1; 
		res |= bishopMask;

		while (bishopMask) {
			res |= (mask & bishopAttack[getlsbBitIndex(bishopMask)][bishopMagicIndex(occ, getlsbBitIndex(bishopMask))]);

			bishopMask &= bishopMask - 1;
		}
	}

	//rook (and queen) 
	{
		map mask = rookAttack[index][rookMagicIndex(occ, index)];
		map rookMask = rookAttack[index][rookMagicIndex(occ, index)] & (Board::Get().piece[bRook] | Board::Get().piece[bQueen]); 
		if (rookMask) numChecks += 1; 
		res |= rookMask; 

		while (rookMask) {
			res |= (mask & rookAttack[getlsbBitIndex(rookMask)][rookMagicIndex(occ, getlsbBitIndex(rookMask))]);

			rookMask &= rookMask - 1; 
		}
	}

	//double check 
	if (numChecks == 2) BoardState::Get().doubleCheck = true;
	else BoardState::Get().doubleCheck = false; 

	if (res == 0x0ull) return ~res;
	else return res; 
}
const uint64_t Board::IcheckMaskBlack() {

	const int index = getlsbBitIndex(Board::Get().piece[bKing]);
	const uint64_t occ = Board::Get().occupancy[white] | Board::Get().occupancy[black];
	uint64_t res = 0x0ull;
	int numChecks = 0;

	//leaper pieces 
	res |= (pawnAttack[black][index] & Board::Get().piece[wPawn]);
	res |= (knightAttack[index] & Board::Get().piece[wKnight]);
	res |= (kingAttack[index] & Board::Get().piece[wKing]);
	if (res) numChecks += 1;

	//slider pieces 
	//bishop (and queen) 
	{
		map mask = bishopAttack[index][bishopMagicIndex(occ, index)];
		map bishopMask = bishopAttack[index][bishopMagicIndex(occ, index)] & (Board::Get().piece[wBishop] | Board::Get().piece[wQueen]);
		if (bishopMask) numChecks += 1;
		res |= bishopMask;

		while (bishopMask) {
			res |= (mask & bishopAttack[getlsbBitIndex(bishopMask)][bishopMagicIndex(occ, getlsbBitIndex(bishopMask))]);

			bishopMask &= bishopMask - 1;
		}
	}

	//rook (and queen) 
	{
		map mask = rookAttack[index][rookMagicIndex(occ, index)];
		map rookMask = rookAttack[index][rookMagicIndex(occ, index)] & (Board::Get().piece[wRook] | Board::Get().piece[wQueen]);
		if (rookMask) numChecks += 1;
		res |= rookMask;

		while (rookMask) {
			res |= (mask & rookAttack[getlsbBitIndex(rookMask)][rookMagicIndex(occ, getlsbBitIndex(rookMask))]);

			rookMask &= rookMask - 1;
		}
	}

	//double check 
	if (numChecks == 2) BoardState::Get().doubleCheck = true;
	else BoardState::Get().doubleCheck = false;

	return res;
}

const uint64_t Board::pinMaskDiagonal() {
	return (BoardState::Get().side == white ? Board::Get().IpinMaskDiagonalWhite() : Board::Get().IpinMaskDiagonalBlack()); 
}

//pin mask during white's turn (when white is about to make move) 
const uint64_t Board::IpinMaskDiagonalWhite() {

	const int index = getlsbBitIndex(Board::Get().piece[wKing]); 
	const uint64_t occ = Board::Get().occupancy[white] | Board::Get().occupancy[black]; 
	uint64_t res = 0x0ull; 

	//potential bishop pin 
	map potentialBishopPinnedPiece = bishopAttack[index][bishopMagicIndex(occ, index)] & (Board::Get().occupancy[white]); 

	//iterate through each potential pin and then create a pin mask 
	while (potentialBishopPinnedPiece) {
		const int pinnedIndex = getlsbBitIndex(potentialBishopPinnedPiece); 
		
		//potential bits are just bishop and queen of the other color for the second 
		map potentialBishopPinAttacker = bishopAttack[pinnedIndex][bishopMagicIndex(occ, pinnedIndex)] & (Board::Get().piece[bBishop] | Board::Get().piece[bQueen]); 
		while (potentialBishopPinAttacker) {
			const int attackerIndex = getlsbBitIndex(potentialBishopPinAttacker); 
			const map uocc = occ & ~indexSquare[pinnedIndex]; 

			if (bishopAttack[attackerIndex][bishopMagicIndex(uocc, attackerIndex)] & Board::Get().piece[wKing]) {
				res |= (bishopAttack[attackerIndex][bishopMagicIndex(uocc, attackerIndex)] & bishopAttack[index][bishopMagicIndex(uocc, index)]);
				res |= indexSquare[attackerIndex];
			}

			potentialBishopPinAttacker &= potentialBishopPinAttacker - 1; 
		}

		potentialBishopPinnedPiece &= potentialBishopPinnedPiece - 1; 
	}

	return res; 
}

const uint64_t Board::IpinMaskDiagonalBlack() {
	
	const int index = getlsbBitIndex(Board::Get().piece[bKing]);
	const uint64_t occ = Board::Get().occupancy[white] | Board::Get().occupancy[black];
	uint64_t res = 0x0ull;

	//potential bishop pin 
	map potentialBishopPinnedPiece = bishopAttack[index][bishopMagicIndex(occ, index)] & (Board::Get().occupancy[black]);

	//iterate through each potential pin and then create a pin mask 
	while (potentialBishopPinnedPiece) {
		const int pinnedIndex = getlsbBitIndex(potentialBishopPinnedPiece);

		//potential bits are just bishop and queen of the other color for the second 
		map potentialBishopPinAttacker = bishopAttack[pinnedIndex][bishopMagicIndex(occ, pinnedIndex)] & (Board::Get().piece[wBishop] | Board::Get().piece[wQueen]);
		while (potentialBishopPinAttacker) {
			const int attackerIndex = getlsbBitIndex(potentialBishopPinAttacker);
			const map uocc = occ & ~indexSquare[pinnedIndex];

			if (bishopAttack[attackerIndex][bishopMagicIndex(uocc, attackerIndex)] & Board::Get().piece[wKing]) {
				res |= (bishopAttack[attackerIndex][bishopMagicIndex(uocc, attackerIndex)] & bishopAttack[index][bishopMagicIndex(uocc, index)]);
				res |= indexSquare[attackerIndex];
			}

			potentialBishopPinAttacker &= potentialBishopPinAttacker - 1;
		}

		potentialBishopPinnedPiece &= potentialBishopPinnedPiece - 1;
	}

	return res;
}

const uint64_t Board::pinMaskHV() {
	return (BoardState::Get().side == white ? Board::Get().IpinMaskHVWhite() : Board::Get().IpinMaskHVBlack()); 
}

//horizontal/veritcal pinmask during white's turn 
const uint64_t Board::IpinMaskHVWhite() {
	const int index = getlsbBitIndex(Board::Get().piece[wKing]);
	const uint64_t occ = Board::Get().occupancy[white] | Board::Get().occupancy[black];
	uint64_t res = 0x0ull;

	//potential bishop pin 
	map potentialRookPinnedPiece = rookAttack[index][rookMagicIndex(occ, index)] & (Board::Get().occupancy[white]);

	//iterate through each potential pin and then create a pin mask 
	while (potentialRookPinnedPiece) {
		const int pinnedIndex = getlsbBitIndex(potentialRookPinnedPiece);

		//potential bits are just bishop and queen of the other color for the second 
		map potentialRookPinAttacker = rookAttack[pinnedIndex][rookMagicIndex(occ, pinnedIndex)] & (Board::Get().piece[bRook] | Board::Get().piece[bQueen]);
		while (potentialRookPinAttacker) {
			const int attackerIndex = getlsbBitIndex(potentialRookPinAttacker);
			const map uocc = occ & ~indexSquare[pinnedIndex];

			if (rookAttack[attackerIndex][rookMagicIndex(uocc, attackerIndex)] & Board::Get().piece[wKing]) {
				res |= (rookAttack[attackerIndex][rookMagicIndex(uocc, attackerIndex)] & rookAttack[index][rookMagicIndex(uocc, index)]);
				res |= indexSquare[attackerIndex];
			}

			potentialRookPinAttacker &= potentialRookPinAttacker - 1;
		}

		potentialRookPinnedPiece &= potentialRookPinnedPiece - 1;
	}

	return res;	
}

//return horizontal/veritcal pin mask during black's turn 
const uint64_t Board::IpinMaskHVBlack() {
	const int index = getlsbBitIndex(Board::Get().piece[bKing]);
	const uint64_t occ = Board::Get().occupancy[white] | Board::Get().occupancy[black];
	uint64_t res = 0x0ull;

	//potential bishop pin 
	map potentialRookPinnedPiece = rookAttack[index][rookMagicIndex(occ, index)] & (Board::Get().occupancy[black]);

	//iterate through each potential pin and then create a pin mask 
	while (potentialRookPinnedPiece) {
		const int pinnedIndex = getlsbBitIndex(potentialRookPinnedPiece);

		//potential bits are just bishop and queen of the other color for the second 
		map potentialRookPinAttacker = rookAttack[pinnedIndex][rookMagicIndex(occ, pinnedIndex)] & (Board::Get().piece[wRook] | Board::Get().piece[wQueen]);
		while (potentialRookPinAttacker) {
			const int attackerIndex = getlsbBitIndex(potentialRookPinAttacker);
			const map uocc = occ & ~indexSquare[pinnedIndex];

			if (rookAttack[attackerIndex][rookMagicIndex(uocc, attackerIndex)] & Board::Get().piece[bKing]) {
				res |= (rookAttack[attackerIndex][rookMagicIndex(uocc, attackerIndex)] & rookAttack[index][rookMagicIndex(uocc, index)]);
				res |= indexSquare[attackerIndex];
			}

			potentialRookPinAttacker &= potentialRookPinAttacker - 1;
		}

		potentialRookPinnedPiece &= potentialRookPinnedPiece - 1;
	}

	return res;
}


/********************
*
*Fen
*singleton
*
*********************/

Fen& Fen::Get() {
	static Fen instance; 
	return instance; 
}


void Fen::clear(Board& board, BoardState& boardState) {
	Get().Iclear(board, boardState); 
}

void Fen::parse(const std::string& fen, Board& board, BoardState& boardState) {
	Get().clear(board, boardState); 
	Get().Iparse(fen, board, boardState); 
}

void Fen::parseStartPosition(Board& board, BoardState& boardState) {
	Get().IparseStartPosition(board, boardState); 
}


void Fen::Iclear(Board& board, BoardState& boardState) {
	//BoardState
	boardState.side = white; 
	boardState.enpassant = 0x0ull; 
	boardState.castleRightWQ = false;
	boardState.castleRightWK = false;
	boardState.castleRightBQ = false;
	boardState.castleRightBK = false;

	boardState.doubleCheck = false;


	//Board
	for (int i = 0; i < 12; ++i) {
		board.piece[i] = 0x0ull; 
	}

	for (int i = 0; i < 2; ++i) {
		board.occupancy[i] = 0x0ull; 
	}
}

void Fen::Iparse(const std::string& fen, Board& board, BoardState& boardState) {
	int index = 0; 

	int positionIndex = 0; 
	while (fen[index] != ' ') {

		//black piece 
		if ('a' <= fen[index] && fen[index] <= 'z') {
			switch (fen[index]) {
			case 'p':
				Board::Get().piece[bPawn] |= indexSquare[positionIndex];
				break;
			case 'n':
				Board::Get().piece[bKnight] |= indexSquare[positionIndex];
				break;
			case 'b':
				Board::Get().piece[bBishop] |= indexSquare[positionIndex];
				break;
			case 'r':
				Board::Get().piece[bRook] |= indexSquare[positionIndex];
				break;
			case 'q':
				Board::Get().piece[bQueen] |= indexSquare[positionIndex];
				break;
			case 'k':
				Board::Get().piece[bKing] |= indexSquare[positionIndex];
				break;
			}

			//set occupancy 
			Board::Get().occupancy[black] |= indexSquare[positionIndex];

			//increment index 
			++positionIndex;
		}
			
		
		//white piece 
		else if ('A' <= fen[index] && fen[index] <= 'Z') {
			switch (fen[index]) {
			case 'P':
				Board::Get().piece[wPawn] |= indexSquare[positionIndex];
				break;
			case 'N':
				Board::Get().piece[wKnight] |= indexSquare[positionIndex];
				break;
			case 'B':
				Board::Get().piece[wBishop] |= indexSquare[positionIndex];
				break;
			case 'R':
				Board::Get().piece[wRook] |= indexSquare[positionIndex];
				break;
			case 'Q':
				Board::Get().piece[wQueen] |= indexSquare[positionIndex];
				break;
			case 'K':
				Board::Get().piece[wKing] |= indexSquare[positionIndex];
				break;
			}
			
			//set occupancy 
			Board::Get().occupancy[white] |= indexSquare[positionIndex];

			//increment index 
			++positionIndex;
			
		}

		//number 
		else if ('0' <= fen[index] && fen[index] <= '9'){ 
			positionIndex += fen[index] - '0';
		}

		++index;

		//increment index 
	}
	++index;
	//std::cout << "index: " << index << std::endl;

	//side t0 move 
	if (fen[index] == 'b') BoardState::Get().side = black;
	else BoardState::Get().side = white;
	index += 2; 

	//std::cout << "index: " << index << std::endl; 
	
	//castling rights 
	while (fen[index] != ' ') {
		switch (fen[index]) {
		case 'K':
			BoardState::Get().castleRightWK = true;
			break;
		case 'Q':
			BoardState::Get().castleRightWQ = true;
			break;
		case 'k':
			BoardState::Get().castleRightBK = true;
			break;
		case 'q':
			BoardState::Get().castleRightBQ = true;
			break;
		}
		++index;
	}
	++index;

	//enpassant square 
	int enpassantStart = index; 
	while (fen[index] != ' ') {
		++index;
	}
	if (index - enpassantStart == 2) {
		//int rank = fen[enpassantStart + 1] - '0';
		//int file = fen[enpassantStart] - 'a';

		//BoardState::Get().enpassant = 8 * (8 - rank) + file;
	}

}

void Fen::IparseStartPosition() {
	parse("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ");
}


