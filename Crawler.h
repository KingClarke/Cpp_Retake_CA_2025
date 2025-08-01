#pragma once
#include "Bug.h"
#include <cstdlib>
#include <ctime>

class Crawler : public Bug {
public:
    Crawler(int id, Position pos, Direction dir, int size)
        : Bug(id, pos, dir, size) {
        // Seed random only once per program run (best done in main honestly)
        static bool seeded = false;
        if (!seeded) {
            srand(static_cast<unsigned int>(time(nullptr)));
            seeded = true;
        }
    }

    // Check if the crawler is at the edge and facing out of bounds
    bool isWayBlocked() const override {
        switch (direction) {
            case Direction::North: return position.y == 0;
            case Direction::East:  return position.x == 9;
            case Direction::South: return position.y == 9;
            case Direction::West:  return position.x == 0;
            default: return true;
        }
    }

    void move() override {
        // Try to move, if blocked, pick a new direction at random until unblocked
        while (isWayBlocked()) {
            direction = static_cast<Direction>(1 + rand() % 4);
        }
        switch (direction) {
            case Direction::North: position.y -= 1; break;
            case Direction::East:  position.x += 1; break;
            case Direction::South: position.y += 1; break;
            case Direction::West:  position.x -= 1; break;
            default: break;
        }
        path.push_back(position);