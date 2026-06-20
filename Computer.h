#pragma once
#include <cmath>
#include <random>
#include "Board.h"



namespace Computer {










class npc {

private:
    char me; char yu;
    std::mt19937 rangen;
    int simulations = 1000000;
    bool randomExpand(Board::boardClass board, char team);
    double uct(int n, int w, int N){ return ((double)w/n) + std::sqrt(2*(std::log(N)/n)); };

public:
    npc(char human);
    uint64_t getBestMove(Board::boardClass& board, uint64_t& moves);

};




npc::npc(char human){

    std::random_device rd;

    this->me = ('W' + 'B' - human);
    this->yu = human;
    this->rangen = std::mt19937(rd());

}


uint64_t npc::getBestMove(Board::boardClass& board, uint64_t& moves){

    if (__builtin_popcountll(moves) == 1) return __builtin_ctzll(moves);

    int N = 0; int sz = __builtin_popcountll(moves);
    struct child { int i, n, w; }; child children[sz]; sz = 0;
    for (int i = 0; i < 64; i++) if ((1ULL << i) & moves) children[sz++] = { i, 0, 0 };

    for (int i = 0; i < sz; i++){
	Board::boardClass simBoard = board; 
        simBoard.makeMove(this->me, children[i].i / 8, children[i].i % 8);
        if (randomExpand(simBoard, this->yu)) children[i].w++;
        children[i].n++; N++;
    }

    while (N < this->simulations){
	int I = 0; double best = -1000000000;
	for (int i = 0; i < sz; i++) { double UCT = uct(children[i].n, children[i].w, N); if (UCT > best) { best = UCT; I = i; } }
        Board::boardClass simBoard = board; 
        simBoard.makeMove(this->me, children[I].i / 8, children[I].i % 8);
        if (randomExpand(simBoard, this->yu)) children[I].w++;
        children[I].n++; N++;
    }

    int I = 0; int best = -1000000000;
    for (int i = 0; i < sz; i++) if (children[i].n > best) { best = children[i].n; I = i; } 
    return children[I].i;

}


bool npc::randomExpand(Board::boardClass board, char team){

    char oppo = 'W' + 'B' - team;

    while (true){
	uint64_t moves = board.getMoves(team);
        if (moves == 0ULL){
	    std::swap(team, oppo);
	    moves = board.getMoves(team);
	    if (moves == 0ULL) break;
        }
	std::uniform_int_distribution<int> dist(0, __builtin_popcountll(moves) - 1);
	int random = dist(this->rangen);
	for (int k = 0; k < random; k++) moves &= moves - 1;                         // to off rightmost setbit
	int moveIndex = __builtin_ctzll(moves);
	board.makeMove(team, moveIndex / 8, moveIndex % 8);
	std::swap(oppo, team);
    }

    int mee = (this->me == 'B') ? __builtin_popcountll(board.black): __builtin_popcountll(board.white);
    int yuu = (this->yu == 'B') ? __builtin_popcountll(board.black): __builtin_popcountll(board.white);
    return (mee > yuu);

};



}
