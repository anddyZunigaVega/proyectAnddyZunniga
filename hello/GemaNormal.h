#pragma once
#include "Gema.h"

class GemaNormal : public Gema {
public:
    GemaNormal();
    GemaNormal(int newType, Texture& tex, int row, int col);

    void setType(int newType, Texture& tex) override;
};
