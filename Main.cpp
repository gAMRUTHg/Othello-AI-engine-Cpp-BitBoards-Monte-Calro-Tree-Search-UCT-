#include <iostream>
#include "Game.h"
#include "Leaderboard.h"








// g++ -O3 -march=native -std=c++20 Main.cpp -o Othello.exe








int main(){
    
    std::cout << "\n#########################################\n";
    std::cout << "##                                     ##\n";
    std::cout << "##     MAIN MENU                       ##\n";
    std::cout << "##      1. Play                        ##\n";
    std::cout << "##      2. Leaderboard                 ##\n";
    std::cout << "##      3. Exit                        ##\n";
    std::cout << "##                                     ##\n";
    std::cout << "#########################################\n";

    int n, cnt;
    if (0 == Game::takeinput(n, cnt, [](int x){ return (x == 1 || x == 2 || x == 3); })) return 0;
    if (n == 3) { std::cout << "Closing."; system("pause"); return 0; }
    if (n == 2) { showLeaderboard(); system("pause"); return 0; }
    
    std::cout << "#######################################################################\n";
    std::cout << "##----------------------------- WELCOME -----------------------------##\n";
    std::cout << "##                                                                   ##\n";
    std::cout << "##   Rules:                                                          ##\n";
    std::cout << "##   1. Black moves first then alternate.               Board        ##\n";
    std::cout << "##   2. You must place a pawn to trap at least     . . . . . . . .   ##\n";
    std::cout << "##      one opponent pawn between your pawns.      . . . . . . . .   ##\n";
    std::cout << "##   3. Trapped pawns must be in a straight line   . . . . . . . .   ##\n";
    std::cout << "##      (horizontal, vertical, or diagonal).       . . . W B . . .   ##\n";
    std::cout << "##   4. All trapped pawns flip to your color.      . . . B W . . .   ##\n";
    std::cout << "##   5. If you have no valid moves, you pass       . . . . . . . .   ##\n";
    std::cout << "##   6. pawns are never moved, only flipped.       . . . . . . . .   ##\n";
    std::cout << "##   7. Game ends when neither player can move.    . . . . . . . .   ##\n";
    std::cout << "##   8. The player with the most pawns wins.                         ##\n";
    std::cout << "##                                                                   ##\n";
    std::cout << "#######################################################################\n";
    
    std::cout << "Choose your team [1 for Black | 2 for white | 0 for quit]: ";
    if (0 == Game::takeinput(n, cnt, [](int x){ return (x == 1 || x == 2 || x == 0); })) return 0;
    if (n == 0) { std::cout << "Closing."; system("pause"); return 0; }
    
    Game::gameClass match((n == 1) ? 'B': 'W');
    match.play();
    system("pause");
    return 0;
    
}