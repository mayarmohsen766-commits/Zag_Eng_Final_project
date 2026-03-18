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

// ============================================================
// CHARACTER SUBCLASSES
// ============================================================

// Warrior — Power Strike
class Warrior : public Character
{
public:
    Warrior(string name) : Character(name, 120, 20, 10, "Warrior") {}

    void specialAttack(Character *target) override
    {
        int dmg = (getAttack() * 2) - target->getDefense();
        if (dmg < 1)
            dmg = 1;
        cout << getName() << " uses [POWER STRIKE] on "
             << target->getName() << " for " << dmg << " damage\n";
        target->takeDamage(dmg);
    }
};

// Mage — Expecto Patronum
class Mage : public Character
{
private:
    bool isProtected;

public:
    Mage(string name) : Character(name, 80, 25, 5, "Mage"), isProtected(false) {}

    void takeDamage(int damage) override
    {
        if (isProtected)
        {
            cout << "Expecto Patronum shield absorbs the attack! "
                 << getName() << " takes NO damage\n";
            isProtected = false;
        }
        else
        {
            Character::takeDamage(damage);
        }
    }

    void specialAttack(Character *target) override
    {
        int dmg = (getAttack() / 4) - target->getDefense();
        if (dmg < 1)
            dmg = 1;
        cout << getName() << " casts [EXPECTO PATRONUM]\n";
        cout << "  -> " << target->getName() << " takes " << dmg << " damage\n";
        cout << "  -> " << getName() << " is now shielded from the next attack\n";
        target->takeDamage(dmg);
        isProtected = true;
    }
};

// Archer — Double Shot
class Archer : public Character
{
public:
    Archer(string name) : Character(name, 95, 18, 7, "Archer") {}

    void specialAttack(Character *target) override
    {
        cout << getName() << " uses [DOUBLE SHOT]!\n";
        for (int shot = 1; shot <= 2; shot++)
        {
            if (!target->isAlive())
            {
                cout << target->getName() << " is already defeated!\n";
                break;
            }
            int dmg = getAttack() - target->getDefense();
            if (dmg < 1)
                dmg = 1;
            cout << "  -> Shot " << shot << ": " << target->getName()
                 << " takes " << dmg << " damage!\n";
            target->takeDamage(dmg);
        }
    }
};

// Monster — Fire Breath
class Monster : public Character
{
public:
    Monster(string name) : Character(name, 150, 22, 8, "Monster") {}

    void specialAttack(Character *target) override
    {
        int dmg = (getAttack() - target->getDefense()) + 15;
        if (dmg < 1)
            dmg = 1;
        cout << getName() << " unleashes [FIRE BREATH] on " << target->getName()
             << " for " << dmg << " damage (+15 fire bonus)\n";
        target->takeDamage(dmg);
    }
};

// ============================================================
// BATTLE SYSTEM
// ============================================================
class BattleSystem
{
    Character *p1;
    Character *p2;

public:
    BattleSystem(Character *a, Character *b) : p1(a), p2(b) {}
    bool isOver() { return !p1->isAlive() || !p2->isAlive(); }
    Character *getWinner()
    {
        if (!p1->isAlive())
            return p2;
        if (!p2->isAlive())
            return p1;
        return nullptr;
    }
};

// ============================================================
// HELPERS ITEM
// ============================================================
void printLine() { cout << "===========================================\n"; }

void giveStartingItems(Character *c)
{
    if (c->charClass == "Warrior")
    {
        c->addItem(new Potion("Health Potion", 40));
        c->addItem(new Weapon("Iron Sword", 8));
        c->addItem(new Shield("Iron Shield", 5));
    }
    else if (c->charClass == "Mage")
    {
        c->addItem(new Potion("Mana Potion", 35));
        c->addItem(new Weapon("Magic Staff", 10));
        c->addItem(new Potion("Elixir", 25));
    }
    else if (c->charClass == "Archer")
    {
        c->addItem(new Potion("Swift Potion", 30));
        c->addItem(new Weapon("Elven Bow", 7));
        c->addItem(new Shield("Leather Shield", 4));
    }
    else if (c->charClass == "Monster")
    {
        c->addItem(new Potion("Dark Potion", 50));
        c->addItem(new Weapon("Dragon Claw", 12));
    }
}

// Create character by choice
Character *createCharacter(int choice)
{
    if (choice == 1)
        return new Warrior("Warrior");
    if (choice == 2)
        return new Mage("Mage");
    if (choice == 3)
        return new Archer("Archer");
    if (choice == 4)
        return new Monster("Monster");
    return nullptr;
}

// ============================================================
// MAIN
// ============================================================
int main()
{

    cout << "\n";
    printLine();
    cout << "        *** RPG COMBAT SYSTEM ***\n";
    printLine();

    // ── Fighter 1 picks character ──
    cout << "\n=== FIGHTER 1: Choose Your Character ===\n";
    cout << "1. Warrior  (HP:120  ATK:20  DEF:10)  Special: Power Strike :damage = attackPower * 2\n";
    cout << "2. Mage     (HP:80   ATK:25  DEF:5 )  Special: Expecto Patronum :light damage + shield for next hit\n";
    cout << "3. Archer   (HP:95   ATK:18  DEF:7 )  Special: Double Shot:2 hits in a row\n";
    cout << "4. Monster  (HP:150  ATK:22  DEF:8 )  Special: Fire Breath: normal damage + 15 bonus\n";
    cout << "Choice (1-4): ";
    int c1;
    cin >> c1;
    while (c1 < 1 || c1 > 4)
    {
        cout << "Invalid! Enter 1-4: ";
        cin >> c1;
    }
    Character *p1 = createCharacter(c1);
    giveStartingItems(p1);
    cout << ">> Fighter 1 chose: " << p1->charClass << "!\n";

    // ── Fighter 2 picks character ──
    cout << "\n=== FIGHTER 2: Choose Your Character ===\n";
    cout << "1. Warrior  (HP:120  ATK:20  DEF:10)  Special: Power Strike :damage = attackPower * 2\n";
    cout << "2. Mage     (HP:80   ATK:25  DEF:5 )  Special: Expecto Patronum :light damage + shield for next hit \n";
    cout << "3. Archer   (HP:95   ATK:18  DEF:7 )  Special: Double Shot :2 hits in a row\n";
    cout << "4. Monster  (HP:150  ATK:22  DEF:8 )  Special: Fire Breath: normal damage + 15 bonus\n";
    cout << "Choice (1-4): ";
    int c2;
    cin >> c2;
    while (c2 < 1 || c2 > 4)
    {
        cout << "Invalid! Enter 1-4: ";
        cin >> c2;
    }
    Character *p2 = createCharacter(c2);
    giveStartingItems(p2);
    cout << ">> Fighter 2 chose: " << p2->charClass << "!\n";

    // ── Show starting inventories ──
    cout << "\n";
    printLine();
    cout << "          STARTING INVENTORIES\n";
    printLine();
    cout << "Fighter 1 - ";
    p1->showInventory();
    cout << "\nFighter 2 - ";
    p2->showInventory();

    // ── Battle loop ──
    BattleSystem battle(p1, p2);
    Character *attacker = p1;
    Character *defender = p2;
    // Track which fighter number is attacking for display
    int attackerNum = 1;
    int round = 1;

    while (!battle.isOver())
    {

        cout << "\n";
        printLine();
        cout << "  ROUND " << round << "\n";
        printLine();

        // Show both HP
        cout << "\n";
        cout << "Fighter 1 - ";
        p1->showStatus();
        cout << "Fighter 2 - ";
        p2->showStatus();
        cout << "\n";

        // Show whose turn
        cout << "--- Fighter " << attackerNum
             << " (" << attacker->getName() << ")'s turn ---\n";
        cout << "1. Basic Attack\n";
        cout << "2. Special Attack\n";
        cout << "3. Use Item\n";
        cout << "Your choice: ";
        int action;
        cin >> action;
        while (action < 1 || action > 3)
        {
            cout << "Invalid! Enter 1, 2, or 3: ";
            cin >> action;
        }

        cout << "\n";

        if (action == 1)
        {
            attacker->basicAttack(defender);
        }
        else if (action == 2)
        {
            attacker->specialAttack(defender);
        }
        else
        {
            // Use Item
            attacker->showInventory();
            if (attacker->inventory.empty())
            {
                cout << "No items left! Turn skipped.\n";
            }
            else
            {
                cout << "Enter item number: ";
                int idx;
                cin >> idx;
                if (idx < 1 || idx > (int)attacker->inventory.size())
                {
                    cout << "[ERROR] Invalid number! Turn skipped.\n";
                }
                else
                {
                    attacker->inventory[idx - 1]->use(attacker);
                    attacker->removeItem(idx - 1);
                }
            }
        }

        // Show defender HP after action
        if (!battle.isOver())
        {
            int defNum = (attackerNum == 1) ? 2 : 1;
            cout << "\n  Fighter " << defNum << " (" << defender->getName() << ")"
                 << " HP: " << defender->getHealth()
                 << "/" << defender->getMaxHP() << "\n";
        }

        if (battle.isOver())
            break;

        // Swap turns
        if (attacker == p1)
        {
            attacker = p2;
            defender = p1;
            attackerNum = 2;
        }
        else
        {
            attacker = p1;
            defender = p2;
            attackerNum = 1;
            round++;
        }
    }

    // ── Winner ──
    cout << "\n";
    printLine();
    cout << "           BATTLE OVER!\n";
    printLine();
    cout << "\nFinal Status:\n";
    cout << "Fighter 1 - ";
    p1->showStatus();
    cout << "Fighter 2 - ";
    p2->showStatus();
    cout << "\n";

    Character *winner = battle.getWinner();
    if (winner)
    {
        int winNum = (winner == p1) ? 1 : 2;
        cout << "*** Fighter " << winNum
             << " (" << winner->getName() << ") WINS! ***\n";
    }
    printLine();

    delete p1;
    delete p2;
    return 0;
}
