#include "Board.h"
#include "Position.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "Bug.h"
#include "Crawler.h"
#include "Hopper.h"
#include <chrono>
#include <iomanip>
#include <ctime>

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

Bug* Board::findBug(int id) {
    for (auto bug : bugs) {
        if (bug->getId() == id) {
            return bug;
        }
    }
    return nullptr;
}

void Board::tap() {
    for (auto bug : bugs) {
        if (bug->isAlive()) {
            bug->move();
        }
    }
    std::cout << "All bugs have moved (tap)." << std::endl;
}

void Board::displayLifeHistory() const {
    for (const auto& bug : bugs) {
        std::cout << bug->getId() << " ";
        if (dynamic_cast<const Crawler*>(bug)) std::cout << "Crawler ";
        else if (dynamic_cast<const Hopper*>(bug)) std::cout << "Hopper ";

        std::cout << "Path: ";
        const auto& path = bug->getPath();
        for (const auto& pos : path) {
            std::cout << "(" << pos.x << "," << pos.y << "),";
        }

        if (bug->isAlive()) {
            std::cout << " Alive!" << std::endl;
        } else {
            std::cout << " Eaten by " << bug->getEatenBy() << std::endl;
        }
    }
}

void Board::saveLifeHistoryToFile() const {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    
    std::ostringstream filename;
    filename << "bugs_life_history_" << std::put_time(std::localtime(&now_c), "%Y%m%d_%H%M%S") << ".out";
    
    std::ofstream out(filename.str());
    if (!out) {
        std::cerr << "Error creating output file: " << filename.str() << std::endl;
        return;
    }

    for (const auto& bug : bugs) {
        out << bug->getId() << " ";
        if (dynamic_cast<const Crawler*>(bug)) out << "Crawler ";
        else if (dynamic_cast<const Hopper*>(bug)) out << "Hopper ";

        out << "Path: ";
        const auto& path = bug->getPath();
        for (const auto& pos : path) {
            out << "(" << pos.x << "," << pos.y << "),";
        }

        if (bug->isAlive()) {
            out << " Alive!" << std::endl;
        } else {
            out << " Eaten by " << bug->getEatenBy() << std::endl;
        }
    }

    out.close();
    std::cout << "Bug life histories saved to " << filename.str() << std::endl;
}