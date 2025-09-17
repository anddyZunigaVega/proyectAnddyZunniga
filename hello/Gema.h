#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Gema {
public:
    int type;
    Sprite sprite;
    Vector2f targetPos;

    Gema();
    Gema(int newType, Texture& tex, int row, int col);
    void setType(int newType, Texture& tex);
    void setPos(int row, int col);
    ~Gema();
};


