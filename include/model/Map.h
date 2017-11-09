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
    Return the number of points excluding the boundaries.
    */
    int getSize() const;

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

    void removeFood();

    bool hasFood() const;

    const Pos &getFood() const;

    /*
    Return the manhatten distance between two positions.

    @param from The starting position
    @param to   The ending position
    @return     The manhatten distance
    */
    static int distance(const Pos &from, const Pos &to);

private:
    std::vector<std::vector<Point>> content;
    int size;
    Pos food;

    /*
    Return all empty positions.
    */
    std::vector<Pos> getEmptyPoints() const;
};

#endif