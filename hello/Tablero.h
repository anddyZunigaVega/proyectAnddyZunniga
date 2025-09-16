#pragma once
#include <SFML/Graphics.hpp>
#include "Gema.h"

using namespace sf;
using namespace std;


class Tablero {
 private:
   
    int moves;
    int score;
    static const int N = 8;
    Gema* grid[N][N];
    const float SWAP_SPEED = 600.f;
    const float FALL_SPEED = 900.f;
    const Time MATCH_PAUSE = milliseconds(300);
    Texture gemTex;

    enum GameState { IDLE, SWAPPING, REVERSING, MATCHING_PAUSE, FALLING };
    GameState state;

    Vector2i firstCell, secondCell;
    Clock pauseClock;

 public:
    Tablero();
    ~Tablero();
    int getMoves();
    int getScore();
    void draw(RenderWindow& window);
    bool tryMove(int row1, int col1, int row2, int col2);
    void restart();
    void update(float time); 
    bool anyMatch();
    void deleteMatch();
    bool createInitialBoard(int i, int j, int gemType);
    bool updateAnimation(float time, float speed);
    void applyGravityAndGenerate();
};
