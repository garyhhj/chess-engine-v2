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

		string fenkiwi = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"; 

		//string fentemp = "rnbqkbnr/1ppppppp/8/p7/Q7/2P5/PP1PPPPP/RNBQKBNR b KQkq - 0 1 ";

		string fentemp = "1nbqkbnr/Pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 1 2";
		string fentemp2 = "r3k2r/p1ppqN2/1n2pnpb/1b1P4/1p2P3/2N2Q1p/PPPBBPPP/2KR3R w --kq - ";
		string fentemp3 = "3k4/8/4K3/1Q6/8/8/8/8 w - - 0 1 "; 
		string fentemp4 = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1 "; 
		string fentemp5 = "r1bq2nr/ppppbk2/2n2p2/3Pp1pp/4P1P1/5N2/PPP2PBP/RNBQ2KR w - - 3 9 "; 
		string fentemp6 = "3K4/8/4k3/1q6/8/8/8/8 b - - 0 1 ";
		string fentemp7 = "3Kq3/8/4k3/8/8/8/8/8 w - - 0 1 ";

		string fentemp8 = "3kQ3/8/4K3/8/8/8/8/8 b - - 0 1 ";

		//string fenEndGame = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ";
		//string fenEndGame = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";
		//string fenEndGame = "r3k2r/p1ppqpb1/bn2Pnp1/4N3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - ";
		//string fenEndGame = "3rk2r/p1ppqpb1/bn2Pnp1/4N3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQk- - ";
		//string fenEndGame = "3rk2r/p1ppqpb1/bn2Pnp1/4N3/1p2P3/2N2Q1p/PPPBBPPP/R2K3R b --k- - ";
		//string fenEndGame = "3rk2r/p1p1qpb1/bn2Pnp1/3pN3/1p2P3/2N2Q1p/PPPBBPPP/R2K3R w --k- d6 ";
		string fenEndGame = "3rk3/8/8/r1Pp3K/8/8/8/8 w --k- d6 ";



		//string fentest1 = "KB6/8/8/8/4pP2/8/7k/8 b - f3 0 1 ";

		string fentest1 = "Kb6/8/8/8/4pp2/8/7k/8 b - - 0 1 ";


		//make sure to parse fen start before running uci 
		Fen::parse(fenEndGame, board, boardState);
	}

	board.print(boardState);

	map pinmaskhv = board.pinMaskHV(boardState); 
	//map pinmaskd = board.pinMaskDiagonal(boardState); 
	printBit(pinmaskhv); 
	//printBit(pinmaskd); 
	

	/*Movelist ml; 
	ml.moveGen(board, boardState); 
	board.makemove(ml.getMove(4), boardState);

	board.print(boardState);*/

	/*map mask = board.pinMaskDiagonal(boardState); 
	printBit(mask); */

	//perft(board, boardState, 1);


	/*
	Movelist ml; 
	ml.moveGen(board, boardState); 
	ml.print(); 

	board.makemove(ml.getMove(2), boardState); 
	board.print(boardState); 

	*/
	
	//perft(board, boardState, 1);


	//board.print(boardState); 

	//UCI::uciRun(board, boardState); 
	/*Movelist ml; 
	ml.moveGen(board, boardState);
	ml.print(); 
	std::cout << "eval: " << Evaluation::evaluate(board, boardState, ml, 1); */

	//Evaluation::negamax(board, boardState, -50000, 50000, 3); 
	//std::cout << "bestmove: " << Evaluation::bestmove << std::endl; 
	//board.print(boardState); 
	//std::cout << "searched " << Evaluation::nodes << " nodes " << std::endl;



	//Movelist ml; 
	//ml.moveGen(board, boardState); 
	//
	//Evaluation::sortMove(board, ml); 

	//for (int i = 0; i < ml.getIndex(); ++i) {
	//	Move::decode(ml.getMove(i)); 
	//	std::cout << " movescore: " << Evaluation::moveScore(board, ml.getMove(i)); 
	//	std::cout << std::endl; 
	//}

	//UCI::uciRun(board, boardState); 
	
	/*Movelist ml; 
	ml.moveGen(board, boardState); 
	ml.print(); 
	auto s = Move::moveString(ml.getMove(2)); 

	std::cout << s << std::endl; */
	
	//Move::moveString(ml.getMove(2)); 

	//test the move string generation 

	/*board.print(boardState); 

	auto s = Evaluation::minMax(board, boardState, 3); 
	std::cout << "bestmove: " << s << std::endl; */

	//UCI::uciRun(board, boardState); 

	//if (word == "isready") {
	//	std::cout << "readyok\n";
	//}
	//else if (word == "position") {
	//	UCI::parsePosition(line, board, boardState);
	//}
	//else if (word == "ucinewgame") {
	//	UCI::parsePosition("position fen rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", board, boardState);
	//}
	//else if (word == "go") {
	//	UCI::parseGo(line, board, boardState);
	//}
	//else if (word == "uci") {
	//	//print engine info 
	//	std::cout << "id name meo\n";
	//	std::cout << "id name yarh\n";
	//	std::cout << "uciok\n";
	//}
	//else if (word == "quit") {
	//	break;
	//}


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
	 

	//printing out the move string give a move and index 
	//another min max function but just for base call that will print out the correct move string 

	//read up on history killer 
	//pv
	//static exchange evaluation 
	//transposition table 


	//pv/killer/history moves 
	//iterative deepening 


	//hashing zorborist 


}