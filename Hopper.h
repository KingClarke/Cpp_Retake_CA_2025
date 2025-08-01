#pragma once
#include "Bug.h"

class Hopper : public Bug {
private:
    int hopLength;

public:
    Hopper(int id, Position position, Direction direction, int size, int hopLength)
        : Bug(id, position, direction, size), hopLength(hopLength) {}

    void move() override;

private:
    Direction getRandomDirection() const;
};