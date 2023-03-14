#include "macro.h"
#include "attackTable.h"
#include "board.h"
#include "movelist.h"
#include "perft.h"
#include "uci.h"
#include "evaluation.h"

#include <iostream>
#include <bitset>
#include <map>
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

		string fentemp = "1nbqkbnr/Pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 1 2";
		string fentemp2 = "r3k2r/p1ppqN2/1n2pnpb/1b1P4/1p2P3/2N2Q1p/PPPBBPPP/2KR3R w --kq - ";

		Fen::parse(fenStart, board, boardState);
	}

	Movelist ml; 
	ml.moveGen(board, boardState); 
	ml.print(); 
	auto s = Move::moveString(ml.getMove(2)); 

	std::cout << s << std::endl; 
	//Move::moveString(ml.getMove(2)); 

	//test the move string generation 

	//UCI::uciRun(board, boardState); 

	

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
	//debruijin optimization for lsb bit index  

	//connecting the engine to the gui 
	 
	//move ordering 
	//positional evaluation 

	//printing out the move string give a move and index 
	//another min max function but just for base call that will print out the correct move string 
	//make min and max into alpha and beta 
	//sorting moves / move ordering 

	//read up on history killer 
	//pv
	//mvvla 
	//how to minimize horizon effect - is it even necessary if engine can search at higher depth? 

	//hashing zorborist 
	

}