#include "evaluation.h"

/********************
*
*Utils
*
*********************/

int Evaluation::ply = 0;
int Evaluation::nodes = 0;

move Evaluation::killerMoves[2][Evaluation::MAXPLY]{}; //[priority][ply]
int Evaluation::historyScore[12][Evaluation::MAXPLY]{}; //[piece][ply]

int Evaluation::pvLength[Evaluation::MAXPLY]{}; //[ply]
move Evaluation::pvTable[Evaluation::MAXPLY][Evaluation::MAXPLY]{}; //[ply][ply] 

/********************
*
*Repetition Table
*
*********************/

Rtable& Rtable::get() {
	static Rtable instance; 
	return instance; 
}

void Rtable::increment(const map hash) {
	Rtable::get().Iincrement(hash); 
}
void Rtable::Iincrement(const map hash) {
	Rtable::rTable[index] = hash; 
	++Rtable::index; 
}

void Rtable::decrement() {
	Rtable::get().Idecrement(); 
}
void Rtable::Idecrement() {
	--Rtable::index; 
}

bool Rtable::repetition(const map hash) {
	return Rtable::get().Irepetition(hash); 
}
bool Rtable::Irepetition(const map hash) {
	for (int i = 0; i < Rtable::index; ++i) {
		if (Rtable::rTable[i] == hash) return true; 
	}
	return false; 
}

void Rtable::clear() {
	Rtable::get().Iclear(); 
}
void Rtable::Iclear() {
	Rtable::index = 0; 
}

void Rtable::debug() {
	Rtable::get().Idebug(); 
}
void Rtable::Idebug() {
	std::ios_base::fmtflags f(std::cout.flags());
	std::cout << "Rtable: ";
	for (int i = 0; i < Rtable::index; ++i) {
		std::cout << "0x" << std::hex << Rtable::rTable[i] << ", "; 
	}
	std::cout << std::endl; 
	std::cout.flags(f);
}


/********************
*
*Transposition Table
*
*********************/

Ttable& Ttable::get() {
	static Ttable instance; 
	return instance; 
}

int Ttable::lookUp(const map hash, const int depth, const int alpha, const int beta) {
	return Ttable::get().IlookUp(hash, depth, alpha, beta); 
}

int Ttable::IlookUp(const map hash, const int depth, const int alpha, const int beta) {
	//do something meow 
	const int index = hash % tTableSize;
	
	transposition& tTableElement = Ttable::tTable[hash % Ttable::tTableSize]; 

	if (tTableElement.exactHash == hash && tTableElement.depth >= depth) {

		

		if (tTableElement.flag == Ttable::tFlagExact) {
			int score = tTableElement.eval; 
			if (score < -mateScoreLowerBound) score += Evaluation::ply;
			else if (score > mateScoreLowerBound) score -= Evaluation::ply; 

			return score;
		}
		if (tTableElement.flag == Ttable::tFlagAlpha && tTableElement.eval <= alpha) return alpha; 

		if (tTableElement.flag == Ttable::tFlagBeta && tTableElement.eval >= beta) return beta; 
	}	

	return Ttable::unknownEval; 
}

void Ttable::insert(const map hash, const int depth, const int flag, int eval) {
	Ttable::get().Iinsert(hash, depth, flag, eval); 
}

void Ttable::Iinsert(const map hash, const int depth, const int flag, int eval) {
	transposition& tTableElement = Ttable::tTable[hash % tTableSize]; 
		
	 
	if (eval < -mateScoreLowerBound) eval -= Evaluation::ply;
	if (eval > mateScoreLowerBound) eval += Evaluation::ply;

	tTableElement.exactHash = hash; 
	tTableElement.depth = depth; 
	tTableElement.flag = flag; 
	tTableElement.eval = eval; 
}

void Ttable::clear() {
	Ttable::get().Iclear(); 
}

void Ttable::Iclear() {	
	for (int index = 0; index < Ttable::tTableSize; ++index) {
		transposition& tTableElement = Ttable::tTable[index]; 

		tTableElement.depth = 0; 
		tTableElement.eval = Ttable::unknownEval; 
		tTableElement.exactHash = 0x0ull; 
		tTableElement.flag = Ttable::tFlagUnknown;
	}
}



static const std::string tflagString[] = {"exact", "alpha", "beta"};
void Ttable::printTtable(const int index) {
	std::ios_base::fmtflags f(std::cout.flags());
	std::cout << "depth/eval: " << Ttable::tTable[index].depth << " / " <<Ttable::tTable[index].eval << "\n";
	std::cout << "flag/hash: " << tflagString[Ttable::tTable[index].flag] << " / " << std::hex << "0x" << Ttable::tTable[index].exactHash << "\n";
	std::cout.flags(f);
}

void Ttable::debug() {
	Ttable::get().Idebug(); 
}
void Ttable::Idebug() {
	for (int i = 0; i < 10; ++i) {
		std::cout << "index: " << i << "\n"; 
		Ttable::printTtable(i); 
	}
	std::cout << std::endl; 
}

Ttable::Ttable() : tTable(new transposition[tTableSize]) {}
Ttable::~Ttable() {
	delete[] Ttable::tTable; 
}



/********************
*
*Evaluation
*
*********************/

/********************
*negamax + alphabeta
*********************/

bool Evaluation::canNullMove(const Board& board) {
	//diable null move pruning when pawns are only material on board 
	return
		((board.getPiece()[wPawn] | board.getPiece()[wKing]) != board.getOccupancy()[white]) ||
		((board.getPiece()[bPawn] | board.getPiece()[bKing]) != board.getOccupancy()[black]);
}

bool Evaluation::canLMR(move m) {
	//if there are captures and promotions or is a principle variation move return false 
	return !(Move::captureFlag(m)) && (Move::promotePiece(m) == wPawn) && (m != Evaluation::pvTable[0][ply]);
}


int Evaluation::quiesenceSearch(Board& board, BoardState& boardState, int alpha, int beta) {
	++Evaluation::nodes;
	const map currboardhash = board.getHashkey();
	
	if (Rtable::repetition(currboardhash)) {
		return 0; //stalemate score 
	}

	const int eval = Evaluation::evaluate(board, boardState, ply); 
	if (eval >= beta) return beta;
	if (eval > alpha) alpha = eval;

	Movelist ml;
	ml.moveGen(board, boardState);
	Evaluation::sortMove(board, ml);
	const int mlIndex = ml.getIndex();

	Board currBoard = board;
	BoardState currBoardState = boardState;

	for (int i = 0; i < mlIndex; ++i) {
		if (!Move::captureFlag(ml.getMove(i))) continue; 

		board.makemove(ml.getMove(i), boardState);
		++Evaluation::ply;
		Rtable::increment(currboardhash); 
		const int eval = -Evaluation::quiesenceSearch(board, boardState, -beta, -alpha);
		Rtable::decrement(); 
		board.updateHash(currboardhash); 
		--Evaluation::ply;


		board = currBoard;
		boardState = currBoardState;

		//fail hard beta cut off (oppnent has refutation so this branch will never be played assuming optimal play) 
		if (eval >= beta) {
			return beta;
		}

		//found better move 
		if (eval > alpha) {
			alpha = eval;
		}
	}

	return alpha;
}


int Evaluation::negamax(Board& board, BoardState& boardState, int alpha, int beta, int depth) {
	++Evaluation::nodes; 
	const map currboardhash = board.getHashkey(); 

	if (Rtable::repetition(currboardhash)) {
		return 0; //stalemate score; 
	}

	//look up in transposition table 
	{
		const bool pvNode = beta - alpha > 1; 
		const int score = Ttable::lookUp(currboardhash, depth, alpha, beta); 
		if (Evaluation::ply && score != Ttable::unknownEval && !pvNode) return score; 
	}
	
	Evaluation::pvLength[ply] = ply;


	//base case 
	if (depth == 0) {
		return Evaluation::quiesenceSearch(board, boardState, alpha, beta);
	}

	bool inCheck = board.attacked(board.getPiece()[(boardState.getSide() == white ? wKing : bKing)], boardState);
	int movesSearched = 0;

	Board currBoard = board;
	BoardState currBoardState = boardState;
	
	//null move pruning 
	if (depth >= 3 && !inCheck && Evaluation::ply && Evaluation::canNullMove(board)) {
		board.makenullmove(boardState); 
		++Evaluation::ply;
		Rtable::increment(board.getHashkey());
		const int eval = -Evaluation::negamax(board, boardState, -beta, -alpha, depth - 1 - 2);
		Rtable::decrement();
		board.updateHash(currboardhash);
		--Evaluation::ply;

		board = currBoard;
		boardState = currBoardState;
		if (eval >= beta) {
			return beta;
		}
	}

	Movelist ml; 
	ml.moveGen(board, boardState); 
	Evaluation::sortMove(board, ml); 
	const int mlIndex = ml.getIndex(); 
	

	//increase search depth while in check	
	if (inCheck) {
		++depth; 
	}

	int hashflag = Ttable::tFlagAlpha; 
	int eval; 
	for (int i = 0; i < mlIndex; ++i) {
		const move currmove = ml.getMove(i); 
		board.makemove(currmove, boardState); 
		const map nextmoveBoardhash = board.getHashkey(); 
		
		
		
		//lmr nodes 
		if(movesSearched >= Evaluation::FULLDEPTHMOVES && depth >= Evaluation::REDUCTIONLIMIT && !inCheck && Evaluation::canLMR(ml.getMove(i))){
			//reduction in search 
			++Evaluation::ply; 
			Rtable::increment(currboardhash); 
			eval = -Evaluation::negamax(board, boardState, -alpha - 1, -alpha, depth - (Evaluation::REDUCTIONLIMIT - 1));

			if ((eval > alpha) && (eval < beta)) {
				board.updateHash(nextmoveBoardhash); 
				eval = -Evaluation::negamax(board, boardState, -beta, -alpha, depth - 1);
			}
			Rtable::decrement();
			--Evaluation::ply; 
		}
		//interesting positions that do not qualify for lmr || first few full depth searches 
		else {
			++Evaluation::ply;
			Rtable::increment(currboardhash); 
			eval = -Evaluation::negamax(board, boardState, -beta, -alpha, depth - 1);		
			Rtable::decrement(); 
			--Evaluation::ply;
		}

		++movesSearched; 

		board = currBoard;
		boardState = currBoardState;
		board.updateHash(currboardhash);

		//found better move 
		if (eval > alpha) {
			alpha = eval;

			using namespace Evaluation;
			historyScore[Move::piece(currmove)][Move::targetSquare(currmove)] += depth; 

			pvTable[ply][ply] = currmove; 
			
			for (int nextPly = ply + 1; nextPly < pvLength[ply + 1]; ++nextPly) {
				pvTable[ply][nextPly] = pvTable[ply + 1][nextPly]; 
			}
			pvLength[ply] = pvLength[ply + 1]; 

			hashflag = Ttable::tFlagExact; 
			
			//fail hard beta cut off (oppnent has refutation so this branch will never be played assuming optimal play) 
			if (eval >= beta) {
				if (!Move::captureFlag(currmove)) {
					Evaluation::killerMoves[1][ply] = Evaluation::killerMoves[0][ply];
					Evaluation::killerMoves[0][ply] = currmove;
				}

				Ttable::insert(currboardhash, depth, Ttable::tFlagBeta, beta); 
				return beta;
			}
		}
	}

	const map checkMask = board.checkMask(boardState); 
	if (checkMask != AllOne && !ml.getIndex()) {
		return -mateScore + ply;
	}
	else if (checkMask == AllOne && !ml.getIndex()) {
		//stalemate score
		return 0;
	}

	Ttable::insert(currboardhash, depth, hashflag, alpha); 
	return alpha; 
}


/********************
*sorting Moves
*********************/
int Evaluation::moveScore(const Board& board, move m) {
	
	//pv move 
	if (Evaluation::pvTable[0][ply] == m) {
		return 20000; 
	}
	//capture moves 
	else if (Move::captureFlag(m)) {
		const map targetSquare = indexSquare[Move::targetSquare(m)];

		int targetPiece = wPawn; 
		for (int i = 0; i < 12; ++i) {
			if (targetSquare & board.getPiece()[i]) {
				targetPiece = i; 
				break; 
			}
		}

		return mvvlva[Move::piece(m)][targetPiece] + 10000;
	}

	//quiet moves 
	else {
		//killer move 
		if (Evaluation::killerMoves[0][ply] == m) return 9000;
		else if (Evaluation::killerMoves[1][ply] == m) return 8000;

		//history score 
		else return Evaluation::historyScore[Move::piece(m)][Move::targetSquare(m)]; 
	} 

	return 0; 
}

void Evaluation::sortMove(const Board& board, Movelist& ml) {
	//std::sort();
	std::sort(ml.getMovelist(), ml.getMovelist() + ml.getIndex(),
		[board](const move& lhs, const move& rhs) -> bool {
			return Evaluation::moveScore(board, lhs) > Evaluation::moveScore(board, rhs);
		});
}



/********************
*evaluation 
*********************/
int Evaluation::evaluate(Board& board, BoardState& boardState, int ply){
	return (boardState.getSide() == white ? 1 : -1) * 
		(
		Evaluation::materialEvaluation(board) +
		Evaluation::positionalEvaluation(board) +
		Evaluation::pawnEvaluation(board) +
		Evaluation::rookEvaluation(board)
		); 
		
}

int Evaluation::materialEvaluation(const Board& board) {
	return 
		getNumBit(board.getPiece()[wPawn]) * materialScore[wPawn] +
		getNumBit(board.getPiece()[wKnight]) * materialScore[wKnight] +
		getNumBit(board.getPiece()[wBishop]) * materialScore[wBishop] +
		getNumBit(board.getPiece()[wRook]) * materialScore[wRook] +
		getNumBit(board.getPiece()[wQueen]) * materialScore[wQueen] +

		getNumBit(board.getPiece()[bPawn]) * materialScore[bPawn] +
		getNumBit(board.getPiece()[bKnight]) * materialScore[bKnight] +
		getNumBit(board.getPiece()[bBishop]) * materialScore[bBishop] +
		getNumBit(board.getPiece()[bRook]) * materialScore[bRook] +
		getNumBit(board.getPiece()[bQueen]) * materialScore[bQueen];
}


int Evaluation::positionalEvaluation(const Board& board) {
	int score = 0; 
	//leapers 
	map piece = board.getPiece()[wPawn]; 
	while (piece) {
		score += wPawnPScore[getlsbBitIndex(piece)]; 
		piece &= piece - 1;
	}

	piece = board.getPiece()[bPawn];
	while (piece) {
		score += bPawnPScore[getlsbBitIndex(piece)];
		piece &= piece - 1;
	}

	piece = board.getPiece()[wKnight];
	while (piece) {
		score += wKnightPScore[getlsbBitIndex(piece)];
		piece &= piece - 1;
	}

	piece = board.getPiece()[bKnight];
	while (piece) {
		score += bKnightPScore[getlsbBitIndex(piece)];
		piece &= piece - 1;
	}

	piece = board.getPiece()[wKing];
	while (piece) {
		score += wKingPScore[getlsbBitIndex(piece)];
		piece &= piece - 1;
	}

	piece = board.getPiece()[bKing];
	while (piece) {
		score += bKingPScore[getlsbBitIndex(piece)];
		piece &= piece - 1;
	}

	//sliders 
	piece = board.getPiece()[wBishop];
	while (piece) {
		score += wBishopPScore[getlsbBitIndex(piece)];
		piece &= piece - 1;
	}

	piece = board.getPiece()[bBishop];
	while (piece) {
		score += bBishopPScore[getlsbBitIndex(piece)];
		piece &= piece - 1;
	}

	piece = board.getPiece()[wRook];
	while (piece) {
		score += wRookPScore[getlsbBitIndex(piece)];
		piece &= piece - 1;
	}

	piece = board.getPiece()[bRook];
	while (piece) {
		score += bRookPScore[getlsbBitIndex(piece)];
		piece &= piece - 1;
	}	
	
	return score; 
}

static const int isolatedPawnScore = 20; 
static const int stackedPawnScore = 20; 
int Evaluation::pawnEvaluation(const Board& board) {

	int res = 0;
	const map whitePawns = board.getPiece()[wPawn];
	const map blackPawns = board.getPiece()[bPawn];

	//discourage stacked pawns 
	res -= getNumBit(whitePawns & (whitePawns >> 8)) * stackedPawnScore; 
	res += getNumBit(blackPawns & (blackPawns << 8)) * stackedPawnScore; 

	//white pawns 
	map pawns = whitePawns;
	while (pawns) {
		const int index = getlsbBitIndex(pawns);

		//rewared past pawns 
		if ((pastPawns[white][index] & blackPawns) == 0x0ull) res += pastPawnScore[white][index];
		//discourage isolated pawns 
		if ((isolatedPawn[index] & whitePawns) == 0x0ull) res -= isolatedPawnScore;
		pawns &= pawns - 1;
	}

	//black pawns
	pawns = blackPawns; 
	while (pawns) {
		const int index = getlsbBitIndex(pawns); 

		if ((pastPawns[black][index] & whitePawns) == 0x0ull) res -= pastPawnScore[black][index];
		if ((isolatedPawn[index] & blackPawns) == 0x0ull) res += isolatedPawnScore;

		pawns &= pawns - 1; 
	}

	return res; 
}

static const int openFileScore = 15; 
static const int semiOpenFileScore = 10; 
int Evaluation::rookEvaluation(const Board& board) {

	int res = 0; 
	const map whitePawns = board.getPiece()[wPawn]; 
	const map blackPawns = board.getPiece()[bPawn]; 

	//white rook and queen 
	map rooks = board.getPiece()[wRook] | board.getPiece()[wQueen]; 
	while (rooks) {
		const int index = getlsbBitIndex(rooks); 

		if ((squareFile[index] & (whitePawns | blackPawns)) == 0x0ull) res += openFileScore;
		else if ((squareFile[index] & blackPawns) == 0x0ull) res += semiOpenFileScore; 

		rooks &= rooks - 1; 
	}

	//black rook and queen 
	rooks = board.getPiece()[bRook] | board.getPiece()[bQueen];
	while (rooks) {
		const int index = getlsbBitIndex(rooks); 

		if ((squareFile[index] & (whitePawns | blackPawns)) == 0x0ull) res -= openFileScore;
		else if ((squareFile[index] & whitePawns) == 0x0ull) res -= semiOpenFileScore; 
		
		rooks &= rooks - 1; 
	}

	return res; 
}



