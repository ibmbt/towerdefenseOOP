#ifndef GAME_H
#define GAME_H
#include "Player.h"
#include "Map.h"
#include "dynamicArray.h"


class Game {
private:
    Player player;
    Map map;
    DynamicArray<Enemy*> enemies;
    bool gameOver;
    bool gameWon;
    int currentWave;
    int waveSize;
    bool placementPhase;
    float waveTimer;
    float waveDelay;
    int enemiesToSpawn;

public:

};


#endif