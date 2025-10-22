#pragma once
#include <SFML/Graphics.hpp>
#include "Tablero.h"
#include "string"

using namespace sf;
using namespace std;

class Juego {
private:
    int currentLevel;
    float progress;
    bool selected;
    bool waitingForEnter;

    Vector2i firstSelect;
    RenderWindow window;
    Tablero tablero;
    Clock clock;
    Font font;

    Text scoreText;
    Text finalScore;
    Text progressText;

    Texture backGroundTex1;
    Sprite backGround1;

    Texture backGroundTex2;
    Sprite backGround2;

    Texture backGroundTexLevel2;
    Sprite backGroundLevel2;

    Texture backGroundTexLevel3;
    Sprite backGroundLevel3;

    Texture backGroundTexFinal;
    Sprite backGroundFinal;

    Texture backGroundTexWait;
    Sprite backGroundWait;

    RectangleShape exitButton;
    RectangleShape progressBar;


public:
    Juego();
    void run();
    void processEvents();
    void render();
};