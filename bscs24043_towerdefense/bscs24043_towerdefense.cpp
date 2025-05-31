#include <iostream>
#include <fstream>
#include "raylib.h"
#include "dynamicArray.h"
using namespace std;


struct Position {
    int x;
    int y;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};


class Enemy {
protected:
    char symbol;
    int health;
    Position pos;
    int pathIndex;
    DynamicArray<Position> path;

public:
    Enemy(char sym, int hp, const DynamicArray<Position>& p)
        : symbol(sym), health(hp), path(p), pathIndex(0) {
        pos = path[0];
    }

    virtual ~Enemy() {}


    virtual void move() {
        if (pathIndex + 1 >= path.getSize()) return;

        Position next = path[pathIndex + 1];

        if (pos.x < next.x) pos.x++;
        else if (pos.x > next.x) pos.x--;
        else if (pos.y < next.y) pos.y++;
        else if (pos.y > next.y) pos.y--;

        if (pos == next) {
            pathIndex++;
        }
    }



    int getHealth() const {
        return health;
    }

    virtual char getSymbol() const { return symbol; }
    virtual Position getPosition() const { return pos; }
    virtual bool isAlive() const { return health > 0; }
    virtual void takeDamage(int dmg) {
        health = health - dmg;
        if (health < 0) health = 0;
    }

    bool reachedEnd() const {
        return pos == path[path.getSize() - 1];
    }

};



class Tower {
protected:
    char symbol;
    int damage;
    int range;
    Position pos;

public:
    Tower(char sym, int dmg, int rng, Position p) : symbol(sym), damage(dmg), range(rng), pos(p) {}

    virtual ~Tower() {}

    virtual void attack(DynamicArray<Enemy*>& enemies) {
        for (int i = 0; i < enemies.getSize(); ++i) {
            Enemy* e = enemies[i];
            if (e->isAlive()) {
                Position ep = e->getPosition();
                int dx = abs(ep.x - pos.x);
                int dy = abs(ep.y - pos.y);
                if (dx + dy <= range) {
                    cout << symbol << " hits " << e->getSymbol()
                        << " at (" << ep.x << "," << ep.y << ") for "
                        << damage << " damage! "
                        << "Remaining HP: " << e->getHealth() << std::endl;
                    e->takeDamage(damage);
                    break; 
                }
            }
        }
    }


    virtual Position getPosition() const { return pos; }
    virtual char getSymbol() const { return symbol; }
};


int main() {

	return 0;
}