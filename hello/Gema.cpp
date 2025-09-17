#include "Gema.h"

Gema::Gema() {
    type = -1;
}

Gema::Gema(int newType, Texture& tex, int row, int col) {
    type = newType;
    sprite.setTexture(tex);
    sprite.setTextureRect(IntRect(type * 130, 0, 130, 130));
    sprite.setPosition(col * 130, row * 130);
    targetPos = sprite.getPosition();
}

void Gema::setType(int newType, Texture& tex) {
    type = newType;
    sprite.setTexture(tex);
    sprite.setTextureRect(IntRect(type * 130, 0, 130, 130));
}

void Gema::setPos(int row, int col) {
    sprite.setPosition(col * 130, row * 130);
    targetPos = sprite.getPosition();
}

Gema::~Gema() {}
