#pragma once

/********************
*
*Utility 
*
*********************/

static constexpr map squareFile[64] = {
    AFile, BFile, CFile, DFile, EFile, FFile, GFile, HFile, AFile, BFile, CFile, DFile, EFile, FFile, GFile, HFile,
    AFile, BFile, CFile, DFile, EFile, FFile, GFile, HFile, AFile, BFile, CFile, DFile, EFile, FFile, GFile, HFile,
    AFile, BFile, CFile, DFile, EFile, FFile, GFile, HFile, AFile, BFile, CFile, DFile, EFile, FFile, GFile, HFile,
    AFile, BFile, CFile, DFile, EFile, FFile, GFile, HFile, AFile, BFile, CFile, DFile, EFile, FFile, GFile, HFile, };


/********************
*
*material score 
*
*********************/

static constexpr int materialScore[12] = {
	100, 300, 10000, 300, 500, 1000, //wPawn, wKnight, wKing, wBishop, wRook, wQueen
	-100, -300, -10000, -300, -500, -1000
};

/********************
*
*mvv lva
*
* calculated as abs(defender Score/attacker Score)
* 
*********************/

//  (Defender) Pawn Knight Bishop   Rook  Queen   King
//(Attackers)
//      Pawn   105    205    305    405    505    605
//    Knight   104    204    304    404    504    604
//    Bishop   103    203    303    403    503    603
//      Rook   102    202    302    402    502    602
//     Queen   101    201    301    401    501    601
//      King   100    200    300    400    500    600

static constexpr int mvvlva[12][12] = //mvvlva[attacker][defender]
{
{ 105, 205, 605, 305, 405, 505,  105, 205, 605, 305, 405, 505, },
{ 104, 204, 604, 304, 404, 504,  104, 204, 604, 304, 404, 504, },
{ 100, 200, 600, 300, 400, 500,  100, 200, 600, 300, 400, 500, },
{ 103, 203, 603, 303, 403, 503,  103, 203, 603, 303, 403, 503, },
{ 102, 202, 602, 302, 402, 502,  102, 202, 602, 302, 402, 502, },
{ 101, 201, 601, 301, 401, 501,  101, 201, 601, 301, 401, 501, },

{ 105, 205, 605, 305, 405, 505,  105, 205, 605, 305, 405, 505, },
{ 104, 204, 604, 304, 404, 504,  104, 204, 604, 304, 404, 504, },
{ 100, 200, 600, 300, 400, 500,  100, 200, 600, 300, 400, 500, },
{ 103, 203, 603, 303, 403, 503,  103, 203, 603, 303, 403, 503, },
{ 102, 202, 602, 302, 402, 502,  102, 202, 602, 302, 402, 502, },
{ 101, 201, 601, 301, 401, 501,  101, 201, 601, 301, 401, 501, },
};

/********************
*
*positional score
*
*********************/


// pawn positional score
static constexpr int wPawnPScore[64] =
{
    90,  90,  90,  90,  90,  90,  90,  90,
    30,  30,  30,  40,  40,  30,  30,  30,
    20,  20,  20,  30,  30,  30,  20,  20,
    10,  10,  10,  20,  20,  10,  10,  10,
     5,   5,  10,  20,  20,   5,   5,   5,
     0,   0,   0,   5,   5,   0,   0,   0,
     0,   0,   0, -10, -10,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0,
};

static constexpr int bPawnPScore[64] = 
{
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,  10,  10,   0,   0,   0,
     0,   0,   0,  -5,  -5,   0,   0,   0,
    -5,  -5, -10, -20, -20,  -5,  -5,  -5,
   -10, -10, -10, -20, -20, -10, -10, -10,
   -20, -20, -20, -30, -30, -30, -20, -20,
   -30, -30, -30, -40, -40, -30, -30, -30,
   -90, -90, -90, -90, -90, -90, -90, -90,
};

// knight positional score
static constexpr int  wKnightPScore[64] =
{
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,  10,  10,   0,   0,  -5,
    -5,   5,  20,  20,  20,  20,   5,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,   5,  20,  10,  10,  20,   5,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5, -10,   0,   0,   0,   0, -10,  -5,
};

static constexpr int bKnightPScore[64] = 
{
     5,  10,   0,   0,   0,   0,  10,   5,
     5,   0,   0,   0,   0,   0,   0,   5,
     5,  -5, -20, -10, -10, -20,  -5,   5,
     5, -10, -20, -30, -30, -20, -10,   5,
     5, -10, -20, -30, -30, -20, -10,   5,
     5,  -5, -20, -20, -20, -20,  -5,   5,
     5,   0,   0, -10, -10,   0,   0,   5,
     5,   0,   0,   0,   0,   0,   0,   5,
};

// king positional score
static constexpr int  wKingPScore[64] =
{
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   5,   5,   5,   5,   0,   0,
     0,   5,   5,  10,  10,   5,   5,   0,
     0,   5,  10,  20,  20,  10,   5,   0,
     0,   5,  10,  20,  20,  10,   5,   0,
     0,   0,   5,  10,  10,   5,   0,   0,
     0,   5,   5, -15, -15,   0,   5,   0,
     0,   0,   5,   0, -15,   0,  10,   0
};

static constexpr int bKingPScore[64] =
{
     0,   0,  -5,   0,  15,   0, -10,   0,
     0,  -5,  -5,  15,  15,   0,  -5,   0,
     0,   0,  -5, -10, -10,  -5,   0,   0,
     0,  -5, -10, -20, -20, -10,  -5,   0,
     0,  -5, -10, -20, -20, -10,  -5,   0,
     0,  -5,  -5, -10, -10,  -5,  -5,   0,
     0,   0,  -5,  -5,  -5,  -5,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0,
};


// bishop positional score
static constexpr int wBishopPScore[64] =
{
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,  10,  10,   0,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,  10,   0,   0,   0,   0,  10,   0,
     0,  30,   0,   0,   0,   0,  30,   0,
     0,   0, -10,   0,   0, -10,   0,   0,
};

static constexpr int bBishopPScore[64] = 
{
     0,   0,  10,   0,   0,  10,   0,   0,
     0, -30,   0,   0,   0,   0, -30,   0,
     0, -10,   0,   0,   0,   0, -10,   0,
     0,   0, -10, -20, -20, -10,   0,   0,
     0,   0, -10, -20, -20, -10,   0,   0,
     0,   0,   0, -10, -10,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0,
};

// rook positional score
static constexpr int wRookPScore[64] =
{
     50,  50,  50,  50,  50,  50,  50,  50,
     50,  50,  50,  50,  50,  50,  50,  50,
      0,   0,  10,  20,  20,  10,   0,   0,
      0,   0,  10,  20,  20,  10,   0,   0,
      0,   0,  10,  20,  20,  10,   0,   0,
      0,   0,  10,  20,  20,  10,   0,   0,
      0,   0,  10,  20,  20,  10,   0,   0,
      0,   0,   0,  20,  20,   0,   0,   0,
};

static constexpr int bRookPScore[64] = 
{
     0,   0,   0, -20, -20,   0,   0,   0,
     0,   0, -10, -20, -20, -10,   0,   0,
     0,   0, -10, -20, -20, -10,   0,   0,
     0,   0, -10, -20, -20, -10,   0,   0,
     0,   0, -10, -20, -20, -10,   0,   0,
     0,   0, -10, -20, -20, -10,   0,   0,
   -50, -50, -50, -50, -50, -50, -50, -50,
   -50, -50, -50, -50, -50, -50, -50, -50,
};


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



