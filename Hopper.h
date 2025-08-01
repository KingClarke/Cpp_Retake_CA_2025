#pragma once
#include "Bug.h"

class Hopper : public Bug {
private:
    int hopLength;

public:
    Hopper(int id, Position position, Direction direction, int size, int hopLength)
        : Bug(id, position, direction, size), hopLength(hopLength) {}
        
    bool canEat(const Bug* other) const override {
        return getSize() >= other->getSize(); // Hopper wins even on tie~
    }

    void move() override;
    int getHopLength() const;
    bool isWayBlocked() const override;

};