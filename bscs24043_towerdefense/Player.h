#ifndef PLAYER_H
#define PLAYER_H
#include "dynamicArray.h"
#include "Tower.h"

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

    int getLives() const { return lives; }
    int getCoins() const { return coins; }
    const DynamicArray<Tower*>& getTowers() const { return towers; }
};

#endif
