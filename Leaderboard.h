#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>












struct record {
    
    int pawns = 0; int opponent = 0;
    std::string name = "              ";
    double ratio = 0;
    
};

std::vector<record> top10(10);
bool loaded = false;

void loadBoard(){

    std::ifstream file("leaderboard.txt");

    for (int i = 0; i < 10; i++){
	file >> top10[i].pawns >> top10[i].opponent >> top10[i].ratio;
	file.ignore(); std::getline(file, top10[i].name);
    }    
    
    file.close();

}

void saveBoard(){

    std::ofstream file("leaderboard.txt");
    
    for (int i = 0; i < 10; i++){
	file << top10[i].pawns << " " << top10[i].opponent << " ";
        file << top10[i].ratio << " " << top10[i].name << "\n";
    }

    file.close();

}

void showLeaderboard(){

    if (loaded == false) { loadBoard(); loaded = true; }
    
    std::cout << "\n##############################################################\n";
    std::cout << "##---------------------- LEADER BOARD ----------------------##\n";
    std::cout << "##            ( all time top 10 sorted by ratio)            ##\n";
    std::cout << "##                                                          ##\n";
    std::cout << "##          name              ratio               pawns     ##\n";
    
    for (int i = 0; i < 10; i++){
        std::cout << "##  " << i << ".  " << top10[i].name << "       ";
        std::cout << std::fixed << std::setprecision(5) << top10[i].ratio << "             ";
        if (top10[i].pawns < 100) std::cout << " ";
        if (top10[i].pawns < 10) std::cout << " ";
        std::cout << top10[i].pawns << "/" << top10[i].opponent << "    ";
        if (top10[i].opponent < 100) std::cout << " ";
        if (top10[i].opponent < 10) std::cout << " ";
        std::cout << "##\n";
    }
    
    std::cout << "##                                                          ##\n";
    std::cout << "##---------------------- LEADER BOARD ----------------------##\n";
    std::cout << "##############################################################\n";
    
}

bool updateLeaderboard(int me, int ai, char* name){

    if (loaded == false) { loadBoard(); loaded = true; }
    int i = 9; double ratio = (double) me/ai;
    if (ratio < top10[9].ratio) return false;
    
    for (int j = 0; j < 14; j++) top10[i].name[j] = name[j];
    
    top10[i].ratio = ratio;
    top10[i].pawns = me;
    top10[i].opponent = ai;
    while (i > 0) { if (top10[i].ratio > top10[i-1].ratio) std::swap(top10[i], top10[i-1]); i--; }
    return true;
    
}