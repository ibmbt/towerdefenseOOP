#ifndef BLOATER_H
#define BLOATER_H

#include "Enemy.h"
#include "dynamicArray.h"
#include "Position.h"


class Bloater : public Enemy {
public:
    Bloater(const DynamicArray<Position>& p) : Enemy('B', 200, 1, 30, p) {}
};


#endif