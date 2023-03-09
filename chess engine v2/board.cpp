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

	return *this; 
}

BoardState::~BoardState() {
	delete BoardState::side;
	delete BoardState::enpassant;

	delete BoardState::castleRightBK;
	delete BoardState::castleRightBQ;
	delete BoardState::castleRightWK;
	delete BoardState::castleRightWQ;
	delete BoardState::doubleCheck;
}


void BoardState::debug() {
	Idebug();
}

void BoardState::Idebug() {
	std::cout << "side: " << (*BoardState::side == white ? "white" : "black") << "\n" <<
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
*
*********************/

Board::Board() : piece(new map[12]), occupancy(new map[2]) {}

Board::Board(const Board& rhs) 
	: piece(new map[12]{rhs.piece[0], rhs.piece[1], rhs.piece[2], rhs.piece[3], rhs.piece[4], rhs.piece[5], rhs.piece[6], rhs.piece[7], rhs.piece[8], rhs.piece[9], rhs.piece[10], rhs.piece[11]}),
	occupancy(new map[2]{rhs.occupancy[0], rhs.occupancy[1]}) {};

Board& Board::operator=(Board& rhs) {
	std::swap(this->piece, rhs.piece); 
	std::swap(this->occupancy, rhs.occupancy); 

	rhs.piece = nullptr; 
	rhs.occupancy = nullptr; 

	return *this; 
}

Board::~Board() {
	delete[] Board::piece; 
	delete[] Board::occupancy; 
}

void Board::print(const BoardState& boardState) const {
	Iprint(boardState);
}

void Board::Iprint(const BoardState& boardState) const{

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
	if (*(boardState.side) == white) std::cout << "white";
	else std::cout << "black"; 
	std::cout << "\n"; 

	//enpassant 
	std::cout << "enpassant: ";
	if (*boardState.enpassant) {
		std::cout << char('A' + getlsbBitIndex(*boardState.enpassant) % 8);
		std::cout << 8 - getlsbBitIndex(*boardState.enpassant) / 8;
	}
	else std::cout << "--"; 
	std::cout << "\n"; 
	
	//castle rights 
	std::cout << "castle right: "; 
	if (*boardState.castleRightWK) std::cout << "K";
	else std::cout << "-";
	if (*boardState.castleRightWQ) std::cout << "Q";
	else std::cout << "-"; 
	if (*boardState.castleRightBK) std::cout << "k";
	else std::cout << "-"; 
	if (*boardState.castleRightBQ) std::cout << "q";
	else std::cout << "-"; 
	std::cout << "\n\n"; 

	std::flush(std::cout);
}


bool Board::attacked(const uint64_t square, const BoardState& boardState) const{
	return (*boardState.side == white ? Board::IattackedWhite(square) : Board::IattackedBlack(square) ); 
}

//square is attack while white's turn 
bool Board::IattackedWhite(const uint64_t square) const{
	const int index = getlsbBitIndex(square); 


	return
		//leapers
		pawnAttack[white][index] & Board::piece[bPawn] |
		knightAttack[index] & Board::piece[bKnight] |
		kingAttack[index] & Board::piece[bKing] |
		
		//sliders
		bishopAttack[index][bishopMagicIndex(Board::occupancy[white] | Board::occupancy[black], index)] & (Board::piece[bBishop] | Board::piece[bQueen]) | 
		rookAttack[index][rookMagicIndex(Board::occupancy[white] | Board::occupancy[black], index)] & (Board::piece[bRook] | Board::piece[bQueen]); 
}

//square is attacked while black's turn 
bool Board::IattackedBlack(const uint64_t square) const{
	const int index = getlsbBitIndex(square); 
	return
		//leapers
		pawnAttack[black][index] & Board::piece[wPawn] |
		knightAttack[index] & Board::piece[wKnight] |
		kingAttack[index] & Board::piece[wKing] |

		//sliders 
		bishopAttack[index][bishopMagicIndex(Board::occupancy[white] | Board::occupancy[black], index)] & (Board::piece[wBishop] | Board::piece[wQueen]) |
		rookAttack[index][rookMagicIndex(Board::occupancy[white] | Board::occupancy[black], index)] & (Board::piece[wRook] | Board::piece[wQueen]);
}

const uint64_t Board::checkMask(BoardState& boardState) const{
	return (*boardState.side == white ? Board::IcheckMaskWhite(boardState) : Board::IcheckMaskBlack(boardState)); 
}

//return check mask for white's turn 
const uint64_t Board::IcheckMaskWhite(BoardState& boardState) const{
	//if (!Board::attacked(Board::Get().piece[wKing])) return 0x0ull; 

	const int index = getlsbBitIndex(Board::piece[wKing]);
	const uint64_t occ = Board::occupancy[white] | Board::occupancy[black]; 
	uint64_t res = 0x0ull;
	int numChecks = 0; 

	//leaper pieces 
	res |= (pawnAttack[white][index] & Board::piece[bPawn]);
	res |= (knightAttack[index] & Board::piece[bKnight]);
	res |= (kingAttack[index] & Board::piece[bKing]);
	if (res) numChecks += 1; 

	//slider pieces 
	//bishop (and queen) 
	{
		map mask = bishopAttack[index][bishopMagicIndex(occ, index)];
		map bishopMask = bishopAttack[index][bishopMagicIndex(occ, index)] & (Board::piece[bBishop] | Board::piece[bQueen]);
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
		map rookMask = rookAttack[index][rookMagicIndex(occ, index)] & (Board::piece[bRook] | Board::piece[bQueen]); 
		if (rookMask) numChecks += 1; 
		res |= rookMask; 

		while (rookMask) {
			res |= (mask & rookAttack[getlsbBitIndex(rookMask)][rookMagicIndex(occ, getlsbBitIndex(rookMask))]);

			rookMask &= rookMask - 1; 
		}
	}

	//double check 
	if (numChecks == 2) *boardState.doubleCheck = true;
	else *boardState.doubleCheck = false; 

	if (res == 0x0ull) return ~res;
	else return res; 
}
const uint64_t Board::IcheckMaskBlack(BoardState& boardState) const{

	const int index = getlsbBitIndex(Board::piece[bKing]);
	const uint64_t occ = Board::occupancy[white] | Board::occupancy[black];
	uint64_t res = 0x0ull;
	int numChecks = 0;

	//leaper pieces 
	res |= (pawnAttack[black][index] & Board::piece[wPawn]);
	res |= (knightAttack[index] & Board::piece[wKnight]);
	res |= (kingAttack[index] & Board::piece[wKing]);
	if (res) numChecks += 1;

	//slider pieces 
	//bishop (and queen) 
	{
		map mask = bishopAttack[index][bishopMagicIndex(occ, index)];
		map bishopMask = bishopAttack[index][bishopMagicIndex(occ, index)] & (Board::piece[wBishop] | Board::piece[wQueen]);
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
		map rookMask = rookAttack[index][rookMagicIndex(occ, index)] & (Board::piece[wRook] | Board::piece[wQueen]);
		if (rookMask) numChecks += 1;
		res |= rookMask;

		while (rookMask) {
			res |= (mask & rookAttack[getlsbBitIndex(rookMask)][rookMagicIndex(occ, getlsbBitIndex(rookMask))]);

			rookMask &= rookMask - 1;
		}
	}

	//double check 
	if (numChecks == 2) *boardState.doubleCheck = true;
	else *boardState.doubleCheck = false;

	if (res == 0x0ull) return ~res;
	else return res;
}

const uint64_t Board::pinMaskDiagonal(const BoardState& boardState) const{
	return (*boardState.side == white ? Board::IpinMaskDiagonalWhite() : Board::IpinMaskDiagonalBlack()); 
}

//pin mask during white's turn (when white is about to make move) 
const uint64_t Board::IpinMaskDiagonalWhite() const{

	const int index = getlsbBitIndex(Board::piece[wKing]); 
	const uint64_t occ = Board::occupancy[white] | Board::occupancy[black]; 
	uint64_t res = 0x0ull; 

	//potential bishop pin 
	map potentialBishopPinnedPiece = bishopAttack[index][bishopMagicIndex(occ, index)] & (Board::occupancy[white]); 

	//iterate through each potential pin and then create a pin mask 
	while (potentialBishopPinnedPiece) {
		const int pinnedIndex = getlsbBitIndex(potentialBishopPinnedPiece); 
		
		//potential bits are just bishop and queen of the other color for the second 
		map potentialBishopPinAttacker = bishopAttack[pinnedIndex][bishopMagicIndex(occ, pinnedIndex)] & (Board::piece[bBishop] | Board::piece[bQueen]); 
		while (potentialBishopPinAttacker) {
			const int attackerIndex = getlsbBitIndex(potentialBishopPinAttacker); 
			const map uocc = occ & ~indexSquare[pinnedIndex]; 

			if (bishopAttack[attackerIndex][bishopMagicIndex(uocc, attackerIndex)] & Board::piece[wKing]) {
				res |= (bishopAttack[attackerIndex][bishopMagicIndex(uocc, attackerIndex)] & bishopAttack[index][bishopMagicIndex(uocc, index)]);
				res |= indexSquare[attackerIndex];
			}

			potentialBishopPinAttacker &= potentialBishopPinAttacker - 1; 
		}

		potentialBishopPinnedPiece &= potentialBishopPinnedPiece - 1; 
	}

	return res; 
}

const uint64_t Board::IpinMaskDiagonalBlack() const{
	
	const int index = getlsbBitIndex(Board::piece[bKing]);
	const uint64_t occ = Board::occupancy[white] | Board::occupancy[black];
	uint64_t res = 0x0ull;

	//potential bishop pin 
	map potentialBishopPinnedPiece = bishopAttack[index][bishopMagicIndex(occ, index)] & (Board::occupancy[black]);

	//iterate through each potential pin and then create a pin mask 
	while (potentialBishopPinnedPiece) {
		const int pinnedIndex = getlsbBitIndex(potentialBishopPinnedPiece);

		//potential bits are just bishop and queen of the other color for the second 
		map potentialBishopPinAttacker = bishopAttack[pinnedIndex][bishopMagicIndex(occ, pinnedIndex)] & (Board::piece[wBishop] | Board::piece[wQueen]);
		while (potentialBishopPinAttacker) {
			const int attackerIndex = getlsbBitIndex(potentialBishopPinAttacker);
			const map uocc = occ & ~indexSquare[pinnedIndex];

			if (bishopAttack[attackerIndex][bishopMagicIndex(uocc, attackerIndex)] & Board::piece[wKing]) {
				res |= (bishopAttack[attackerIndex][bishopMagicIndex(uocc, attackerIndex)] & bishopAttack[index][bishopMagicIndex(uocc, index)]);
				res |= indexSquare[attackerIndex];
			}

			potentialBishopPinAttacker &= potentialBishopPinAttacker - 1;
		}

		potentialBishopPinnedPiece &= potentialBishopPinnedPiece - 1;
	}

	return res;
}

const uint64_t Board::pinMaskHV(const BoardState& boardState) const{
	return (*boardState.side == white ? Board::IpinMaskHVWhite() : Board::IpinMaskHVBlack()); 
}

//horizontal/veritcal pinmask during white's turn 
const uint64_t Board::IpinMaskHVWhite() const{
	const int index = getlsbBitIndex(Board::piece[wKing]);
	const uint64_t occ = Board::occupancy[white] | Board::occupancy[black];
	uint64_t res = 0x0ull;

	//potential bishop pin 
	map potentialRookPinnedPiece = rookAttack[index][rookMagicIndex(occ, index)] & (Board::occupancy[white]);

	//iterate through each potential pin and then create a pin mask 
	while (potentialRookPinnedPiece) {
		const int pinnedIndex = getlsbBitIndex(potentialRookPinnedPiece);

		//potential bits are just bishop and queen of the other color for the second 
		map potentialRookPinAttacker = rookAttack[pinnedIndex][rookMagicIndex(occ, pinnedIndex)] & (Board::piece[bRook] | Board::piece[bQueen]);
		while (potentialRookPinAttacker) {
			const int attackerIndex = getlsbBitIndex(potentialRookPinAttacker);
			const map uocc = occ & ~indexSquare[pinnedIndex];

			if (rookAttack[attackerIndex][rookMagicIndex(uocc, attackerIndex)] & Board::piece[wKing]) {
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
const uint64_t Board::IpinMaskHVBlack() const{
	const int index = getlsbBitIndex(Board::piece[bKing]);
	const uint64_t occ = Board::occupancy[white] | Board::occupancy[black];
	uint64_t res = 0x0ull;

	//potential bishop pin 
	map potentialRookPinnedPiece = rookAttack[index][rookMagicIndex(occ, index)] & (Board::occupancy[black]);

	//iterate through each potential pin and then create a pin mask 
	while (potentialRookPinnedPiece) {
		const int pinnedIndex = getlsbBitIndex(potentialRookPinnedPiece);

		//potential bits are just bishop and queen of the other color for the second 
		map potentialRookPinAttacker = rookAttack[pinnedIndex][rookMagicIndex(occ, pinnedIndex)] & (Board::piece[wRook] | Board::piece[wQueen]);
		while (potentialRookPinAttacker) {
			const int attackerIndex = getlsbBitIndex(potentialRookPinAttacker);
			const map uocc = occ & ~indexSquare[pinnedIndex];

			if (rookAttack[attackerIndex][rookMagicIndex(uocc, attackerIndex)] & Board::piece[bKing]) {
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
	*boardState.side = white; 
	*boardState.enpassant = 0x0ull; 
	*boardState.castleRightWQ = false;
	*boardState.castleRightWK = false;
	*boardState.castleRightBQ = false;
	*boardState.castleRightBK = false;

	*boardState.doubleCheck = false;


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
				board.piece[bPawn] |= indexSquare[positionIndex];
				break;
			case 'n':
				board.piece[bKnight] |= indexSquare[positionIndex];
				break;
			case 'b':
				board.piece[bBishop] |= indexSquare[positionIndex];
				break;
			case 'r':
				board.piece[bRook] |= indexSquare[positionIndex];
				break;
			case 'q':
				board.piece[bQueen] |= indexSquare[positionIndex];
				break;
			case 'k':
				board.piece[bKing] |= indexSquare[positionIndex];
				break;
			}

			//set occupancy 
			board.occupancy[black] |= indexSquare[positionIndex];

			//increment index 
			++positionIndex;
		}
			
		
		//white piece 
		else if ('A' <= fen[index] && fen[index] <= 'Z') {
			switch (fen[index]) {
			case 'P':
				board.piece[wPawn] |= indexSquare[positionIndex];
				break;
			case 'N':
				board.piece[wKnight] |= indexSquare[positionIndex];
				break;
			case 'B':
				board.piece[wBishop] |= indexSquare[positionIndex];
				break;
			case 'R':
				board.piece[wRook] |= indexSquare[positionIndex];
				break;
			case 'Q':
				board.piece[wQueen] |= indexSquare[positionIndex];
				break;
			case 'K':
				board.piece[wKing] |= indexSquare[positionIndex];
				break;
			}
			
			//set occupancy 
			board.occupancy[white] |= indexSquare[positionIndex];

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
	if (fen[index] == 'b') *boardState.side = black;
	else *boardState.side = white;
	index += 2; 

	//std::cout << "index: " << index << std::endl; 
	
	//castling rights 
	while (fen[index] != ' ') {
		switch (fen[index]) {
		case 'K':
			*boardState.castleRightWK = true;
			break;
		case 'Q':
			*boardState.castleRightWQ = true;
			break;
		case 'k':
			*boardState.castleRightBK = true;
			break;
		case 'q':
			*boardState.castleRightBQ = true;
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
		const int mapIndex = (fen[enpassantStart] - 'a') + 8 * ('8' - fen[uint64_t(enpassantStart) + 1]);
		*boardState.enpassant = indexSquare[mapIndex];
	}

}

void Fen::IparseStartPosition(Board& board, BoardState& boardState) {
	parse("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ", board, boardState);
}


void Board::makemove(move move, BoardState& boardState) {
	*boardState.side == white ? Board::Imakemovewhite(move, boardState) : Board::Imakemoveblack(move, boardState); 
}

void Board::Imakemovewhite(move move, BoardState& boardState) {

	//const int piece = Move::piece(move);
	//popBit(Board::piece[piece], Move::sourceSquare(move));
	//setBit(Board::piece[piece], Move::targetSquare(move));

	////promotions
	//if (Move::promotePiece(move) != wPawn) {
	//	popBit(Board::piece[piece], Move::targetSquare(move));
	//	setBit(Board::piece[Move::promotePiece(move)], Move::targetSquare(move));
	//}

	//
	//if (Move::captureFlag(move)) {
	//	//remove captured black piece 
	//}

	//

	//constexpr bool captureFlag(const move m);
	//constexpr bool doublePushFlag(const move m);
	//constexpr bool enpassantFlag(const move m);
	//constexpr bool castlingFlag(const move m);


	//iterate through the other side and remove bit from bitboard if it is a capture regardless? 



	//change boardState
}


void Board::Imakemoveblack(move move, BoardState& boardState) {

}
