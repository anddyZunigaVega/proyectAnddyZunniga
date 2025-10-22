#include "Gema.h"

Gema::Gema() : type(-1) {}

Gema::~Gema() {}

void Gema::setType(int newType, Texture& tex) {
    try {
        if (newType < -1 || newType > 6)
            throw(newType);

        type = newType;
        sprite.setTexture(tex);
        sprite.setTextureRect(IntRect(type * 130, 0, 130, 130));
    }
    catch (int valor) {
        cout << "Error en Gema::setType, tipo invalido: " << valor << endl;
        exit(1);
    }
}

void Gema::setPos(int row, int col) {
    try {
        if (row < 0 || col < 0)
            throw(-1);

        sprite.setPosition(col * 130, row * 130);
        targetPos = sprite.getPosition();
    }
    catch (int valor) {
        cout << "Error en Gema::setPos, posición invalida: " << valor << endl;
        exit(1);
    }
}

int Gema::getType() {
    return type;
}