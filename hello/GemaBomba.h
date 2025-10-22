#pragma once
#include "Gema.h"

class GemaBomba : public Gema {
public:
    GemaBomba();
    GemaBomba(Texture& tex, int row, int col);

    void setType(int newType, Texture& tex) override;
};