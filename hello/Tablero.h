#pragma once
#include <SFML/Graphics.hpp>
#include "Gema.h"
#include "GemaNormal.h"
#include "GemaHielo.h"
#include "GemaBomba.h"
#include "Lista.h"
#include <fstream>
#include <string>

using namespace sf;
using namespace std;

class Tablero {
private:
    int moves;
    int score;

    //ranking
    string name;           
    int allMoves;
    int allScore;

    bool win;
    static const int N = 8;
    Gema* grid[N][N];
    Texture gemTex;
    Texture iceTex;
    Texture iceDamageTex;
    Texture bombTex;
    const float SWAP_SPEED = 600.f;
    const float FALL_SPEED = 900.f;
    const Time MATCH_PAUSE = milliseconds(300);

    enum GameState { IDLE, SWAPPING, REVERSING, MATCHING_PAUSE, FALLING };
    GameState state;

    Vector2i firstCell, secondCell;
    Clock pauseClock;

    
    int level;
    int iceRemaining;
    int bombsActivated;

    int targetScore;
    int targetMoves;
    int targetIce;
    int targetBombsToActivate;

public:
    Tablero();
    ~Tablero();
    int getMoves();
    int getScore();

    bool getStateIDLE();
    void draw(RenderWindow& window);
    bool createInitialBoard(int i, int j, int gemType);
    void restart();
    bool tryMove(int row1, int col1, int row2, int col2);
    bool anyMatch();
    void deleteMatch();
    void update(float time);
    bool updateAnimation(float time, float speed);
    void applyGravityAndGenerate();
    int getTargetScore();
    void setLevel(int newLevel);
    int getLevel();
    bool getWin();
    int getIceRemaining();
    int getBombsActivated();
    bool objectivesMet();

    //ranking
	void setName(string nName);
    int getAllScore();
    int getAllMoves();
    void addToRanking();
};