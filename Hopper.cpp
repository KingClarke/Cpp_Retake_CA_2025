#include "Hopper.h"
#include <cstdlib>

Hopper::Hopper(int id, Position pos, Direction dir, int size, int hopLen)
    : Bug(id, pos, dir, size), hopLength(hopLen) {
    if (hopLength < 2) hopLength = 2;
    if (hopLength > 4) hopLength = 4;
}

Hopper::~Hopper() {}

int Hopper::getHopLength() const {
    return hopLength;
}

bool Hopper::isWayBlocked() const {
    switch (direction) {
        case Direction::North: return position.y - hopLength < 0;
        case Direction::East:  return position.x + hopLength > 9;
        case Direction::South: return position.y + hopLength > 9;
        case Direction::West:  return position.x - hopLength < 0;
        default: return true;
    }
}

void Hopper::move() {
    // If blocked, pick a new direction at random until unblocked
    while (isWayBlocked()) {
        direction = static_cast<Direction>(1 + rand() % 4);
    }

    Position newPos = position;
    int actualHop = hopLength;

    switch (direction) {
        case Direction::North:
            if (position.y - hopLength < 0)
                actualHop = position.y;
            newPos.y -= actualHop;
            break;
        case Direction::East:
            if (position.x + hopLength > 9)
                actualHop = 9 - position.x;
            newPos.x += actualHop;
            break;
        case Direction::South:
            if (position.y + hopLength > 9)
                actualHop = 9 - position.y;
            newPos.y += actualHop;
            break;
        case Direction::West:
            if (position.x - hopLength < 0)
                actualHop = position.x;
            newPos.x -= actualHop;
            break;
        default:
            break;
    }

    setPosition(newPos); // updates both position and path
};