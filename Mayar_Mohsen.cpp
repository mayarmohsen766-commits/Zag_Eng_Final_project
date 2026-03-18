/*
 * ============================================================
 *   RPG Combat & Inventory System
 * ============================================================
 */

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

// ============================================================
// CHARACTER CLASS
// ============================================================
class Character
{
private:
    string name;
    int health;
    int maxHealth;
    int attackPower;
    int defense;

public:
    string charClass;
    vector<Item *> inventory;

    Character(string n, int hp, int atk, int def, string cls)
        : name(n), health(hp), maxHealth(hp),
          attackPower(atk), defense(def), charClass(cls) {}

    // Getters
    string getName() const { return name; }
    int getHealth() const { return health; }
    int getMaxHP() const { return maxHealth; }
    int getAttack() const { return attackPower; }
    int getDefense() const { return defense; }
    bool isAlive() const { return health > 0; }

    // Setters
    void setHealth(int h) { health = (h < 0) ? 0 : h; }
    void setAttack(int v) { attackPower = v; }
    void setDefense(int v) { defense = v; }
    // Methods
    void heal(int amount)
    {
        health += amount;
        if (health > maxHealth)
            health = maxHealth;
    }

    virtual void takeDamage(int damage)
    {
        health -= damage;
        if (health < 0)
            health = 0;
        cout << name << " has " << health << " HP remaining.\n";
    }

    void basicAttack(Character *target)
    {
        int dmg = attackPower - target->getDefense();
        if (dmg < 1)
            dmg = 1;
        cout << name << " attacks " << target->getName()
             << " for " << dmg << " damage!\n";
        target->takeDamage(dmg);
    }
    virtual void specialAttack(Character *target) = 0;

    void addItem(Item *item) { inventory.push_back(item); }

    void removeItem(int index)
    {
        delete inventory[index];
        inventory.erase(inventory.begin() + index);
    }

    void showInventory()
    {
        cout << name << "'s Inventory:\n";
        if (inventory.empty())
        {
            cout << "  (empty)\n";
            return;
        }
        for (int i = 0; i < (int)inventory.size(); i++)
        {
            cout << "  " << (i + 1) << ". ";
            inventory[i]->showInfo();
            cout << "\n";
        }
    }
    void showStatus() const
    {
        cout << "[ " << name << " | " << charClass << " ]"
             << "  HP: " << health << "/" << maxHealth
             << "  ATK: " << attackPower
             << "  DEF: " << defense << "\n";
    }

    virtual ~Character()
    {
        for (Item *item : inventory)
            delete item;
    }
};

// ============================================================
// ITEM SUBCLASSES
// ============================================================

class Potion : public Item
{
    int healAmount;

public:
    Potion(string n, int heal) : Item(n, "Potion"), healAmount(heal) {}
    void use(Character *target) override
    {
        cout << target->getName() << " uses " << name
             << " and restores " << healAmount << " HP\n";
        target->heal(healAmount);
        cout << target->getName() << " now has " << target->getHealth() << " HP\n";
    }
    void showInfo() override
    {
        cout << "[Potion] " << name << " (Heals " << healAmount << " HP)";
    }
};

class Weapon : public Item
{
    int attackBonus;

public:
    Weapon(string n, int bonus) : Item(n, "Weapon"), attackBonus(bonus) {}
    void use(Character *target) override
    {
        cout << target->getName() << " equips " << name
             << "! (+" << attackBonus << " ATK)\n";
        target->setAttack(target->getAttack() + attackBonus);
    }
    void showInfo() override
    {
        cout << "[Weapon] " << name << " (+" << attackBonus << " ATK)";
    }
};

class Shield : public Item
{
    int defenseBonus;

public:
    Shield(string n, int bonus) : Item(n, "Shield"), defenseBonus(bonus) {}
    void use(Character *target) override
    {
        cout << target->getName() << " equips " << name
             << "! (+" << defenseBonus << " DEF)\n";
        target->setDefense(target->getDefense() + defenseBonus);
    }
    void showInfo() override
    {
        cout << "[Shield] " << name << " (+" << defenseBonus << " DEF)";
    }
};
