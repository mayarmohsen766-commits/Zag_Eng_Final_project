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
