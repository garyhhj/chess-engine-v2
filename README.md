# chess engine

## Introduction 

This is a uci chess engine written in c++. 
It is statically evaluated and has an estimated strength of ~1700elo. 

## demo 

![gif of demo has gone bad](https://media.giphy.com/media/vFKqnCdLPNOKc/giphy.gif)

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

### evaluation 
- material score 
- positional evaluation 
- basic pawn evaluation 
- basic rook evaluation

## todo
- opening book 
- time control 

## references 

[magic bitboard chess engine guide](https://www.youtube.com/watch?v=QUNP-UjujBM&list=PLmN0neTso3Jxh8ZIylk74JpwfiWNI76Cs) by Code Monkey King. 

[chess programming webiste](https://www.chessprogramming.org/Main_Page) that explains concepts used 

