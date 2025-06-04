#ifndef TOWER_H
#define TOWER_H
#include "Position.h"
#include "dynamicArray.h"
#include "Enemy.h"


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

public:
    Tower(char sym, int dmg, int rng, Position p, int cst, float cd)
        : symbol(sym), damage(dmg), range(rng), pos(p), cost(cst), cooldown(cd), attackTimer(0.0f) {
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
                    break; 
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

    virtual void displayInfo(Vector2 mousePos) const {
        if (showRange) {}
    }


    Position getPosition() const { return pos; }
    char getSymbol() const { return symbol; }
    int getCost() const { return cost; }
    int getRange() const { return range; }
    int getDamage() const { return damage; }
};

#endif