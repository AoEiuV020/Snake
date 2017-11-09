#ifndef SNAKE_MAP_H_
#define SNAKE_MAP_H_

#include "model/Point.h"
#include <list>

/*
Game map.
*/
class Map {
public:
    Map(const int rowCnt_, const int colCnt_);

    ~Map();

    Point &getPoint(const Pos &p);

    const Point &getPoint(const Pos &p) const;

    int getRowCount() const;

    int getColCount() const;

    /*
    Check the state of the map.
    */
    bool isInside(const Pos &p) const;

    bool isSafe(const Pos &p) const;

    bool isAllBody() const;

    /*
    Food API.
    */
    void createRandFood();

    void createFood(const Pos &pos);

private:
    std::vector<std::vector<Point>> content;
    Pos food;

    /*
    Return all empty positions.
    */
    std::vector<Pos> getEmptyPoints() const;
};

#endif