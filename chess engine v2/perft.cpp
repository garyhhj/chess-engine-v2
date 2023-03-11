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
	if (depth == 0) {
		std::cout << "nodes: 1" << std::endl; 
	}
	int node = 0; 
	Movelist ml; 
	ml.moveGen(board, boardState); 

	//current board conditions 
	Board currBoard = board;
	BoardState currBoardState = boardState;
	for (int index = 0; index < ml.getIndex(); ++index) {
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
	std::flush(std::cout);
}