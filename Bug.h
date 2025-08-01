#pragma once
#include <list>
#include "Position.h"

enum class Direction {
    North = 1,
    East,
    South,
    West
};

class Bug {
protected:
    int id;
    Position position;
    Direction direction;
    int size;
    bool alive;
    std::list<Position> path;
    int eatenBy;

public:
    Bug(int id, Position position, Direction direction, int size)
        : id(id), position(position), direction(direction), size(size), alive(true), eatenBy(0) {}

    virtual ~Bug() = default;
    virtual void move() = 0;

    bool isWayBlocked(const Position& newPos) const {
        for (const auto& pos : path) {
            if (pos == newPos) {
                return true;
            }
        }
        return false;
    }

    virtual bool canEat(const Bug* other) const {
        return size > other->getSize();
    }

    virtual void eat(std::vector<Bug*>& bugsInCell) {
        for (auto* other : bugsInCell) {
            if (other != this && other->isAlive()) {
                other->kill();
                other->setEatenBy(this->getId());
                this->increaseSize(other->getSize());
            }
        }
    }

    int getId() const { return id; }
    bool isAlive() const { return alive; }
    Position getPosition() const { return position; }
    int getSize() const { return size; }
    Direction getDirection() const { return direction; }
    const std::list<Position>& getPath() const { return path; }
    int getEatenBy() const { return eatenBy; }

    void increaseSize(int amount) { size += amount; }
    void kill() { alive = false; }
    void setAlive(bool status) { alive = status; }
    void setPosition(const Position& newPosition) {
        position = newPosition;
        path.push_back(newPosition);
    }
    void setDirection(Direction newDirection) { direction = newDirection; }
    void setEatenBy(int id) { eatenBy += id; }
};