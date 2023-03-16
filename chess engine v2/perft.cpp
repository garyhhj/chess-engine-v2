#include "perft.h"

int perftHelper(Board& board, BoardState& boardState, int depth) {
	if (depth == 0) return 1;
	
	int nodes = 0; 

	Movelist ml; 
	ml.moveGen(board, boardState); 

	Board currBoard = board; 
	BoardState currBoardState = boardState; 

	for (int i = 0; i < ml.getIndex(); ++i) {
		board = currBoard; 
		boardState = currBoardState; 
		board.makemove(ml.getMove(i), boardState);
		nodes += perftHelper(board, boardState, depth - 1); 
	}

	return nodes; 

}

void perft(Board& board, BoardState& boardState, int depth) {
	std::cout << "perft" << std::endl; 
	auto start = std::chrono::steady_clock::now();
	if (depth == 0) {
		std::cout << "nodes: 1" << std::endl; 
	}
	uint64_t node = 0; 
	Movelist ml; 
	ml.moveGen(board, boardState); 

	//current board conditions 
	Board currBoard = board;
	BoardState currBoardState = boardState;
	const int totalNodes = ml.getIndex();
	for (int index = 0; index < totalNodes; ++index) {
		board = currBoard; 
		boardState = currBoardState; 
		board.makemove(ml.getMove(index), boardState); 
		
		int childNodes = perftHelper(board, boardState, depth - 1); 
		//padding/move index 
		std::cout << "move: " << index;
		if (index < 10) std::cout << " ";
		std::cout << " ";


		Move::decode(ml.getMove(index)); 


		std::cout << " | nodes: " << childNodes << "\n";
		node += childNodes;
	}
	std::cout << "nodes: " << node << "\n";


	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
	std::flush(std::cout);
}