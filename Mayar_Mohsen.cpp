#include <iostream>
#include <vector>
#include <string>
using namespace std;

// ============================================================
// ITEM CLASS
// ============================================================
class Item
{
protected:
    string name;
    string type;

public:
    Item(string n, string t) : name(n), type(t) {}
    string getName() { return name; }
    string getType() { return type; }
    virtual void use(class Character *target) = 0;
    virtual void showInfo() { cout << "[" << type << "] " << name; }
    virtual ~Item() {}
};
