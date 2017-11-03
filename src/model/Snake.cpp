#include "model/Snake.h"
#include "util/util.h"
#include <queue>
#include <algorithm>

using std::vector;
using std::list;
using std::queue;
using util::Random;

Snake::Snake() {}

Snake::~Snake() {}

void Snake::setDirection(const Direction &d) {
    direc = d;
}

void Snake::setMap(Map *const m) {
    map = m;
}

Direction Snake::getDirection() const {
    return direc;
}

bool Snake::isDead() const {
    return dead;
}

void Snake::addBody(const Pos &p) {
    if (bodies.size() == 0) {  // Insert a head
        map->getPoint(p).setType(Point::Type::SNAKE_HEAD);
    } else {  // Insert a body
        if (bodies.size() > 1) {
            const Pos &oldTail = getTail();
            map->getPoint(oldTail).setType(Point::Type::SNAKE_BODY);
        }
        map->getPoint(p).setType(Point::Type::SNAKE_TAIL);
    }
    bodies.push_back(p);
}

void Snake::move(bool eatenFood) {
    map->getPoint(getHead()).setType(Point::Type::SNAKE_BODY);
    Pos newHead = getHead().getAdj(direc);
    bodies.push_front(newHead);
    if (!eatenFood) {
        removeTail();
    }
    map->getPoint(newHead).setType(Point::SNAKE_HEAD);
}

const Pos &Snake::getHead() const {
    return *bodies.begin();
}

const Pos &Snake::getTail() const {
    return *bodies.rbegin();
}

void Snake::removeTail() {
    if (map->getPoint(getTail()).getType() == Point::Type::SNAKE_TAIL) {
        map->getPoint(getTail()).setType(Point::Type::EMPTY);
    }
    bodies.pop_back();
    if (bodies.size() > 1) {
        map->getPoint(getTail()).setType(Point::Type::SNAKE_TAIL);
    }
}

void Snake::setDead(bool isDead) {
    dead = isDead;
}
