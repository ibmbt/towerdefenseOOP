#ifndef JOELTOWER_H
#define JOELTOWER_H
#include "Tower.h"
#include "Position.h"
#include "raylib.h"

class JoelTower : public Tower {

    bool molotovUnlocked;
    bool rifleUnlocked;

public:
    JoelTower(Position p) : Tower('J', 25, 2, p, 50, 1.0f),
        molotovUnlocked(false), rifleUnlocked(false) {
        maxLevel = 2;
        upgrade1Name = "Molotov Upgrade";
        upgrade2Name = "Rifle Upgrade";
    }

    void upgradePath1() override {
        if (level >= maxLevel) return;

        damage += 15;
        range += 0.5f;
        cooldown *= 1.2f;
        molotovUnlocked = true;
        level++;
    }

    void upgradePath2() override {
        if (level >= maxLevel) return;

        damage += 10;
        range += 1.5f;
        cooldown *= 0.8f;
        rifleUnlocked = true;
        level++;
    }

    void displayInfo(Vector2 mousePos) const override {
        if (showRange) {
            drawBaseInfoPanel(mousePos, "Joel", molotovUnlocked, rifleUnlocked);
        }
    }
};


#endif