#ifndef TOWER_H
#define TOWER_H
#include "Globals.h"
#include "Position.h"
#include "dynamicArray.h"
#include "myString.h"
#include "Enemy.h"
using namespace std;

class Tower;

class Player {
private:
    DynamicArray<Tower*> towers;
    int lives;
    int coins;

public:
    Player(int initialLives, int initialCoins) : lives(initialLives), coins(initialCoins) {}

    ~Player() {
        for (int i = 0; i < towers.getSize(); i++) {
            delete towers[i];
        }
    }

    bool canAfford(int cost) const { return coins >= cost; }
    void deductCoins(int amount) { coins -= amount; }
    void addCoins(int amount) { coins += amount; }
    void addTower(Tower* tower) { towers.push(tower); }
    void loseLife() { lives--; }

    void setPlayer(int _lives, int _coins) {
        lives = _lives;
        coins = _coins;
    }

    int getLives() const { return lives; }
    int getCoins() const { return coins; }
    const DynamicArray<Tower*>& getTowers() const { return towers; }

    void resetPlayer() {
        towers.clear();
    }

    bool sellTower(Tower* towerToSell) {
        for (int i = 0; i < towers.getSize(); i++) {
            if (towers[i] == towerToSell) {
                towers.remove(i);
                return true;
            }
        }
        return false;
    }
};



class Tower {
protected:
    char symbol;
    int damage;
    int range;
    Position pos;
    int cost;
    float cooldown;
    float attackTimer;
    bool showRange = false;

    int level;
    int maxLevel;
    String upgrade1Name;
    String upgrade2Name;

    void drawBaseInfoPanel(Vector2 mousePos, const char* towerName, bool u1Unlocked, bool u2Unlocked) const {
        float panelWidth = 250;
        float panelHeight = (level <= maxLevel) ? 150 : 100;
        float panelX = min(mousePos.x + 15, SCREEN_WIDTH - panelWidth - 5);
        float panelY = min(mousePos.y - panelHeight - 10, SCREEN_HEIGHT - panelHeight - 5);

        DrawRectangleRounded({ panelX, panelY, panelWidth, panelHeight }, 0.1f, 8, Color{ 40, 40, 60, 230 });
        DrawRectangleRoundedLines({ panelX, panelY, panelWidth, panelHeight }, 0.1f, 8, PURPLE);

        DrawText(TextFormat("%s Tower", towerName), panelX + 10, panelY + 10, 20, WHITE);
        DrawText(TextFormat("Damage: %d", damage), panelX + 10, panelY + 35, 18, LIGHTGRAY);
        DrawText(TextFormat("Range: %d", range), panelX + 10, panelY + 55, 18, LIGHTGRAY);

        DrawText(TextFormat("[S] Sell: %dg", (int)(getCost() * 0.5f)), panelX + 10, panelY + 75, 18, RED);

        if (level < maxLevel) {
            DrawText(TextFormat("1. %s (%dg)", upgrade1Name.c_str(), getUpgradeCost(1)),
                panelX + 10, panelY + 100, 18, u1Unlocked ? GRAY : RED);
            DrawText(TextFormat("2. %s (%dg)", upgrade2Name.c_str(), getUpgradeCost(2)),
                panelX + 10, panelY + 120, 18, u2Unlocked ? GRAY : RED);
        }
        if (level == maxLevel) {
            DrawText("Fully Upgraded", panelX + 10, panelY + 105, 18, RED);
        }
    }


public:
    Tower(char sym, int dmg, int rng, Position p, int cst, float cd)
        : symbol(sym), damage(dmg), range(rng), pos(p), cost(cst), cooldown(cd), attackTimer(0.0f), level(0), maxLevel(2) {
    }

    virtual ~Tower() {}

    virtual void update(float deltaTime, DynamicArray<Enemy*>& enemies) {
        attackTimer += deltaTime;
        if (attackTimer >= cooldown) {
            attackTimer = 0.0f;
            attack(enemies);
        }
    }


    virtual void attack(DynamicArray<Enemy*>& enemies) {
        for (int i = 0; i < enemies.getSize(); ++i) {
            Enemy* e = enemies[i];
            if (e->isAlive()) {
                Position ep = e->getPosition();
                float distance = sqrt(pow(ep.x - pos.x, 2) + pow(ep.y - pos.y, 2));
                if (distance <= range) {
                    e->takeDamage(damage);
                    break; // Attack one enemy per round
                }
            }
        }
    }

    void drawRange() const {
        if (showRange) {
            Vector2 center = {
                pos.x * CELL_SIZE + CELL_SIZE / 2,
                pos.y * CELL_SIZE + CELL_SIZE / 2 + UI_HEIGHT
            };

            DrawCircleSector(
                center,
                range * CELL_SIZE,
                0, 360, 32,
                Color{ 120, 0, 120, 50 } 
            );

        }
    }

    void setShowRange(bool show) { showRange = show; }

    virtual void displayInfo(Vector2 mousePos) const = 0;


    Position getPosition() const { return pos; }
    char getSymbol() const { return symbol; }
    int getCost() const { return cost; }
    int getRange() const { return range; }
    int getDamage() const { return damage; }
    int getLevel() const { return level; }

    virtual void upgradePath1() = 0;
    virtual void upgradePath2() = 0;

    bool canUpgrade(int path, Player* player) const{
        return ((level < maxLevel) && (player->canAfford(getUpgradeCost(path))));
    }


    int getUpgradeCost(int path) const {
        if (path == 1 || path == 2)
            return (75 + (level * 25));
    }

};

#endif