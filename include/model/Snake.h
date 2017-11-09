#ifndef SNAKE_SNAKE_H_
#define SNAKE_SNAKE_H_

#include "model/Map.h"

class SnakeAI;

/**
 * 游戏主角，蛇，
 */
class Snake {
public:
    // 设置友元类，方便AI类处理，
    friend class SnakeAI;

    Snake();

    ~Snake();

    /**
     * 传入蛇头方向，
     */
    void setDirection(const Direction &d);

    /**
     * @return 得到蛇头方向，
     */
    Direction getDirection() const;

    /**
     * 传入地图，
     */
    void setMap(Map *const m);

    /**
     * @return 得到蛇是否死亡，
     */
    bool isDead() const;

    /**
     * 添加蛇身，游戏开始前添加好蛇的长度，
     */
    void addBody(const Pos &p);

    /**
     * 蛇移动一格，
     * @param eatenFood 是否吃到食物，如果吃到则不删除尾巴，造成蛇变长一格的效果，
     */
    void move(bool eatenFood);

    /**
     * @return 返回蛇头位置，
     */
    const Pos &getHead() const;

    /**
     * 设置蛇是否死亡，
     */
    void setDead(bool isDead);

private:
    /**
     * 游戏地图，用于在蛇身体加长时改变地图上的点，
     */
    Map *map = nullptr;

    /**
     * 保存蛇身每个点位置的列表,
     */
    std::list<Pos> bodies;

    /**
     * 表示蛇头方向，
     */
    Direction direc = NONE;

    /**
     * 表示蛇是否死亡,
     */
    bool dead = false;

    /**
     * 删除蛇尾，蛇在前进时调用本函数删除蛇尾，
     */
    void removeTail();

    /**
     * @return 返回蛇尾位置，
     */
    const Pos &getTail() const;

};

#endif
