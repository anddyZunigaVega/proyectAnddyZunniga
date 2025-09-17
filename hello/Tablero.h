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
    Texture gemTex;
    const float SWAP_SPEED = 600.f;
    const float FALL_SPEED = 900.f;
    const Time MATCH_PAUSE = milliseconds(300);

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
    bool createInitialBoard(int i, int j, int gemType);
    void restart();
    bool tryMove(int row1, int col1, int row2, int col2);
    bool anyMatch();
    void deleteMatch();
    void update(float time); 
    bool updateAnimation(float time, float speed);
    void applyGravityAndGenerate();
};
