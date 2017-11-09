//
// Created by AoEiuV020 on 2017.10.31-21:08:32.
//

#ifndef SNAKE_SNAKEAI_H
#define SNAKE_SNAKEAI_H


#include <model/Map.h>
#include <model/Snake.h>

class SnakeAI {
public:
    void init();

    void setMap(Map *map_);

    void decideNext();

    void setSnake(Snake *snake_);

private:

    Map *map;

    Snake *snake;

    /**
     * 查找距离蛇尾最远的路径，
     * @param path 路径保存在这个列表中，
     */
    void findMaxPathToTail(std::list<Direction> &path);

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

    std::vector<std::vector<Direction>> directionMap;
    std::vector<std::vector<bool>> visitMap;
    std::vector<std::vector<uint32_t>> distanceMap;
    std::vector<std::vector<Pos>> parentMap;
};


#endif //SNAKE_SNAKEAI_H
