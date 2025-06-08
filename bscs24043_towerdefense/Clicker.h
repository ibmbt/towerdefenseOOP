#ifndef CLICKER_H
#define CLICKER_H

#include "Enemy.h"
#include "dynamicArray.h"
#include "Position.h"


class Clicker : public Enemy {
public:
    Clicker(const DynamicArray<Position>& p) : Enemy('C', 50, 3, 15, p) {}
};

#endif