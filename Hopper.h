#pragma once
#include "Bug.h"

class Hopper : public Bug {
private:
    int hopLength;

public:
    Hopper(int id, Position pos, Direction dir, int size, int hopLen);
    virtual ~Hopper();

    int getHopLength() const;
    bool isWayBlocked() const override;
    void move() override;
};