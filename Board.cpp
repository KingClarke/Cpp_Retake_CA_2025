#include "Board.h"
#include "Position.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "Bug.h"
#include "Crawler.h"
#include "Hopper.h"

Board::Board() {
    std::cout << "Board initialized.\n";
}

Board::~Board() {
    for (auto bug : bugs) {
        delete bug;
    }
    std::cout << "Board destroyed.\n";
}

void Board::loadBugs(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;

            std::replace(line.begin(), line.end(), ',', ' '); // In case commas are used
            std::istringstream stream(line);

            int id, x, y, direction, size, hopLength;
            char type;

            if (stream >> type >> id >> x >> y >> direction >> size) {
                Position pos(x, y);
                if (type == 'C') {
                    bugs.push_back(new Crawler(id, pos, static_cast<Direction>(direction), size));
                } else if (type == 'H' && stream >> hopLength) {
                    bugs.push_back(new Hopper(id, pos, static_cast<Direction>(direction), size, hopLength));
                } else {
                    std::cerr << "Invalid data for bug type " << type << std::endl;
                }
            } else {
                std::cerr << "Error parsing line: " << line << std::endl;
            }
        }
        file.close();
        std::cout << "Bugs loaded from file.\n";
    } else {
        std::cerr << "Failed to open file: " << filename << std::endl;
    }
}

void Board::displayBugs() const {
    for (auto bug : bugs) {
        std::cout << bug->getId() << " ";
        if (dynamic_cast<const Crawler*>(bug)) {
            std::cout << "Crawler ";
        } else if (dynamic_cast<const Hopper*>(bug)) {
            std::cout << "Hopper ";
        }
        Position pos = bug->getPosition();
        std::cout << "(" << pos.x << "," << pos.y << ") ";
        std::cout << bug->getSize() << " ";
        Direction dir = bug->getDirection();
        if (dir == Direction::North) std::cout << "North ";
        else if (dir == Direction::East) std::cout << "East ";
        else if (dir == Direction::South) std::cout << "South ";
        else if (dir == Direction::West) std::cout << "West ";
        if (auto h = dynamic_cast<const Hopper*>(bug)) {
            std::cout << h->getHopLength() << " ";
        }
        std::cout << (bug->isAlive() ? "Alive" : "Dead") << std::endl;
    }
}