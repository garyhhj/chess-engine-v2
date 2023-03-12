#pragma once
#include "macro.h"
#include "board.h"
#include "movelist.h"

#include <chrono>

//some function for timers 

int perftHelper(Board& board, BoardState& boardState, int depth);

void perft(Board& board, BoardState& boardState, int depth);