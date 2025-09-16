#pragma once
#include <SFML/Graphics.hpp>
#include "Tablero.h"
#include "string"


using namespace sf;
using namespace std;


class Juego {
 private:
    bool selected;
    Vector2i firstSelect;
    RenderWindow window;
    Tablero tablero;
    Clock clock;
    Font font;
    Text scoreText;
    Text finalScore;
    Texture backGroundTex1;
    Sprite backGround1;
    Texture backGroundTex2;
    Sprite backGround2;
    RectangleShape exitButton;
    
 public:
    Juego();
    void run();
    void processEvents();
    void render();
    
};
