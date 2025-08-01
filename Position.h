#pragma once

#include <unordered_map>
#include <vector>

struct Position {
    int x;
    int y;

    Position(int xVal, int yVal) : x(xVal), y(yVal) {}

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }

    // Required for std::map:
    bool operator<(const Position& other) const {
        return (y < other.y) || (y == other.y && x < other.x);
    }
};

namespace std {
    template <>
    struct hash<Position> {
        size_t operator()(const Position& p) const {
            return hash<int>()(p.x) ^ (hash<int>()(p.y) << 1);
        }
    };
}
