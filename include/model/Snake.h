#ifndef SNAKE_SNAKE_H_
#define SNAKE_SNAKE_H_

#include "model/Map.h"

class SnakeAI;
/*
Game snake.
*/
class Snake {
public:
    typedef Map::SizeType SizeType;
    friend class SnakeAI;

    Snake();
    ~Snake();

    void setDirection(const Direction &d);
    void setMap(Map *const m);
    Direction getDirection() const;
    bool isDead() const;

    /*
    Add a new snake body.

    @param p The position of the new bodies
    @return  True if adding succeed, false otherwise
    */
    void addBody(const Pos &p);

    /*
    Move the snake along a given path.
    */
    void move(bool eatenFood);

/*
Get the head position.
*/
const Pos& getHead() const;

    void setDead(bool isDead);

private:
    Map *map = nullptr;
    std::list<Pos> bodies;

    Direction direc = NONE;

    bool dead = false;

    /*
    Remove the snake tail.
    */
    void removeTail();

    /*
    Get the tail position.
    */
    const Pos& getTail() const;

};

#endif
