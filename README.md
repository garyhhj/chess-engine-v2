# chess engine

## Demo 

![chess-engine-demo](https://user-images.githubusercontent.com/54121714/236674622-c532858e-66bf-40a9-996f-3a0ea63cc614.gif)


## Introduction 

This is a uci chess engine written in c++. 
It is statically evaluated and has won against chess bots of ~1800elo. 

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

[fully legal move generator](https://www.codeproject.com/Articles/5313417/Worlds-Fastest-Bitboard-Chess-Movegenerator) article 

