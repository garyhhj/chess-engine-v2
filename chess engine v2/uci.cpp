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
}

/*
Example UCI commands to init position on chess board
// init start position and make the moves on chess board
position startpos moves e2e4 e7e5

// init position from fen string and make moves on chess board
position fen r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 moves e2a6 e8g8
*/
void UCI::parsePosition(const std::string& position, Board& board, BoardState& boardState) {
	UCI::get().IparsePosition(position, board, boardState); 
}

void UCI::IparsePosition(const std::string& position, Board& board, BoardState& boardState) {
	std::stringstream ss(position); 
	std::string word; 
	while (ss >> word) {
		if (word == "startpos") {
			Fen::parseStartPosition(board, boardState); 
		}
		else if (word == "fen") {
			std::string fen = "";
			int count = 6; 
			while (count--) {
				ss >> word;
				fen += word;
				fen += " ";
			}
			Fen::parse(fen, board, boardState); 
		}
		else if (word == "moves") {
			while (ss >> word) {
				Movelist ml;
				ml.moveGen(board, boardState);
				const int index = UCI::parseMove(word, ml, boardState);
				if (index != -1) board.makemove(ml.getMove(index), boardState);
				else {
					std::cout << "illegal move: " << word << std::endl;
					return;
				}
			}

		}
	}
}

void UCI::parseGo(const std::string& command, Board& board, BoardState& boardState) {
	UCI::get().IparseGo(command, board, boardState); 
}

//eg go depth 1
void UCI::IparseGo(const std::string& command, Board& board, BoardState& boardState) {

	Evaluation::ply = 0; 
	Evaluation::nodes = 0; 
	
	std::fill(Evaluation::killerMoves[0], Evaluation::killerMoves[0] + 2 * 64, 0x0ul);
	std::fill(Evaluation::historyScore[0], Evaluation::historyScore[0] + 12 * 64, 0);

	std::fill(Evaluation::pvLength, Evaluation::pvLength + 64, 0);
	std::fill(Evaluation::pvTable[0], Evaluation::pvTable[0] + 64 * 64, 0x0ul);




	std::stringstream ss(command); 
	std::string word; 
	int depth;
	ss >> word; //go 
	ss >> word; //depth 
	ss >> depth; 
	
	//iterative deepening
	for (int currDepth = 1; currDepth <= depth; ++currDepth) {
		Evaluation::nodes = 0; 

		const int eval = Evaluation::negamax(board, boardState, -infinity, infinity, currDepth);
		std::cout << "info score cp " << eval << " depth " << currDepth << " nodes " << Evaluation::nodes << " pv ";

		for (int i = 0; i < Evaluation::pvLength[0]; ++i) {
			std::cout << Move::moveString(Evaluation::pvTable[0][i]) << " ";
		}
		std::cout << "\n";
	}
	

	if (Evaluation::pvTable[0][0] != 0x0ul) {
		std::cout << "bestmove " << Move::moveString(Evaluation::pvTable[0][0]) << "\n";
	}
	//else {
	//	std::cout << "could not find best move" << std::endl; 
	//	std::cout << "debugging information: " << std::endl;
	//	board.print(boardState); 
	//}
}

void UCI::uciRun(Board& board, BoardState& boardState) {
	UCI::get().IuciRun(board, boardState); 
}
void UCI::IuciRun(Board& board, BoardState& boardState) {
	std::cout.clear(); 

	//print engine info 
	std::cout << "id name meo\n";
	std::cout << "id name yarh\n";
	std::cout << "uciok\n";

	
	std::string line; 
	while (std::getline(std::cin, line)) {
		std::stringstream ss(line); 
		
		if (line.size() == 0) {
			continue; 
		}
		
		std::string word; 
		ss >> word; 
		if (word == "isready") {
			std::cout << "readyok\n"; 
		}
		else if (word == "position") {
			UCI::parsePosition(line, board, boardState); 
			board.print(boardState); 
		}
		else if (word == "ucinewgame") {
			UCI::parsePosition("position fen rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", board, boardState);
			Ttable::clear(); 
			board.print(boardState); 
		}
		else if (word == "go") {
			UCI::parseGo(line, board, boardState); 
		}
		else if (word == "uci") {
			//print engine info 
			std::cout << "id name meo\n";
			std::cout << "id name yarh\n";
			std::cout << "uciok\n";
		}
		else if (word == "quit") {
			break; 
		}
		std::cout << std::flush; 
	}


	if (!std::cin) {
		std::cout << "info: cin is corrupted" << std::endl; 
	}
}
