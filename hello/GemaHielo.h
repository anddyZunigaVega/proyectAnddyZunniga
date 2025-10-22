#pragma once
#include "Gema.h"

class GemaHielo : public Gema {
public:
    int hits;

    GemaHielo();
    GemaHielo(Texture& tex, int row, int col);

    void setType(int newType, Texture& tex) override;
    void takeHit();
    void setDamagedTexture(Texture& damageTex);
};