#pragma once
/********************
*
*material score 
*
*********************/

static constexpr int materialScore[12] = {
	100, 300, 0, 300, 500, 900, //wPawn, wKnight, wKing, wBishop, wRook, wQueen
	-100, -300, 0, -300, -500, -900
};

/********************
*
*mvv lva
*
* calculated as abs(defender Score/attacker Score)
* 
*********************/

static constexpr int mvvlva[12][12] = //mvvlva[attacker][defender]
{
{0, 0, 0, 0, 0, 0, 1, 3, 1, 3, 5, 9, },
{0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 3, },
{0, 0, 0, 0, 0, 0, 1, 3, 1, 3, 5, 9, },
{0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 3, },
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, },
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
{1, 3, 1, 3, 5, 9, 0, 0, 0, 0, 0, 0, },
{0, 1, 0, 1, 1, 3, 0, 0, 0, 0, 0, 0, },
{1, 3, 1, 3, 5, 9, 0, 0, 0, 0, 0, 0, },
{0, 1, 0, 1, 1, 3, 0, 0, 0, 0, 0, 0, },
{0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, },
{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, },
};

/********************
*
*positional score
*
*********************/

//aiya writing this seems annoying 


//enum piece : const int {
//	wPawn = 0,
//	wKnight = 1,
//	wKing = 2,
//	wBishop = 3,
//	wRook = 4,
//	wQueen = 5,
//
//	bPawn = 6,
//	bKnight = 7,
//	bKing = 8,
//	bBishop = 9,
//	bRook = 10,
//	bQueen = 11,
//};
