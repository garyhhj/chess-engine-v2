#include "board.h"


/********************
*
*BoardState
*singleton
*
*********************/

BoardState& BoardState::Get() {
	static BoardState instance;
	return instance;
}

void BoardState::debug() {
	Get().Idebug();
}


void BoardState::Idebug() {
	std::cout << "side: " << side << "\n" <<
		"enpassant: " << enpassant << "\n";
	std::flush(std::cout);
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


constexpr void Fen::clear() {
	Get().Iclear(); 
}

constexpr void Fen::parse(const std::string& fen) {
	Get().clear(); 
	Get().Iparse(fen); 
}

constexpr void Fen::parseStartPosition() {
	Get().IparseStartPosition(); 
}


constexpr void Fen::Iclear() {
	//BoardState
	BoardState::Get().side = white; 
	BoardState::Get().enpassant = 0x0ull; 
	BoardState::Get().castleRightWQ = false;
	BoardState::Get().castleRightWK = false; 
	BoardState::Get().castleRightBQ = false; 
	BoardState::Get().castleRightBK = false; 

	//Board
	for (int i = 0; i < 12; ++i) {
		Board::Get().piece[i] = 0x0ull; 
	}

	for (int i = 0; i < 2; ++i) {
		Board::Get().occupancy[i] = 0x0ull; 
	}
}

constexpr void Fen::Iparse(const std::string& fen) {
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
			
			//increment index 
			++positionIndex;
			
			//then set occupancy 
			Board::Get().occupancy[black] |= indexSquare[positionIndex];
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
			
			//increment index 
			++positionIndex;
			
			//then set occupancy 
			Board::Get().occupancy[white] |= indexSquare[positionIndex];
		}

		//number 
		else if ('0' <= fen[index] && fen[index] <= '9'){ 
			positionIndex += fen[index] - '0';
		}

		++index;

		//increment index 
	}

	//side t0 move 
	if (fen[index] == 'b') BoardState::Get().side = black;
	else BoardState::Get().side = white;
	index += 2; 

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
		int rank = fen[enpassantStart + 1] - '0';
		int file = fen[enpassantStart] - 'a';

		BoardState::Get().enpassant = 8 * (8 - rank) + file;
	}

}
constexpr void Fen::IparseStartPosition() {
	parse("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ");
}


