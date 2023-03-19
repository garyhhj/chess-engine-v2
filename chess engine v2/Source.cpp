#include "macro.h"
#include "attackTable.h"
#include "board.h"
#include "movelist.h"
#include "perft.h"
#include "uci.h"
#include "evaluation.h"
#include "zobrist.h"
#include "randomNum.h"

#include <iostream>
#include <bitset>
#include <map>
//write some stuff for perft test 


int main() {
	
	Board board;
	BoardState boardState;
	Zobrist::initZobristTables();

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
		string fenEndGame = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";



		//string fentest1 = "KB6/8/8/8/4pP2/8/7k/8 b - f3 0 1 ";

		string fentest1 = "3rk2r/p1p1qpb1/bn2Pnp1/3pN3/1p2P3/2N2Q1p/PPPBBPPP/R2K3R w --k- d6 ";
		string fentest2 = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - "; 
		string fentest3 = "8/2p5/3p4/KP5r/1R2Pp1k/8/6P1/8 b - e3 ";
		string fentest4 = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ";

		string fentest5 = "8/8/8/KP5r/1R1pP2k/8/6P1/8 b - e3 ";

		string fentest6 = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ";
		string fentest7 = "8/8/3p4/KPp4r/1R2Pp1k/8/6P1/8 w - c6 ";
		string fentest8 = "8/2p5/3p4/KP5r/1R3p1k/4P3/6P1/8 b - - ";
		string fentest9 = "8/8/2pp4/KP5r/1R3p1k/4P3/6P1/8 w - - ";
		string fentest10 = "8/8/2pp4/KP5r/1R3pPk/4P3/8/8 b - g3 ";
		string fentest11 = "8/8/2pp4/KP5r/1R2rpPk/4P3/8/8 b - g3 ";

		string fentest12 = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - "; 
		string fentest13 = "8/2p5/3p4/KP5r/1R3p1k/4P3/6P1/8 b - - ";  //very important position, just start from here tbh 
		string fentest14 = "8/8/2pp4/KP5r/1R3p1k/4P3/6P1/8 w - - ";  //very important position, just start from here tbh 
		string fentest15 = "8/8/2pp4/KP5r/1R3p1k/4P1P1/8/8 b - - ";  //very important position, just start from here tbh 

		string fentest16 = "8/2p5/3p4/KP4kr/1R3p2/4P3/6P1/8 w - - ";  //very important position, just start from here tbh 
		string fentest17 = "8/2p5/3p4/1P4kr/KR3p2/4P3/6P1/8 b - - ";  //very important position, just start from here tbh 
		string fentest18 = "8/2p5/3p4/1P3k1r/KR3p2/4P3/6P1/8 w - - ";  //very important position, just start from here tbh 
		string fentest19 = "8/2p5/3p4/1P3k1r/KR3pP1/4P3/8/8 b - g3 ";  //very important position, just start from here tbh 

		string fentest20 = "8/2p5/3p3k/1P32r/KR3pP1/4P3/8/8 b - g3 ";  //very important position, just start from here tbh 

		//string fentest19 = "8/2p5/3p4/1P3k1r/KR3pP1/4P3/8/8 b - g3 ";  //very important position, just start from here tbh 
		string fentest21 = "8/2p5/3p4/1P4kr/KR3pP1/4P3/8/8 w - - ";  //very important position, just start from here tbh 
		string fentest22 = "8/2p5/3p4/1P4kr/K4pP1/4P3/8/1R6 b - - ";  //very important position, just start from here tbh 
		string fentest23 = "8/2p5/3p1k2/1P5r/K4pP1/4P3/8/1R6 w - - ";  //very important position, just start from here tbh 
		string fentest24 = "8/2p5/3p1k2/1P5r/K4pP1/4P3/8/5R2 b - - ";  //very important position, just start from here tbh 

		string stuff = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ";
		string stuffmirror
			= "8/2P5/3P4/kp5R/1r3P1K/8/4p1p1/8 b - - ";
		string kiwipete = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -";
		string kiwipete1 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPP1BPPP/R1B1K2R b KQkq -";
		string kiwipete2 = "r3k2r/p1pp1pb1/bn1qpnp1/3PN3/1p2P3/2N2Q1p/PPP1BPPP/R1B1K2R w KQkq -"; //try e1d2 
		string kiwipete3 = "r3k2r/p1pp1pb1/bn1qpnp1/3PN3/1p2P3/2N2Q1p/PPP1BPPP/R1BK12R b --kq -"; 
		string kiwipete4 = "r3k2r/p2p1pb1/bn1qpnp1/2pPN3/1p2P3/2N2Q1p/PPP1BPPP/R1BK12R w --kq c6 ";

		string kiwipeteclone = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -";
		string kiwipeteclone1 = "r3k2r/p1ppqpb1/bn2Pnp1/4N3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq -";
		string kiwipeteclone2 = "r4k1r/p1ppqpb1/bn2Pnp1/4N3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQ-- -"; //lots of bugs revisit after first one 
		string kiwipeteclone3 = "r4k1r/p1ppqPb1/bn3np1/4N3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQ-- -"; //try f6 g8 
		string kiwipeteclone4 = "r3nk1r/p1ppqPb1/bn4p1/4N3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQ-- -"; 
		string kiwipeteclone5 = "r3Rk1r/p1ppq1b1/bn4p1/4N3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQ-- -";

		string endgame = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - "; 
		string pos4 = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
		string pos4_1 = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P1RPP/R2Q2K1 b kq - 0 1"; //b2a1 
		string pos4_2 = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/P2P1RPP/q2Q2K1 w kq - 0 1"; 

		string pos5 = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  "; 
		string pos6 = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 "; 

		string posInitial = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	
		string fenKcpy = "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNB1KBNR w KQk- - 0 1";

		
		Fen::parse(posInitial, board, boardState);

	}
	board.print(boardState);

	Board currBoard = board; 
	BoardState currBoardState = boardState; 
	const map currhash = Zobrist::hashZobrist(board, boardState); 

	Movelist ml; 
	ml.moveGen(board, boardState); 

	for (int i = 0; i < ml.getIndex(); ++i) {
		Move::decode(ml.getMove(i)); std::cout << std::endl; 
		
		board.makemove(ml.getMove(i), boardState); 

		board = currBoard; 
		boardState = currBoardState; 
		board.getHashkey() = currhash; 
	}
	std::cout << "done" << std::endl; 
	
	//test zobirst hashing 


	
	//create function that hashes keys 

	//map Zobrist::pieceKey[12][64]{}; //pieceKey[piece][square]
	//map Zobrist::enpassant[64]{}; //enapssant[square]
	//map Zobrist::castleKey[16]{}; //castleKey[castleflag(binary representation)]
	//map Zobrist::sideKey{};


	//std::string s; 
	//for (int i = 0; i < 12; ++i) {
	//	std::cout << "I: " << i << std::endl; 
	//	printBit(Random::getRandomUint64()); 
	//	std::cin >> s; 
	//}

	//I need to initialize the arrays for zobrist 
	//I need to create a function that hashes zobrist 
	//I also need to create lazy evaluation/update for zobrist hash using xor 

	
	//UCI::uciRun(board, boardState); 

	//implement time handeling + 

	//change how to handel detecting check mate to if statement at end of search 

	//check quiesence 
	
	//something is wrong with evaluation... idk something very sus 
	// 
	// 
		//Movelist ml;
		//ml.moveGen(board, boardState);
		//board.makemove(ml.getMove(32), boardState);

		//Move::decode(ml.getMove(32)); std::cout << std::endl;

		//board.print(boardState); 

	/*std::cout << "pawn" << std::endl; 
	printBit(board.getPiece()[wPawn]); 
	printBit(board.getPiece()[bPawn]); 

	std::cout << "knight" << std::endl;
	printBit(board.getPiece()[wKnight]);
	printBit(board.getPiece()[bKnight]);

	std::cout << "king" << std::endl;
	printBit(board.getPiece()[wKing]);
	printBit(board.getPiece()[bKing]);

	std::cout << "bishop" << std::endl;
	printBit(board.getPiece()[wBishop]);
	printBit(board.getPiece()[bBishop]);

	std::cout << "rook" << std::endl;
	printBit(board.getPiece()[wRook]);
	printBit(board.getPiece()[bRook]);
	
	std::cout << "queen" << std::endl;
	printBit(board.getPiece()[wQueen]);
	printBit(board.getPiece()[bQueen]);

	


	//board.print(boardState); 

	/*
	Movelist ml; 
	ml.moveGen(board, boardState); 
	board.makemove(ml.getMove(5), boardState); 

	board.print(boardState);*/

	//perft(board, boardState, 4); 

	//wrong at depht of 5 

	//Movelist ml; 
	//ml.moveGen(board, boardState); 
	//board.makemove(ml.getMove(7), boardState); 
	//
	//board.print(boardState); 

	//auto mask = board.checkMask(boardState); 

	

	//std::cout << "checkmask" << std::endl; 
	//printBit(mask); 

	//if (boardState.doubleCheck) {
	//	std::cout << "double check yay" << std::endl;
	//}
	//else std::cout << "noo something is wrong" << std::endl; 

	//perft(board, boardState, 1); 


	//perft(board, boardState, 1); 
	//perft(board, boardState, 2);


	//perft(board, boardState, 4);


//	perft(board, boardState, 2); 

	//fix errors from kiwipete first 

	//position 2 not correct at depth of 8 - correct up to depth of 7 

	//replace rooks with bishop and test diagonal pins? light square and dark square on both sides 

	//perft(board, boardState, 2);

	//Movelist ml;
	//ml.moveGen(board, boardState);
	//board.makemove(ml.getMove(11), boardState);

	//board.print(boardState); 

	//std::cout << "mask hv : " << std::endl; 
	//printBit(board.pinMaskHV(boardState)); 

	//perft(board, boardState, 1); 

/*


	board.print(boardState);

	*///perft(board, boardState, 4); 

	//board.print(boardState);

	//auto mask = board.pinMaskHV(boardState); 
	//printBit(mask); 
	//perft(board, boardState, 1); 

	//Movelist ml; 
	//ml.moveGen(board, boardState); 
	//board.makemove(ml.getMove(2), boardState); 
	//Move::decode(ml.getMove(2)); std::cout << std::endl; 
	//board.print(boardState);


	//auto mask = board.pinMaskHV(boardState); 
	//std::cout << "hv mask: " << std::endl; 
	//printBit(mask); 

	//perft(board, boardState, 1); 
	//perft(board, boardState, 2); 

	//Movelist ml; 
	//ml.moveGen(board, boardState); 
	////ml.print(); 
	//board.makemove(ml.getMove(3), boardState); 
	//board.print(boardState); 
	//
	//board.print(boardState); 

	//perft(board, boardState, 3); 

	//auto maskh = board.pinMaskHV(boardState); 
	//
	//std::cout << "horizontal mask" << std::endl; 
	//printBit(maskh); 
/*
	Movelist ml; 
	ml.moveGen(board, boardState); 
	board.makemove(ml.getMove(3), boardState); 

	board.print(boardState); 

	*/
	//perft(board, boardState, 1); 

	//kitty key : e2e4: 15
	//	monkey key : e2e4: 16
	//	different nodes
	//	kitty key : g2g4: 16
	//	monkey key : g2g4: 17

	//map pinmaskhv = board.pinMaskHV(boardState); 
	//map pinmaskd = board.pinMaskDiagonal(boardState); 
	//printBit(pinmaskhv); 
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