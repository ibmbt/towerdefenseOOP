#ifndef CELL_H
#define CELL_H
#include "Position.h"
using namespace std;


class Cell {
private:
    Position pos;
    bool hasTower;
    bool hasEnemy;
    char type; // '.' for empty, 'x' for path, '#' for blocked

public:
    Cell() : pos{ -1, -1 }, hasTower(false), hasEnemy(false), type('.') {}
    Cell(float x, float y, char t) : pos{ x, y }, hasTower(false), hasEnemy(false), type(t) {}

    bool isEmpty() const { return !hasTower && !hasEnemy && type == '.'; }
    bool isPath() const { return type == 'x'; }
    void placeTower() { hasTower = true; }
    void removeTower() { hasTower = false; }
    void placeEnemy() { hasEnemy = true; }
    void removeEnemy() { hasEnemy = false; }
    Position getPosition() const { return pos; }
    char getType() const { return type; }
};


#endif