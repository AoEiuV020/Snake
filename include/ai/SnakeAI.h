//
// Created by AoEiuV020 on 2017.10.31-21:08:32.
//

#ifndef SNAKE_SNAKEAI_H
#define SNAKE_SNAKEAI_H


#include <model/Map.h>
#include <model/Snake.h>

class SnakeAI {
public:
    typedef Map::SizeType SizeType;
    void enableHamilton();

    void setMap(Map *map_);

    void decideNext();

    void setSnake(Snake *snake_);

private:
    bool hamiltonEnabled = false;

    Map *map;

    Snake *snake;

    /*
    Find path between two points.

    @param path The result path will be stored in this field.
    */
    void findMinPathToFood(std::list<Direction> &path);
    void findMaxPathToTail(std::list<Direction> &path);

    /*
    Find path from the snake's head to a given position.

    @param type 0->shortest path, 1->longest path
    @param to   The given position
    @param path The result path will be stored in this field.
    */
    void findPathTo(const int type, const Pos &to, std::list<Direction> &path);

    /*
    Find the shortest path as straight as possible between two positions.

    @param from The starting position
    @param to   The ending position
    @param path The result will be stored in this field
    */
    void findMinPath(const Pos &from, const Pos &to, std::list<Direction> &path);

    /*
    Find the longest path between two positions.

    @param from The starting position
    @param to   The ending position
    @param path The result will be stored in this field
    */
    void findMaxPath(const Pos &from, const Pos &to, std::list<Direction> &path);

    /*
    Build a path between two positions.

    @param from The start position
    @param to   The end position
    @param path The result will be stored in this field.
    */
    void buildPath(const Pos &from, const Pos &to, std::list<Direction> &path) const;

    /*
    Build a Hamiltonian cycle on the map.
    The path index will be stored in the 'value' field of each Point.
    */
    void buildHamilton();
};


#endif //SNAKE_SNAKEAI_H
