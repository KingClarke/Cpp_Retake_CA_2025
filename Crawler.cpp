#include "Crawler.h"
#include <cstdlib>

Crawler::Crawler(int id, Position pos, Direction dir, int size)
    : Bug(id, pos, dir, size) {}


bool Crawler::isWayBlocked() const {
    switch (direction) {
        case Direction::North: return position.y == 0;
        case Direction::East:  return position.x == 9;
        case Direction::South: return position.y == 9;
        case Direction::West:  return position.x == 0;
        default: return true;
    }
}

void Crawler::move() {
    // If blocked, pick a new direction at random until unblocked
    while (isWayBlocked()) {
        direction = static_cast<Direction>(1 + rand() % 4);
    }

    Position newPos = position;
    switch (direction) {
        case Direction::North: newPos.y -= 1; break;
        case Direction::East:  newPos.x += 1; break;
        case Direction::South: newPos.y += 1; break;
        case Direction::West:  newPos.x -= 1; break;
        default: break;
    }

    setPosition(newPos); // Updates both position and path
}