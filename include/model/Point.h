#ifndef SNAKE_POINT_H_
#define SNAKE_POINT_H_

#include "model/Pos.h"
#include <cstdint>

/**
 * 地图上的点，
 */
class Point {
public:
    typedef uint32_t ValueType;

    static const ValueType MAX_VALUE = UINT32_MAX;

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

    void setParent(const Pos &p_);

    void setDist(const ValueType dist_);

    Type getType() const;

    Pos getParent() const;

    ValueType getDist() const;

private:
    /**
     * 类型，表示该点上有什么，
     */
    Type type = EMPTY;
    Pos parent;
    bool visit;
    ValueType dist;
    ValueType index;
};

#endif
