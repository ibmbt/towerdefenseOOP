#define _CRT_SECURE_NO_WARNINGS
#include "DynamicArray.h"
#include "raylib.h"
#include "Position.h"
#include "Cell.h"
#include "Enemy.h"
#include "Tower.h"
#include "Map.h"
#include "Player.h"
#include "Game.h"
#include <string>
#include <cmath>

// === Constants ===
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int CELL_SIZE = 64;
const int UI_HEIGHT = 150;

// === Global Variable ===
char SELECTED_TOWER = '\0';


// === Enemy Types ===
class Clicker : public Enemy {
public:
    Clicker(const DynamicArray<Position>& p) : Enemy('C', 50, 3, 15, p) {}
};

class Bloater : public Enemy {
public:
    Bloater(const DynamicArray<Position>& p) : Enemy('B', 200, 1, 30, p) {}
};

// === Tower Types ===
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


int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "The Last of Us Tower Defense");
    SetTargetFPS(60);

    //Game game;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        //game.handleInput();
        //game.update(deltaTime);
        //game.draw();

        //if (game.shouldClose()) break;
    }

    CloseWindow();
    return 0;
}