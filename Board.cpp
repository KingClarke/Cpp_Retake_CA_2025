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
#include <map>
#include <thread>
#include "Slitherer.h"


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

            std::replace(line.begin(), line.end(), ';', ' ');
            std::istringstream stream(line);

            int id, x, y, direction, size, hopLength;
            char type;

            if (stream >> type >> id >> x >> y >> direction >> size) {
                Position pos(x, y);
                if (type == 'C') {
                    bugs.push_back(new Crawler(id, pos, static_cast<Direction>(direction), size));
                } else if (type == 'H' && stream >> hopLength) {
                    bugs.push_back(new Hopper(id, pos, static_cast<Direction>(direction), size, hopLength));
                } else if (type == 'S') {
                    bugs.push_back(new Slitherer(id, pos, static_cast<Direction>(direction), size));
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
        } else if (dynamic_cast<const Slitherer*>(bug)) {
            std::cout << "Slitherer ";
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
    std::cout << "Tapping the board... bugs will move.\n";

    // Step 1: Move alive bugs
    for (auto bug : bugs) {
        if (bug->isAlive()) {
            bug->move();
        }
    }

    // Step 2: Group bugs by current cell
    std::map<Position, std::vector<Bug*>> cellMap;
    for (auto bug : bugs) {
        if (bug->isAlive()) {
            cellMap[bug->getPosition()].push_back(bug);
        }
    }

    // Step 3: Resolve fights in each occupied cell
    for (auto& [pos, bugsHere] : cellMap) {
        if (bugsHere.size() < 2) continue;

        // Find max size
        int maxSize = 0;
        for (auto* b : bugsHere) {
            if (b->getSize() > maxSize) maxSize = b->getSize();
        }

        // Collect all contenders with max size
        std::vector<Bug*> contenders;
        for (auto* b : bugsHere) {
            if (b->getSize() == maxSize) {
                contenders.push_back(b);
            }
        }

        // Choose winner (prefer hopper, else random)
        Bug* winner = nullptr;
        if (contenders.size() == 1) {
            winner = contenders[0];
        } else {
            for (auto* c : contenders) {
                if (dynamic_cast<Hopper*>(c)) {
                    winner = c;
                    break;
                }
            }
            if (!winner) {
                winner = contenders[rand() % contenders.size()];
            }
        }

        // Winner eats all others
        if (winner) {
            winner->eat(bugsHere);
            std::cout << "Bug " << winner->getId() << " ate others at ("
                      << pos.x << "," << pos.y << ") and grew to size "
                      << winner->getSize() << std::endl;
        }
    }

    std::cout << "Tap completed.\n";
}


void Board::displayLifeHistory() const {
    for (const auto& bug : bugs) {
        std::cout << bug->getId() << " ";
        if (dynamic_cast<const Crawler*>(bug)) std::cout << "Crawler ";
        else if (dynamic_cast<const Hopper*>(bug)) std::cout << "Hopper ";
        else if (dynamic_cast<const Slitherer*>(bug)) std::cout << "Slitherer ";

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

void Board::displayCells() const {
    std::map<Position, std::vector<std::string>> cellMap;

    for (auto bug : bugs) {
        if (!bug->isAlive()) continue; // only count alive bugs

        Position pos = bug->getPosition();
        std::ostringstream entry;
        if (dynamic_cast<const Crawler*>(bug)) {
        entry << "Crawler " << bug->getId();
        } else if (dynamic_cast<const Hopper*>(bug)) {
            entry << "Hopper " << bug->getId();
        } else if (dynamic_cast<const Slitherer*>(bug)) {
            entry << "Slitherer " << bug->getId();
        }

        cellMap[pos].push_back(entry.str());
    }

    for (int y = 0; y <= 9; ++y) {
        for (int x = 0; x <= 9; ++x) {
            Position pos{x, y};
            std::cout << "(" << x << "," << y << "): ";
            if (cellMap.count(pos) == 0) {
                std::cout << "empty";
            } else {
                const auto& bugsHere = cellMap.at(pos);
                for (size_t i = 0; i < bugsHere.size(); ++i) {
                    std::cout << bugsHere[i];
                    if (i < bugsHere.size() - 1)
                        std::cout << ", ";
                }
            }
            std::cout << std::endl;
        }
    }
}

void resolveFight(std::vector<Bug*>& bugsInCell) {
    if (bugsInCell.empty()) return;

    // Find the biggest size
    int maxSize = 0;
    for (auto* bug : bugsInCell) {
        if (bug->isAlive() && bug->getSize() > maxSize) {
            maxSize = bug->getSize();
        }
    }

    // Collect all biggest bugs (could be more than one)
    std::vector<Bug*> biggestBugs;
    for (auto* bug : bugsInCell) {
        if (bug->isAlive() && bug->getSize() == maxSize) {
            biggestBugs.push_back(bug);
        }
    }

    // Choose the winner
    Bug* winner = nullptr;
    if (biggestBugs.size() == 1) {
        winner = biggestBugs[0];
    } else {
        // Multiple equal biggest bugs - pick one randomly
        int idx = rand() % biggestBugs.size();
        winner = biggestBugs[idx];
    }

    // Now the winner eats all the others
    int totalSizeEaten = 0;
    for (auto* bug : bugsInCell) {
        if (bug != winner && bug->isAlive()) {
            totalSizeEaten += bug->getSize();
            bug->kill();
            bug->setEatenBy(winner->getId());
        }
    }

    winner->increaseSize(totalSizeEaten);
}


void Board::runSimulation() {
    while (true) {
        tap();
        int aliveCount = 0;
        for (auto bug : bugs) {
            if (bug->isAlive()) {
                aliveCount++;
            }
        }

        if (aliveCount <= 1) {
            std::cout << "Simulation finished! Only one bug remains, panting and victorious~" << std::endl;
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

