#ifndef ENEMY_H
#define ENEMY_H
#include "Position.h"
#include "dynamicArray.h"

class Enemy {
protected:
    char symbol;
    int health;
    int maxHealth;
    float speed;
    int worth;
    Position pos;
    int pathIndex;
    DynamicArray<Position> path;
    float progress;

public:
    Enemy(char sym, int hp, int spd, int wrth, const DynamicArray<Position>& p)
        : symbol(sym), health(hp), maxHealth(hp), speed(spd), worth(wrth), path(p), pathIndex(0), progress(0.0f) {
        pos = path[0];
    }

    virtual ~Enemy() {}

    virtual void update(float deltaTime) {
        if (pathIndex + 1 < path.getSize()) {
            Position next = path[pathIndex + 1];
            float dx = next.x - pos.x;
            float dy = next.y - pos.y;
            float length = sqrt(dx * dx + dy * dy);
            if (length > 0) {
                dx /= length;
                dy /= length;
            }
            pos.x += dx * speed * deltaTime;
            pos.y += dy * speed * deltaTime;

            if (abs(pos.x - next.x) < 0.1f && abs(pos.y - next.y) < 0.1f) {
                pathIndex++;
            }
        }
    }

    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    char getSymbol() const { return symbol; }
    Position getPosition() const { return pos; }
    bool isAlive() const { return health > 0; }
    int getWorth() const { return worth; }

    void takeDamage(int dmg) {
        health -= dmg;
        if (health < 0) health = 0;
    }

    bool reachedEnd() const {
        return pathIndex >= path.getSize() - 1;
    }
};

#endif