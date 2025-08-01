#pragma once

#include "Bug.h"

class Slitherer : public Bug {
public:
    Slitherer(int id, const Position& pos, Direction dir, int size);
    void move() override;
};
