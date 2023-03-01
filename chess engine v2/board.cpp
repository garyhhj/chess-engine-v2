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


void Fen::clear() {
	Get().Iclear(); 
}

void Fen::parse(const std::string& fen) {
	Get().clear(); 
	Get().Iparse(fen); 
}

void Fen::parseStartPosition() {
	Get().IparseStartPosition(); 
}


void Fen::Iclear() {
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

void Fen::Iparse(const std::string& fen) {
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

void Fen::IparseStartPosition() {
	parse("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ");
}


