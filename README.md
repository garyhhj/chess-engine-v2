# chess engine

## Introduction 

This is a uci chess engine written in c++. 
It is statically evaluated and has an estimated strength of 1700elo. 

## demo 

[insert gif of engine playing against itself]

## features 

### move generation
- legal move generator 
- checkmask & pinmasks to prune illegal moves 

### search 
- negamax algorithm 
- alpha beta pruning 
- null move pruning 
- late move reduction 
- quiesence search 
- pv table 
- transposition table 
- iterative depening 

### evaluation 
- material score 
- positional evaluation 
- basic pawn evaluation 
- basic rook evaluation

## todo  
- implement time control 
- opening book 

