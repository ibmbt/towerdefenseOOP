#ifndef TOWER_H
#define TOWER_H
#include "Position.h"
#include "dynamicArray.h"
#include "Enemy.h"


class Tower
{
protected:
    char symbol;
    int damage;
    int range;
    Position pos;
    int cost;
    float cooldown;
    float attackTimer;

public:
    Tower(char sym, int dmg, int rng, Position p, int cst, float cd)
        : symbol(sym), damage(dmg), range(rng), pos(p), cost(cst), cooldown(cd), attackTimer(0.0f)
    {
    }

    virtual ~Tower() {}

    virtual void update(float deltaTime, DynamicArray<Enemy*>& enemies)
    {
        attackTimer += deltaTime;
        if (attackTimer >= cooldown)
        {
            attackTimer = 0.0f;
            attack(enemies);
        }
    }

    virtual void attack(DynamicArray<Enemy*>& enemies)
    {
        for (int i = 0; i < enemies.getSize(); ++i)
        {
            Enemy* e = enemies[i];
            if (e->isAlive())
            {
                Position ep = e->getPosition();
                float distance = sqrt(pow(ep.x - pos.x, 2) + pow(ep.y - pos.y, 2));
                if (distance <= range)
                {
                    e->takeDamage(damage);
                    break; // Attack one enemy per round
                }
            }
        }
    }

    Position getPosition() const { return pos; }
    char getSymbol() const { return symbol; }
    int getCost() const { return cost; }
    int getRange() const { return range; }
};

#endif