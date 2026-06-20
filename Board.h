#pragma once
#include "Leaderboard.h"
#include <cstdint>



namespace Board {










class boardClass {
    
public:
    uint64_t black = (1ULL << 28) + (1ULL << 35);
    uint64_t white = (1ULL << 27) + (1ULL << 36);
    void printBoard();
    void makeMove(char team, int i, int j);
    bool isValidMove(char team, int i, int j);
    uint64_t getMoves(char team);

};







void boardClass::printBoard(){

    std::cout << "\n\n";
    char peas[] = {'.', 'B', 'W'};

    for (int i = 0; i < 8; i++){
	std::cout << i << "   ";
	for (int j = 0; j < 8; j++){
	    uint64_t mask = 1ULL << (8*i + j);
	    std::cout << peas[!!(this->black&mask) + 2*!!(this->white&mask)] << " ";
	}
	std::cout << "\n";
    }

    std::cout << "    0 1 2 3 4 5 6 7";
    std::cout << "\n\n";

}


bool boardClass::isValidMove(char team, int i, int j){

    if (i < 0 || i >= 8 || j < 0 || j >= 8) return false;
    uint64_t mask = 1ULL << (8*i + j);
    if ((black&mask) || (white&mask)) return false;
    uint64_t& me = (team == 'B') ? this->black: this->white;
    uint64_t& yu = (team == 'B') ? this->white: this->black;
    uint64_t not0 = 0xfefefefefefefefeULL;
    uint64_t not7 = 0x7f7f7f7f7f7f7f7fULL;
    uint64_t m;

    m = (mask << 1) & not0 & yu; while (m&yu) m = (m << 1) & not0; if (m&me) return true;
    m = (mask >> 1) & not7 & yu; while (m&yu) m = (m >> 1) & not7; if (m&me) return true;
    m = (mask << 8) & yu; while (m&yu) m <<= 8; if (m&me) return true;
    m = (mask >> 8) & yu; while (m&yu) m >>= 8; if (m&me) return true;
    m = (mask << 9) & not0 & yu; while (m&yu) m = (m << 9) & not0; if (m&me) return true;
    m = (mask << 7) & not7 & yu; while (m&yu) m = (m << 7) & not7; if (m&me) return true;
    m = (mask >> 7) & not0 & yu; while (m&yu) m = (m >> 7) & not0; if (m&me) return true;
    m = (mask >> 9) & not7 & yu; while (m&yu) m = (m >> 9) & not7; if (m&me) return true;

    return false;

}


void boardClass::makeMove(char team, int i, int j){

    uint64_t mask = 1ULL << (8*i + j);
    uint64_t& me = (team == 'B') ? this->black: this->white;
    uint64_t& yu = (team == 'B') ? this->white: this->black;
    uint64_t not0 = 0xfefefefefefefefeULL;
    uint64_t not7 = 0x7f7f7f7f7f7f7f7fULL;
    uint64_t m; me |= mask;
    
    m = (mask << 1) & not0 & yu; while (m&yu) m = (m << 1) & not0;             // left
    if (m&me) { m >>= 1; while (m&yu) { me |= m; yu &= ~m; m >>= 1; } }
    m = (mask >> 1) & not7 & yu; while (m&yu) m = (m >> 1) & not7;             // right
    if (m&me) { m <<= 1; while (m&yu) { me |= m; yu &= ~m; m <<= 1; } }
    m = (mask << 8) & yu; while (m&yu) m <<= 8;                                // up
    if (m&me) { m >>= 8; while (m&yu) { me |= m; yu &= ~m; m >>= 8; } }
    m = (mask >> 8) & yu; while (m&yu) m >>= 8;                                // down
    if (m&me) { m <<= 8; while (m&yu) { me |= m; yu &= ~m; m <<= 8; } }
    m = (mask << 9) & not0 & yu; while (m&yu) m = (m << 9) & not0;             // up right
    if (m&me) { m >>= 9; while (m&yu) { me |= m; yu &= ~m; m >>= 9; } } 
    m = (mask << 7) & not7 & yu; while (m&yu) m = (m << 7) & not7;             // up left
    if (m&me) { m >>= 7; while (m&yu) { me |= m; yu &= ~m; m >>= 7; } } 
    m = (mask >> 7) & not0 & yu; while (m&yu) m = (m >> 7) & not0;             // down right
    if (m&me) { m <<= 7; while (m&yu) { me |= m; yu &= ~m; m <<= 7; } }
    m = (mask >> 9) & not7 & yu; while (m&yu) m = (m >> 9) & not7;             // down left
    if (m&me) { m <<= 9; while (m&yu) { me |= m; yu &= ~m; m <<= 9; } }

}


uint64_t boardClass::getMoves(char team){

    uint64_t& me = (team == 'B') ? this->black: this->white;
    uint64_t& yu = (team == 'B') ? this->white: this->black;
    uint64_t moves = 0ULL;
    uint64_t not0 = 0xfefefefefefefefeULL; 
    uint64_t not7 = 0x7f7f7f7f7f7f7f7fULL;
    uint64_t whatnot = ~0ULL;
    uint64_t empty = ~(me | yu);
    
    auto lambda = [&](auto shift, uint64_t& mask){
	uint64_t c = (yu & mask & shift(me));
	c |= (yu & mask & shift(c));
	c |= (yu & mask & shift(c));
        c |= (yu & mask & shift(c));
        c |= (yu & mask & shift(c));
        c |= (yu & mask & shift(c));
        moves |= empty & mask & shift(c);
    };

    lambda([](uint64_t x){ return x << 1; }, not0);     // right
    lambda([](uint64_t x){ return x << 8; }, whatnot);     // down
    lambda([](uint64_t x){ return x << 9; }, not0);     // down right
    lambda([](uint64_t x){ return x << 7; }, not7);     // down left
    lambda([](uint64_t x){ return x >> 1; }, not7);    // left
    lambda([](uint64_t x){ return x >> 8; }, whatnot);    // up
    lambda([](uint64_t x){ return x >> 9; }, not7);    // up left
    lambda([](uint64_t x){ return x >> 7; }, not7);    // up down;

    return moves;

}




}