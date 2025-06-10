#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include "raylib.h"

class SoundManager {
private:
    Sound towerPlaceSound;
    Sound enemyDeathSound;
    Sound sellSound;
    Sound gameoverSound;

public:
    SoundManager() {
        InitAudioDevice();
        // Load all sounds during construction
        towerPlaceSound = LoadSound("resources/sounds/place.wav");
        enemyDeathSound = LoadSound("resources/sounds/kill.wav");
        sellSound = LoadSound("resources/sounds/sell.wav");
        gameoverSound = LoadSound("resources/sounds/gameover.wav");
    }

    ~SoundManager() {
        // Cleanup all sounds
        UnloadSound(towerPlaceSound);
        UnloadSound(enemyDeathSound);
        UnloadSound(sellSound);
        UnloadSound(gameoverSound);
        CloseAudioDevice();
    }

    // Direct access methods
    void playTowerPlace() { PlaySound(towerPlaceSound); }
    void playEnemyDeath() { PlaySound(enemyDeathSound); }
    void playSell() { PlaySound(sellSound); }
    void playGameOver() { PlaySound(gameoverSound); }
};

#endif