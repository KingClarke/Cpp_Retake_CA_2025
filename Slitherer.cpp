#include "Slitherer.h"
#include <iostream>

Slitherer::Slitherer(int id, const Position& pos, Direction dir, int size)
    : Bug(id, pos, dir, size) {}

void Slitherer::move() {
    if (!alive) return;

    Position newPos = position;

    // Slitherers zigzag: alternate between moving forward and turning
    if (path.size() % 2 == 0) {
        switch (direction) {
            case Direction::North: newPos.y--; break;
            case Direction::East: newPos.x++; break;
            case Direction::South: newPos.y++; break;
            case Direction::West: newPos.x--; break;
        }
    } else {
        direction = static_cast<Direction>((static_cast<int>(direction) + 1) % 4); // Turn right
    }

    // Keep within bounds (0–9)
    newPos.x = std::max(0, std::min(9, newPos.x));
    newPos.y = std::max(0, std::min(9, newPos.y));
    position = newPos;

    path.push_back(position);
}
