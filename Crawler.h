#pragma once
#include "Bug.h"

class Crawler : public Bug {
public:
    Crawler(int id, Position position, Direction direction, int size)
        : Bug(id, position, direction, size) {}

    void move() override;
    bool isWayBlocked() const override;
};