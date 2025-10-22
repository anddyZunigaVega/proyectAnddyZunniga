#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;

class Gema {
public:
    int type;
    Sprite sprite;
    Vector2f targetPos;

    Gema();
    virtual ~Gema();

    virtual void setType(int newType, Texture& tex);
    virtual void setPos(int row, int col);
    virtual int getType();
};