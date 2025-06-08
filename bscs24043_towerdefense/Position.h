#ifndef POSTITION_H
#define POSTITION_H
using namespace std;


struct Position {
    float x;
    float y;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }

    Position() : x(-1), y(-1) {}

    Position(float _x, float _y) : x(_x), y(_y) {}

    Position(const Position& other) : x(other.x), y(other.y) {}

    Position& operator=(const Position& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
        }
        return *this;
    }


};

#endif