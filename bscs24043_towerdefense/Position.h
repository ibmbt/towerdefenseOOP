#ifndef POSTITION_H
#define POSTITION_H


struct Position
{
    float x;
    float y;

    bool operator==(const Position& other) const;

};


bool Position::operator==(const Position& other) const
{
    return x == other.x && y == other.y;
}

#endif