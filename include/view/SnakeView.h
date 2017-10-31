//
// Created by AoEiuV020 on 2017.10.31-16:03:20.
//

#ifndef SNAKE_SNAKEVIEW_H
#define SNAKE_SNAKEVIEW_H

#include "model/Map.h"

class SnakePresenter;

class SnakeView {
public:
    typedef Map::SizeType SizeType;

    virtual void setPresenter(SnakePresenter *presenter_);

    virtual void draw(Map *map);

    virtual void start();

    virtual void stop();

    virtual void printMsg(const std::string &msg);

protected:
    Map *map = nullptr;
    SnakePresenter *presenter = nullptr;

};


#endif //SNAKE_SNAKEVIEW_H
