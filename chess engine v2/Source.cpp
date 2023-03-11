#include "macro.h"
#include "attackTable.h"
#include "board.h"
#include "movelist.h"
#include "perft.h"

#include <iostream>
#include <bitset>
#include <map>
#include <chrono>
//write some stuff for perft test 


int main() {

	Board board; 
	BoardState boardState; 

	{
		using namespace std;
		string fen1 = "8/8/8/8/8/8/8/8 w - - ";
		string fenStart = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ";
		string fenTricky = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ";
		string fenK = "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1";
		string fenCmk = "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 ";
		string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RN2K2R b KQkq - 0 1 ";
			
		//string fentemp = "rnbqkbnr/1ppppppp/8/p7/Q7/2P5/PP1PPPPP/RNBQKBNR b KQkq - 0 1 ";

		string fentemp = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";
		string fentemp2 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/1R2K2R b KQkq - ";

		Fen::parse(fentemp2, board, boardState);
	}
	

	//board.print(boardState); 

	perft(board, boardState, 2);

	//board.print(boardState); 

	//Movelist ml; 
	//ml.moveGen(board, boardState); 
	////ml.print(); 
	//board.makemove(ml.getMove(36), boardState); 
	//board.print(boardState); 

	
	//Movelist ml1; 
	//ml1.moveGen(board, boardState); 
	//board.makemove(ml1.getMove(13), boardState); 

		
	//Movelist ml2; 
	//ml2.moveGen(board, boardState); 
	//board.makemove(ml2.getMove(20), boardState); 


	//Movelist ml3; 
	//ml3.moveGen(board, boardState); 
	//board.makemove(ml3.getMove(19), boardState); 
	//board.print(boardState); 
	//
	//const map safesquare = board.safeSquares(boardState); 
	//
	//perft(board, boardState, 1); 

	//perft(board, boardState, 1); 

	//perft(board, boardState, 1); 

	//for king moves need to chedck attacked 


	//const map mask = board.checkMask(boardState); 
	//printBit(mask); 
	//const map pind = board.pinMaskDiagonal(boardState); 
	//printBit(pind); 

	//ml.moveGen(board, boardState); 




	//auto start = std::chrono::steady_clock::now();
	//Movelist ml; 
	//ml.moveGen(board, boardState); 
	//board.makemove(ml.getMove(5), boardState);
	////board.print(boardState); 
	//
	//Movelist ml2; 
	//ml2.moveGen(board, boardState); 
	////ml2.print(); 
	//board.makemove(ml2.getMove(15), boardState); 
	////board.print(boardState); 

	//Movelist ml3; 
	//ml3.moveGen(board, boardState); 
	////ml3.print(); 
	//board.makemove(ml3.getMove(20), boardState); 
	////board.print(boardState); 

	//perft(board, boardState, 1); 
	//perft(board, boardState, 2); 
	//uh oh... this make move has some issues 

	//std::cout << "wpawn, knight, king:" << std::endl;
	//printBit(board.piece[wPawn]); 
	//printBit(board.piece[wKnight]); 
	//printBit(board.piece[wKing]); 

	//std::cout << "wbishop, wrook, wqueen" << std::endl; 
	//printBit(board.piece[wBishop]); 
	//printBit(board.piece[wRook]); 
	//printBit(board.piece[wQueen]); 

	//std::cout << "bpawn, knight, king:" << std::endl; 
	//printBit(board.piece[bPawn]); 
	//printBit(board.piece[bKnight]); 
	//printBit(board.piece[bKing]); 

	//std::cout << "bbishop, brook, bqueen" << std::endl; 
	//printBit(board.piece[bBishop]); 
	//printBit(board.piece[bRook]); 
	//printBit(board.piece[bQueen]); 

	//std::cout << "occupancy white black" << std::endl; 
	//printBit(board.occupancy[white]); 
	//printBit(board.occupancy[black]); 

	//std::cout << "boardstate" << std::endl; 
	//boardState.debug(); 

	//perft(board, boardState, 1);


	//auto end = std::chrono::steady_clock::now();
	//std::chrono::duration<double> elapsed_seconds = end - start;
	//std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";









	
	//after restoring board state and then using make move results in an error... so likely boardstate / board is not restored properly 

	//print to test debug 

	/*map occ = Board::Get().occupancy[white] | Board::Get().occupancy[black]; 

	std::cout << "occ" << std::endl; 
	printBit(occ); 

	int index = getlsbBitIndex(E1); 


	int magicIndex = ((occ & relevantRookBlocker[index]) * rookMagicNum[index]) >> (64 - getNumBit(relevantRookBlocker[index])); 

	printBit(rookAttack[index][magicIndex]); */
	


	////set up board with parse fen
	//map mask = Board::Get().checkMask(); 
	//
	//printBit(mask);
	/*std::string s; 

	for (int i = 0; i < 64; ++i) {
		std::cout << i << std::endl; 
		printBit(relevantRookBlocker[i]); 

		std::cin >> s; 
	}*/


	


	/*  8   r n b q k b n r
		7   p p p p p p p p
		6   0 0 0 0 0 0 0 0
		5   0 0 0 0 0 0 0 0
		4   0 0 0 0 0 0 0 0
		3   0 0 0 0 0 0 0 0
		2   P P n p P P P P
		1   r 0 0 0 K B N R

		    a b c d e f g h*/
	//todo: 
	//perft test 

	//test out rule of 3.5 for board and boardState  




	//debruijin optimization for lsb bit index  
	//make move function (make sure to change board state/enpassant flag/castling flags) 


	
	

}