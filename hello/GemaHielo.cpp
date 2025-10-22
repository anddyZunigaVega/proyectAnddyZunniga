#include "GemaHielo.h"

GemaHielo::GemaHielo() : Gema() {
    hits = 2;
    type = 5;
}

GemaHielo::GemaHielo(Texture& tex, int row, int col) {
    try {
        hits = 2;
        type = 5;
        sprite.setTexture(tex);
        sprite.setTextureRect(IntRect(0, 0, 130, 130));
        sprite.setPosition(col * 130, row * 130);
        targetPos = sprite.getPosition();
    }
    catch (...) {
        cout << "Error en GemaHielo::constructor, recurso inválido" << endl;
        exit(1);
    }
}

void GemaHielo::setType(int newType, Texture& tex) {
    try {
        if (newType != -1 && newType != 5)
            throw(newType);

        sprite.setTexture(tex);
        sprite.setTextureRect(IntRect(0, 0, 130, 130));
        type = newType;
    }
    catch (int valor) {
        cout << "Error en GemaHielo::setType, tipo invalido: " << valor << endl;
        exit(1);
    }
}

void GemaHielo::takeHit() {
    hits--;
}

void GemaHielo::setDamagedTexture(Texture& damageTex) {
    sprite.setTexture(damageTex);
}
