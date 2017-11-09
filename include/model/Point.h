#ifndef SNAKE_POINT_H_
#define SNAKE_POINT_H_

#include "model/Pos.h"
#include <cstdint>

/**
 * 地图上的点，
 */
class Point {
public:
    /**
     * 枚举地图上的点的类型，
     * 包括空白，墙，食物，蛇头，蛇身，蛇尾，
     */
    enum Type {
        EMPTY,
        WALL,
        FOOD,
        SNAKE_BODY,
        SNAKE_HEAD,
        SNAKE_TAIL,
    };

    Point();

    ~Point();

    void setType(Type type_);

    Type getType() const;

private:
    /**
     * 类型，表示该点上有什么，
     */
    Type type = EMPTY;
};

#endif
