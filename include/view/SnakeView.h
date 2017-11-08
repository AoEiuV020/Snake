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

    /**
     * 关联presenter,
     */
    virtual void setPresenter(SnakePresenter *presenter_);

    /**
     * 绘图，
     * 未必是马上绘图，先保存这个map,并标记有map需要绘制，
     * @param map
     */
    virtual void draw(Map *map);

    /**
     * 开始游戏，
     */
    virtual void start();

    /**
     * 初始化视图，
     * 空实现，
     */
    virtual void init();

    /**
     * 结束绘图，
     */
    virtual void stop();

    /**
     * 打印文字，
     * 空实现，要在子类中重写，
     */
    virtual void printMsg(const std::string &msg);

    /**
     * 改得分，
     * 得分改变时presenter调用这个方法通知分数修改，
     */
    virtual void onScoreChanged(int score_);

    virtual void loop();

protected:
    Map *map = nullptr;
    SnakePresenter *presenter = nullptr;

    /**
     * 标记当前的map是否已经绘制，避免重复绘制，
     */
    bool drown = true;

    /**
     * 缓存得分，
     */
    int score;

    /**
     * 游戏是否正在运行，
     */
    static bool gameRunning;
};


#endif //SNAKE_SNAKEVIEW_H
