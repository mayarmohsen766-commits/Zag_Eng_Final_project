# Zag_Eng_Final_project
# RPG Combat & Inventory System

A turn-based RPG battle game built in C++ as an OOP assignment.  
Two fighters choose their characters and fight round by round until one reaches 0 HP.

## How to Play

1. **Fighter 1** picks a character (1–4)
2. **Fighter 2** picks a character (1–4)
3. Each round, the active fighter chooses:
   - `1` Basic Attack
   - `2` Special Attack
   - `3` Use Item from inventory
4. Turns alternate until one fighter's HP hits 0

## Characters

| Character | HP  | ATK | DEF | Special Attack         | Effect                        |
|-----------|-----|-----|-----|------------------------|-------------------------------|
| Warrior   | 120 | 20  | 10  | Power Strike           | ATK × 2                       |
| Mage      | 80  | 25  | 5   | Expecto Patronum       | Light damage + absorbs next hit |
| Archer    | 95  | 18  | 7   | Double Shot            | 2 hits in a row               |
| Monster   | 150 | 22  | 8   | Fire Breath            | Normal damage + 15 bonus      |

## Items (Starting Inventory)

| Character | Items                                        |
|-----------|----------------------------------------------|
| Warrior   | Health Potion (+40 HP), Iron Sword (+8 ATK), Iron Shield (+5 DEF) |
| Mage      | Mana Potion (+35 HP), Magic Staff (+10 ATK), Elixir (+25 HP)      |
| Archer    | Swift Potion (+30 HP), Elven Bow (+7 ATK), Leather Shield (+4 DEF)|
| Monster   | Dark Potion (+50 HP), Dragon Claw (+12 ATK)                       |

## OOP Concepts Applied

- **Abstraction** — `Character` and `Item` are abstract base classes
- **Inheritance** — `Warrior`, `Mage`, `Archer`, `Monster` extend `Character`; `Potion`, `Weapon`, `Shield` extend `Item`
- **Polymorphism** — each character overrides `specialAttack()` with unique behavior
- **Encapsulation** — character stats are `private`, accessed through getters/setters
- **Composition** — each `Character` contains a `vector<Item*>` as its inventory
