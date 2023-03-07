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
			
		string fentemp = "rnbqkbnr/pppppppp/8/8/8/8/6P1/RNBQKBNR w KQkq - 0 1 ";

		//for (int i = 0; i < fentemp.size(); ++i) {

		//	std::cout << i << " " << fentemp[i] << '\n'; 
		//}
		//std::flush(std::cout); 


		Fen::parse(fentemp);
	}

	//std::string s; 
	//for (int i = 0; i < 64; ++i) {
	//	std::cout << "i: " << i << std::endl; 
	//	std::cout << "lsbbitindex: " << getlsbBitIndex(indexSquare[i]) << std::endl; 
	//	printBit(indexSquare[i]); 
	//	std::cin >> s; 
	//}

	Board::print(); 

	Movelist ml;
	ml.moveGen(Board::Get(), BoardState::Get()); 
	
	ml.print(); 

	//map mask = Board::Get().checkMask();
	//printBit(mask); 
	//BoardState::debug(); 

	/*map mask = Board::Get().checkMask(); 
	printBit(mask); 
	BoardState::debug(); */

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
	//generate moves 

	//generate legal moves 
	//debruijin optimization for lsb bit index  

	//testing leaper piece move generation 

	//need to test each piece /part inidividually 

}