#include <fstream>
#include <sstream>
#include <cstddef>
#include "Customer.cpp"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void loadCustomers(vector<Customer>& customers, const string& filename) {
    ifstream infile(filename);
    if (!infile) return;
    string line;
    while (getline(infile, line)) {
        size_t pos = 0;
        vector<string> fields;
        // Split line by ';'
        while ((pos = line.find(';')) != string::npos) {
            fields.push_back(line.substr(0, pos));
            line.erase(0, pos + 1);
        }
        fields.push_back(line); // last field

        if (fields.size() < 5) continue; // not enough data check

        int id = stoi(fields[0]);
        string title = fields[1];
        string name = fields[2];
        string type = fields[3];
        int numPurchases = stoi(fields[4]);

        int* purchases = nullptr;
        if (numPurchases > 0 && fields.size() >= 5 + numPurchases) {
            purchases = new int[numPurchases];
            for (int i = 0; i < numPurchases; ++i) {
                purchases[i] = stoi(fields[5 + i]);
            }
        }

        Customer c(id, title, name, numPurchases, purchases, type);
        customers.push_back(c);
        delete[] purchases;
    }
    infile.close();
}

// Store customers to file (semicolon-separated format)
void storeCustomers(const vector<Customer>& customers, const string& filename) {
    ofstream outfile(filename);
    for (const auto& c : customers) {
        outfile << c.getCustomerId() << ";"
                << c.getTitle() << ";"
                << c.getName() << ";"
                << c.getType() << ";"
                << c.getNumberOfPurchases() << ";";
        int* purchases = c.getPurchases();
        for (int i = 0; i < c.getNumberOfPurchases(); ++i) {
            outfile << purchases[i];
            if (i < c.getNumberOfPurchases() - 1)
                outfile << ";";
        }
        outfile << "\n";
    }
    outfile.close();
}

// Find customer by ID
const Customer* findCustomerById(const vector<Customer>& customers, int id) {
    for (const auto& c : customers) {
        if (c.getCustomerId() == id) {
            return &c;
        }
    }
    return nullptr;
}

// Find customer by purchase number
const Customer* findCustomerByPurchaseNumber(const vector<Customer>& customers, int purchaseNumber) {
    for (const auto& c : customers) {
        int* purchases = c.getPurchases();
        for (int i = 0; i < c.getNumberOfPurchases(); ++i) {
            if (purchases[i] == purchaseNumber) {
                return &c;
            }
        }
    }
    return nullptr;
}

void addCustomer(vector<Customer>& customers) {
    int id, numPurchases;
    string title, name, type;
    cout << "Enter Customer ID: ";
    cin >> id;
    cout << "Enter Title: ";
    cin >> title;
    cout << "Enter Name: ";
    cin >> name;
    cout << "Enter Number of Purchases: ";
    cin >> numPurchases;
    int* purchases = nullptr;
    if (numPurchases > 0) {
        purchases = new int[numPurchases];
        cout << "Enter Purchases (integers): ";
        for (int i = 0; i < numPurchases; ++i) {
            cin >> purchases[i];
        }
    }
    cout << "Enter Type (New/Regular): ";
    cin >> type;
    Customer c(id, title, name, numPurchases, purchases, type);
    customers.push_back(c);
    delete[] purchases;
    cout << "Customer added!\n";
}

void displayAllCustomers(const vector<Customer>& customers) {
    if (customers.empty()) {
        cout << "No customers to display.\n";
        return;
    }
    // Copy and sort by name
    vector<Customer> sortedCustomers = customers;
    sort(sortedCustomers.begin(), sortedCustomers.end(), [](const Customer& a, const Customer& b) {
        return a.getName() < b.getName();
    });
    cout << "\nAll Customers (sorted by name):\n";
    for (const auto& c : sortedCustomers) {
        cout << c << endl;
    }
}

void displayCustomersByType(const vector<Customer>& customers) {
    if (customers.empty()) {
        cout << "No customers to display.\n";
        return;
    }
    string type;
    cout << "Enter Type to filter by (New/Regular): ";
    cin >> type;
    bool found = false;
    for (const auto& c : customers) {
        if (c.getType() == type) {
            cout << c << endl;
            found = true;
        }
    }
    if (!found) cout << "No customers of type '" << type << "' found.\n";
}

int main() {
    vector<Customer> customers;
    const string filename = "customers.txt";
    loadCustomers(customers, filename);
    int choice;
    do {
        cout << "\nCustomer Management System\n";
        cout << "1. Add Customer\n";
        cout << "2. Display All Customers (by name)\n";
        cout << "3. Display Customers by Type\n";
        cout << "4. Find Customer by ID\n";
        cout << "5. Find Customer by Purchase Number\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                addCustomer(customers);
                break;
            case 2:
                displayAllCustomers(customers);
                break;
            case 3:
                displayCustomersByType(customers);
                break;
            case 4: {
                int id;
                cout << "Enter Customer ID to search: ";
                cin >> id;
                const Customer* found = findCustomerById(customers, id);
                if (found) {
                    cout << "Customer found at address: " << found << endl;
                    cout << *found << endl;
                } else {
                    cout << "Customer not found.\n";
                }
                break;
            }
            case 5: {
                int purchaseNum;
                cout << "Enter Purchase Number to search: ";
                cin >> purchaseNum;
                const Customer* found = findCustomerByPurchaseNumber(customers, purchaseNum);
                if (found) {
                    cout << "Customer found at address: " << found << endl;
                    cout << *found << endl;
                } else {
                    cout << "No customer found with that purchase number.\n";
                }
                break;
            }
            case 0:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);
    storeCustomers(customers, filename);
    return 0;
}
