// ============================================================
// CHARACTER CLASS
// ============================================================
class Character {
private:
    string name;
    int health;
    int maxHealth;
    int attackPower;
    int defense;
public:
    string charClass;
    vector<Item*> inventory;

    Character(string n, int hp, int atk, int def, string cls)
        : name(n), health(hp), maxHealth(hp),
          attackPower(atk), defense(def), charClass(cls) {}

    // Getters
    string getName()  const { return name; }
    int getHealth()   const { return health; }
    int getMaxHP()    const { return maxHealth; }
    int getAttack()   const { return attackPower; }
    int getDefense()  const { return defense; }
    bool isAlive()    const { return health > 0; }

    // Setters
    void setHealth(int h)  { health = (h < 0) ? 0 : h; }
    void setAttack(int v)  { attackPower = v; }
    void setDefense(int v) { defense = v; }
    //Methods
    void heal(int amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth;
    }

    virtual void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
        cout << name << " has " << health << " HP remaining.\n";
    }

    void basicAttack(Character* target) {
        int dmg = attackPower - target->getDefense();
        if (dmg < 1) dmg = 1;
        cout << name << " attacks " << target->getName()
             << " for " << dmg << " damage!\n";
        target->takeDamage(dmg);
    }
    virtual void specialAttack(Character* target) = 0;

    void addItem(Item* item) { inventory.push_back(item); }

    void removeItem(int index) {
        delete inventory[index];
        inventory.erase(inventory.begin() + index);
    }

    void showInventory() {
        cout << name << "'s Inventory:\n";
        if (inventory.empty()) { cout << "  (empty)\n"; return; }
        for (int i = 0; i < (int)inventory.size(); i++) {
            cout << "  " << (i+1) << ". ";
            inventory[i]->showInfo();
            cout << "\n";
        }
    }
    void showStatus() const {
        cout << "[ " << name << " | " << charClass << " ]"
             << "  HP: " << health << "/" << maxHealth
             << "  ATK: " << attackPower
             << "  DEF: " << defense << "\n";
    }

    virtual ~Character() {
        for (Item* item : inventory) delete item;
    }
};



// ============================================================
// CHARACTER SUBCLASSES
// ============================================================

// Warrior — Power Strike
class Warrior : public Character {
public:
    Warrior(string name) : Character(name, 120, 20, 10, "Warrior") {}

    void specialAttack(Character* target) override {
        int dmg = (getAttack() * 2) - target->getDefense();
        if (dmg < 1) dmg = 1;
        cout << getName() << " uses [POWER STRIKE] on "
             << target->getName() << " for " << dmg << " damage\n";
        target->takeDamage(dmg);
    }
};

// Mage — Expecto Patronum
class Mage : public Character {
private:
    bool isProtected;
public:
    Mage(string name) : Character(name, 80, 25, 5, "Mage"), isProtected(false) {}

    void takeDamage(int damage) override {
        if (isProtected) {
            cout << "Expecto Patronum shield absorbs the attack! "
                 << getName() << " takes NO damage\n";
            isProtected = false;
        } else {
            Character::takeDamage(damage);
        }
    }

    void specialAttack(Character* target) override {
        int dmg = (getAttack() / 4) - target->getDefense();
        if (dmg < 1) dmg = 1;
        cout << getName() << " casts [EXPECTO PATRONUM]\n";
        cout << "  -> " << target->getName() << " takes " << dmg << " damage\n";
        cout << "  -> " << getName() << " is now shielded from the next attack\n";
        target->takeDamage(dmg);
        isProtected = true;
    }
};

// Archer — Double Shot
class Archer : public Character {
public:
    Archer(string name) : Character(name, 95, 18, 7, "Archer") {}

    void specialAttack(Character* target) override {
        cout << getName() << " uses [DOUBLE SHOT]!\n";
        for (int shot = 1; shot <= 2; shot++) {
            if (!target->isAlive()) {
                cout << target->getName() << " is already defeated!\n";
                break;
            }
            int dmg = getAttack() - target->getDefense();
            if (dmg < 1) dmg = 1;
            cout << "  -> Shot " << shot << ": " << target->getName()
                 << " takes " << dmg << " damage!\n";
            target->takeDamage(dmg);
        }
    }
// Monster — Fire Breath
class Monster : public Character {
public:
    Monster(string name) : Character(name, 150, 22, 8, "Monster") {}

    void specialAttack(Character* target) override {
        int dmg = (getAttack() - target->getDefense()) + 15;
        if (dmg < 1) dmg = 1;
        cout << getName() << " unleashes [FIRE BREATH] on " << target->getName()
             << " for " << dmg << " damage (+15 fire bonus)\n";
        target->takeDamage(dmg);
    }
};

};
