//
// Created by AoEiuV020 on 2017.10.31-16:03:20.
//

#ifndef SNAKE_SNAKEVIEW_H
#define SNAKE_SNAKEVIEW_H

#include <model/Snake.h>
#include "model/Map.h"

class SnakePresenter;

/**
 * 游戏视图虚基类，
 */
class SnakeView {
public:
    /**
     * 关联游戏逻辑,
     */
    void setPresenter(SnakePresenter *presenter_);

    /**
     * 绘图，
     * 未必是马上绘图，先保存这个map,并标记有map需要绘制，
     */
    virtual void draw(Map *map, Snake *snake_);

    /**
     * 开始游戏，
     */
    void start();

    /**
     * 初始化视图，
     * 空实现，
     */
    virtual void init(int, char **);

    /**
     * 结束绘图，
     */
    void stop();

    /**
     * 改得分，
     * 得分改变时presenter调用这个方法通知分数修改，
     */
    void setScore(int score_);

    /**
     * 显示消息，
     * @param message 消息，
     */
    virtual void message(std::string message);

    /**
     * 游戏循环，
     * 游戏结束前主线程不能退出了，
     */
    virtual void loop();

    /**
     * 胜利，
     */
    void win();

    /**
     * 失败，
     */
    void lose();

    virtual void onStart();

    virtual void onStop();

protected:
    /**
     * 绑定游戏逻辑，
     */
    static SnakePresenter *presenter;
    /**
     * 缓存地图，
     */
    Map *map = nullptr;
    /**
     * 缓存蛇，
     */
    Snake *snake = nullptr;

    /**
     * 标记当前的map是否已经绘制，避免重复绘制，
     */
    bool drown = true;

    /**
     * 缓存得分，
     */
    int score = 0;

    /**
     * 游戏是否正在运行，
     */
    static bool gameRunning;

    /**
     * 游戏退出，
     */
    void exit();

private:
    // 游戏失败消息，
    static const std::string MSG_LOSE;
    // 游戏胜利消息，
    static const std::string MSG_WIN;
    // 游戏退出消息，
    static const std::string MSG_EXIT;
};


#endif //SNAKE_SNAKEVIEW_H
