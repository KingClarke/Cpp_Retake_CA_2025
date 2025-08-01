#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Bug.h"
#include "Crawler.h"
#include "Hopper.h"

class Board {
private:
    std::vector<Bug*> bugs;

public:
    Board();  // Constructor
    ~Board(); // Destructor

    void loadBugs(const std::string& filename);  // Load bugs from a file
    Bug* findBug(int id);  // Find a specific bug by ID
    void tap();  // Move bugs, make them fight and eat each other
    void runSimulation();  // Run the simulation for bug life
    void displayBugs() const;  // Display information on all bugs
    void displayCells() const;  // Show all cells with their bugs
    void saveLifeHistoryToFile() const;  // Save the bugs' life history to a file
    void displayLifeHistory() const;  // Display the life history of all bugs

private:
    void fightAndEat();  // Let bugs fight and eat each other
    void resolveFight(std::vector<Bug*>& bugsInCell);  // Resolve a fight between multiple bugs
};