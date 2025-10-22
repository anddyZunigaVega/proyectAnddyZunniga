#include "GemaNormal.h"

GemaNormal::GemaNormal() : Gema() {}

GemaNormal::GemaNormal(int newType, Texture& tex, int row, int col) {
    try {
        setType(newType, tex);
        sprite.setPosition(col * 130, row * 130);
        targetPos = sprite.getPosition();
    }
    catch (int valor) {
        cout << "Error en GemaNormal::constructor, tipo invalido: " << valor << endl;
        exit(1);
    }
}

void GemaNormal::setType(int newType, Texture& tex) {
    try {
        Gema::setType(newType, tex);
    }
    catch (int valor) {
        cout << "Error en GemaNormal::setType, tipo invalido: " << valor << endl;
        exit(1);
    }
}