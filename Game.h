#pragma once
#include <string>
#include <chrono>  
#include <algorithm>
#include "Board.h"
#include "Computer.h"


namespace Game {










int takeinput(auto& n, auto& cnt, auto F){
    cnt = 0; n = -1;
    do {
        std::cout << "Enter your choice:    ";
        cnt++;
        if (std::cin >> n && F(n)) return 1;
        if (cnt == 10){
            std::cout << "Too many tries. I cant do this all day. Closing.\n";
            return 0;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while (true);
}





class gameClass {
    
private:
    Board::boardClass board;
    char human;
    char now;

public:
    gameClass(char human);
    void play();
    
};




gameClass::gameClass(char human){
    
    this->human = human;
    this->now = 'B';
    
}


void gameClass::play(){
    
    int i, j; int cnt;
    Computer::npc AI(this->human);
    char username[14];
    
    double min_time = 1e9, max_time = -1.0, total_time = 0.0;
    int ai_moves_count = 0;
    
    while(true){
        this->board.printBoard(); uint64_t moves = board.getMoves(this->now);
        if (moves == 0){
            if (this->now == this->human) std::cout << "You dont have any valid moves ....\n";
            else std::cout << "Computer doesnt have any valid moves ....\n";
	    this->now = 'W' + 'B' - this->now;
            moves = board.getMoves(this->now); if (moves == 0) break;
        }
        if (now == this->human){
	    std::cout << "Your turn, you can place at :   ";
	    while (moves > 0){
		int I = __builtin_ctzll(moves); moves &= moves - 1;
		std::cout << "(" << I/8 << "," << I%8 << ")  ";
	    }
	    std::cout << "\nEnter 2 valid integers space seperated :    ";
            takeinput(i, cnt, [](int x){ return x >= 0 && x < 8; }); takeinput(j, cnt, [](int x){ return x >= 0 && x < 8; });
            if (this->board.isValidMove(this->now, i, j)){ this->board.makeMove(this->now, i, j); this->now = 'W' + 'B' - this->now; }
	    else std::cout << "try again.";
        }
        else {
	    auto start = std::chrono::high_resolution_clock::now();
    	    uint64_t move = AI.getBestMove(this->board, moves);
	    auto end = std::chrono::high_resolution_clock::now();
    	    this->board.makeMove(this->now, move/8, move%8);
	    std::chrono::duration<double> diff = end - start;
            double exec_time = diff.count();
            min_time = std::min(min_time, exec_time);
            max_time = std::max(max_time, exec_time);
            total_time += exec_time;
            ai_moves_count++;
            std::cout << "Execution time : " << exec_time << " seconds\n";
    	    std::cout << "Computer played at : (" << move/8 << "," << move%8 << ")\n";
    	    this->now = 'W' + 'B' - this->now;
    	}
    }
    
    std::cout << "GAME OVER!\n";
    std::cout << "Black pieces: " << __builtin_popcountll(this->board.black) << ((this->human == 'B') ? " (You)\n": " (Npc)\n");
    std::cout << "White pieces: " << __builtin_popcountll(this->board.white) << ((this->human == 'W') ? " (You)\n": " (Npc)\n");
    int humanPawns = (this->human == 'B') ? __builtin_popcountll(this->board.black): __builtin_popcountll(this->board.white);
    int totalPawns = __builtin_popcountll(this->board.black) + __builtin_popcountll(this->board.white);
    while (true){
        std::cout << "Enter your name (must be < 15 chars) :    ";
        std::string s; std::cin >> s; if (s.size() >= 15) continue;
        int k = 0; while (k < s.size()) { username[k] = s[k]; k++; }
        while (k < 14) username[k++] = ' ';
	break;
    }
    bool res = updateLeaderboard(humanPawns, totalPawns - humanPawns, username);

    if (res) std::cout << "Congragulations, You made it to all time top 10 by pawn ratio";
    else std::cout << "Better luck next time, That wasnt all time top 10 performace :)";
    showLeaderboard();

    if (ai_moves_count > 0) {
        std::cout << "\n---------------------------------------\n";
        std::cout << "   AI Execution Time Summary:\n";
        std::cout << "   Min time : " << min_time << " seconds\n";
        std::cout << "   Max time : " << max_time << " seconds\n";
        std::cout << "   Avg time : " << (total_time / ai_moves_count) << " seconds\n";
        std::cout << "---------------------------------------\n";
    }
    
    std::cout << "\nPress any key to exit";
    std::cin.get();
    
}





}