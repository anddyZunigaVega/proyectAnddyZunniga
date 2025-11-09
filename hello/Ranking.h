#pragma once
#include "Tablero.h"
#include "string"
#include <iostream>
#include <fstream>

using namespace std;

class Ranking{

 private:
	string name;
	int score;
	int moves;
	float scoreForMoves;

 public:
	Ranking() {}
	Ranking(string nName,int nScore,int nMoves):name(nName),score(nScore),moves(nMoves),scoreForMoves(score/moves){}
	float getScoreForMoves() { return scoreForMoves; }
	int getScore() { return score; }
	int getMoves() { return moves; }
	string getName() { return name; }
};