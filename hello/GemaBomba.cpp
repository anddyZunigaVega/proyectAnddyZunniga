#include "GemaBomba.h"

GemaBomba::GemaBomba() : Gema() { type = 6; }

GemaBomba::GemaBomba(Texture& tex, int row, int col) {
    try {
        type = 6;
        sprite.setTexture(tex);
        sprite.setTextureRect(IntRect(0, 0, 130, 130));
        sprite.setPosition(col * 130, row * 130);
        targetPos = sprite.getPosition();
    }
    catch (int valor) {
        cout << "Error en GemaBomba::constructor, recurso inválido" << endl;
        exit(1);
    }
}

void GemaBomba::setType(int newType, Texture& tex) {
    try {
        if (newType != 6 && newType != -1)
            throw(newType);

        type = newType;
        sprite.setTexture(tex);
        sprite.setTextureRect(IntRect(0, 0, 130, 130));
    }
    catch (int valor) {
        cout << "Error en GemaBomba::setType, tipo invalido: " << valor << endl;
        exit(1);
    }
}
