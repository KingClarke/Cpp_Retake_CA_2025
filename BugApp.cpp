#include "Board.h"
#include <iostream>

void showMenu() {
    std::cout << "\n----- Bug's Life Menu -----\n";
    std::cout << "1. Initialize Bug Board (load from file)\n";
    std::cout << "2. Display All Bugs\n";
    std::cout << "3. Find a Bug\n";
    std::cout << "4. Tap the Bug Board (Move/Fight/Eat)\n";
    std::cout << "5. Display Life History of all Bugs\n";
    std::cout << "6. Display all Cells\n";
    std::cout << "7. Run Simulation (Auto Tapping every 1s)\n";
    std::cout << "8. Exit (Save Life Histories and Leave)\n";
    std::cout << "Choose your next move: ";
}

void BugApp() {
    Board board;
    bool running = true;

    while (running) {
        showMenu();
        int choice;
        std::cin >> choice;
        switch (choice) {
            case 1: {
                std::string filename;
                std::cout << "Enter filename to load bugs (e.g., bugs.txt): ";
                std::cin >> filename;
                board.loadBugs(filename);
                break;
            }
            case 2:
                board.displayBugs();
                break;
            case 3: {
                int id;
                std::cout << "Enter Bug ID to find: ";
                std::cin >> id;
                Bug* foundBug = board.findBug(id);
                if (foundBug) {
                    Position pos = foundBug->getPosition();
                    Direction dir = foundBug->getDirection();
                    std::string directionStr;
                    switch (dir) {
                        case Direction::North: directionStr = "North"; break;
                        case Direction::East: directionStr = "East"; break;
                        case Direction::South: directionStr = "South"; break;
                        case Direction::West: directionStr = "West"; break;
                    }
                    std::string status = foundBug->isAlive() ? "Alive" : "Dead";
                    std::cout << "Found Bug: ID " << foundBug->getId() << "\n"
                              << "Position: (" << pos.x << ", " << pos.y << ")\n"
                              << "Direction: " << directionStr << "\n"
                              << "Size: " << foundBug->getSize() << "\n"
                              << "Status: " << status << "\n";
                } else {
                    std::cout << "Bug not found or dead.\n";
                }
                break;
            }
            case 4:
                board.tap();
                break;
            case 5:
                board.displayLifeHistory();
                break;
            case 6:
                board.displayCells();
                break;
            case 7:
                board.runSimulation();
                break;
            case 8:
                board.saveLifeHistoryToFile();
                running = false;
                std::cout << "Board Complete... exiting... \n";
                break;
            default:
                std::cout << "Error: Bad Input\n";
                break;
        }
    }
}