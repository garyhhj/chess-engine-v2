#include "perft.h"

int perftHelper(Board& board, BoardState& boardState, int depth) {
	int nodes = 1; 
	if (depth == 0) return nodes; 

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
	int node = 0; 
	Movelist ml; 
	ml.moveGen(board, boardState); 

	//current board conditions 
	Board currBoard = board;
	BoardState currBoardState = boardState;
	std::cout << "for loop" << std::endl; 
	for (int index = 0; index < ml.getIndex(); ++index) {
		board = currBoard; 
		boardState = currBoardState; 
		board.makemove(ml.getMove(index), boardState); 
		
		int childNodes = perftHelper(board, boardState, depth - 1); 
		std::cout << "index: " << index << " | nodes: " << childNodes << "\n";
		node += childNodes;
	}
	std::cout << "nodes: " << node << "\n";
	std::flush(std::cout);
}