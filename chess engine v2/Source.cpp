#include "macro.h"
#include "attackTable.h"
#include "board.h"
#include "movelist.h"

#include <iostream>
#include <bitset>
#include <map>




int main() {

	{
		using namespace std;
		string fen1 = "8/8/8/8/8/8/8/8 w - - ";
		string fenStart = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ";
		string fenTricky = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ";
		string fenK = "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1";
		string fenCmk = "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 ";
		string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RN2K2R b KQkq - 0 1 ";
			
		string fentemp= "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ";

		Fen::parse(fentemp);
	}

	Board::print(); 

	//set up board with parse fen 

	if (Board::Get().attacked(E1)) {
		std::cout << "meoww" << std::endl; 
	}
	else {
		std::cout << "woof" << std::endl; 
	}

	//map mask = Board::Get().checkMask(); 
	//printBit(mask); 

	//todo: 
	//generate moves 

	//generate legal moves 
	//debruijin optimization for lsb bit index  

	//allow uint64_t be passed for encoding 

	//check if position at board is attacked 

	//easiest way is to check for attack by leaper piece by doing the reverse 
	//then check slider pieces 

	//test attacked 
	//test checkmask and write a checkmask for black 
}