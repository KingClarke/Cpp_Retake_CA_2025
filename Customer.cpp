
#include <string>
#include <iostream>
using namespace std;

class Customer {
private:
    int customerId;
    string title;
    string name;
    int numberOfPurchases;
    int* purchases;
    string type;

public:
    // Default constructor
    Customer()
        : customerId(0), title(""), name(""), numberOfPurchases(0), purchases(nullptr), type("") {}

    // Parameterized constructor
    Customer(int id, const string& t, const string& n, int numPurchases, const int* p, const string& ty)
        : customerId(id), title(t), name(n), numberOfPurchases(numPurchases), purchases(nullptr), type(ty) {
        if (numPurchases > 0 && p != nullptr) {
            purchases = new int[numPurchases];
            for (int i = 0; i < numPurchases; ++i) {
                purchases[i] = p[i];
            }
        }
    }

    // Getters
    int getCustomerId() const { return customerId; }
    string getTitle() const { return title; }
    string getName() const { return name; }
    int getNumberOfPurchases() const { return numberOfPurchases; }
    int* getPurchases() const { return purchases; }
    string getType() const { return type; }

    // Setters
    void setCustomerId(int id) { customerId = id; }
    void setTitle(const string& t) { title = t; }
    void setName(const string& n) { name = n; }
    void setNumberOfPurchases(int num) { numberOfPurchases = num; }
    void setPurchases(const int* p, int num) {
        if (purchases) delete[] purchases;
        if (num > 0 && p != nullptr) {
            purchases = new int[num];
            for (int i = 0; i < num; ++i) {
                purchases[i] = p[i];
            }
        } else {
            purchases = nullptr;
        }
        numberOfPurchases = num;
    }
    // Copy constructor
    Customer(const Customer& other)
        : customerId(other.customerId), title(other.title), name(other.name), numberOfPurchases(other.numberOfPurchases), purchases(nullptr), type(other.type) {
        if (numberOfPurchases > 0 && other.purchases != nullptr) {
            purchases = new int[numberOfPurchases];
            for (int i = 0; i < numberOfPurchases; ++i) {
                purchases[i] = other.purchases[i];
            }
        }
    }

    // Assignment operator
    Customer& operator=(const Customer& other) {
        if (this != &other) {
            customerId = other.customerId;
            title = other.title;
            name = other.name;
            type = other.type;
            setPurchases(other.purchases, other.numberOfPurchases);
            numberOfPurchases = other.numberOfPurchases;
        }
        return *this;
    }

    // Destructor
    ~Customer() {
        delete[] purchases;
    }

    // Overloaded operators == and !=
    bool operator==(const Customer& other) const {
        if (customerId != other.customerId || title != other.title || name != other.name ||
            numberOfPurchases != other.numberOfPurchases || type != other.type)
            return false;
        for (int i = 0; i < numberOfPurchases; ++i) {
            if (purchases[i] != other.purchases[i])
                return false;
        }
        return true;
    }

    bool operator!=(const Customer& other) const {
        return !(*this == other);
    }

    // Overloaded stream insertion and extraction operators // ### check
    friend ostream& operator<<(ostream& os, const Customer& c) {
        os << c.customerId << " " << c.title << " " << c.name << " " << c.numberOfPurchases << " " << c.type;
        for (int i = 0; i < c.numberOfPurchases; ++i) {
            os << " " << static_cast<int>(c.purchases[i]);
        }
        return os;
    }

    friend istream& operator>>(istream& is, Customer& c) {
        is >> c.customerId >> c.title >> c.name >> c.numberOfPurchases >> c.type;
        if (c.purchases) delete[] c.purchases;
        if (c.numberOfPurchases > 0) {
            c.purchases = new int[c.numberOfPurchases];
            for (int i = 0; i < c.numberOfPurchases; ++i) {
                int temp;
                is >> temp;
                c.purchases[i] = temp;
            }
        } else {
            c.purchases = nullptr;
        }
        return is;
    }
    void setType(const string& ty) { type = ty; }
};
