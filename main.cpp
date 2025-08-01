#include <iostream>
using namespace std;

void CustomerApp();
void BugApp();

int main() {
    int choice;
    do {
        cout << "\nMain Menu\n";
        cout << "1. Customer Management\n";
        cout << "2. Bug Simulation\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                CustomerApp();
                break;
            case 2:
                BugApp();
                break;
            case 0:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}