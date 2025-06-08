#ifndef ELLIETOWER_H
#define ELLIETOWER_H
#include "Tower.h"
#include "Position.h"
#include "raylib.h"

class EllieTower : public Tower {

    bool bowUnlocked;
    bool stealthUnlocked;

public:

    EllieTower(Position p) : Tower('E', 10, 3, p, 30, 0.5f),
        bowUnlocked(false), stealthUnlocked(false) {
        maxLevel = 2;
        upgrade1Name = "Bow Upgrade";
        upgrade2Name = "Stealth Upgrade";
    }

    void upgradePath1() override {
        if (level >= maxLevel) return;

        damage += 8;
        range += 1.0f;
        bowUnlocked = true;
        level++;
    }

    void upgradePath2() override {
        if (level >= maxLevel) return;

        cooldown *= 0.7f;
        stealthUnlocked = true;
        level++;
    }

    void attack(DynamicArray<Enemy*>& enemies) override {
        if (stealthUnlocked) {

            if (rand() % 100 < 30) {
                Tower::attack(enemies);
            }
        }
        Tower::attack(enemies);
    }

    void displayInfo(Vector2 mousePos) const override {
        if (showRange) {
            drawBaseInfoPanel(mousePos, "Ellie", bowUnlocked, stealthUnlocked);
        }
    }

};


#endif