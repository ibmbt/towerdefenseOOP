#ifndef SAVELOAD_H
#define SAVELOAD_H
#include <iostream>
#include "myString.h"
#include "Globals.h"
#include "Tower.h"
#include "JoelTower.h"
#include "EllieTower.h"
#include "Exceptions.h"
#include "Map.h"


class SaveLoad {
public:
    static void saveGame(const string& filename, GameState currentState, int waveNumber, const Player& player) {
        ofstream fout(filename, ios::binary);
        if (!fout) {
            throw FileNotFound();
        }

        fout.write((char*)&currentState, sizeof(GameState));
        fout.write((char*)&waveNumber, sizeof(int));

        int coins = player.getCoins();
        int lives = player.getLives();
        fout.write((char*)&coins, sizeof(int));
        fout.write((char*)&lives, sizeof(int));

        int towerCount = player.getTowers().getSize();
        fout.write((char*)&towerCount, sizeof(int));

        for (int i = 0; i < towerCount; ++i) {
            Tower* t = player.getTowers()[i];
            char type = t->getSymbol();
            int level = t->getLevel();
            Position pos = t->getPosition();

            fout.write((char*)&type, sizeof(char));
            fout.write((char*)&level, sizeof(int));
            fout.write((char*)&pos, sizeof(Position));
        }
        fout.close();
    }

    static void loadGame(const string& filename, GameState& currentState, int& waveNumber, Player& player, Map& map) {
        ifstream fin(filename, ios::binary);
        if (!fin) {
            throw FileNotFound();
        }

        fin.read((char*)&currentState, sizeof(GameState));
        fin.read((char*)&waveNumber, sizeof(int));

        int coins = 0, lives = 0;
        fin.read((char*)&coins, sizeof(int));
        fin.read((char*)&lives, sizeof(int));
        player.setPlayer(lives, coins);
        player.resetPlayer();

        int towerCount;
        fin.read((char*)&towerCount, sizeof(int));

        for (int i = 0; i < towerCount; ++i) {
            char type;
            int level;
            Position pos;
            fin.read((char*)&type, sizeof(char));
            fin.read((char*)&level, sizeof(int));
            fin.read((char*)&pos, sizeof(Position));

            Tower* t = nullptr;
            if (type == 'J') t = new JoelTower(pos);
            else if (type == 'E') t = new EllieTower(pos);

            for (int l = 0; l < level; ++l) {
                if (l % 2 == 0) t->upgradePath1();
                else t->upgradePath2();
            }

            player.addTower(t);
        }
        fin.close();

        switch (currentState) {
        case LEVEL_1:
            map.load("resources/maps/map1.txt");
            break;
        case LEVEL_2:
            map.load("resources/maps/map2.txt");
            break;
        case LEVEL_3:
            map.load("resources/maps/map3.txt");
            break;
        default:
            break;
        }

    }
};

#endif