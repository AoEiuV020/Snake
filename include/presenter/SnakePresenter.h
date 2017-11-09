//
// Created by AoEiuV020 on 2017.10.31-15:41:44.
//

#ifndef SNAKE_SNAKEPRESENTER_H
#define SNAKE_SNAKEPRESENTER_H

#include "model/Snake.h"
#include "util/Console.h"
#include <thread>
#include <mutex>

class SnakeView;

/**
 * 游戏逻辑，
 */
class SnakePresenter {
public:
    SnakePresenter();

    ~SnakePresenter();

    /**
     * 关联视图,
     * 保存这个view, 需要绘图时通知这个view, 本类不管绘图相关细节，
     */
    void attach(SnakeView *view_);

    /**
     * 设置地图行数，
     * 在游戏初始化前修改才有效，
     * @param n 行数，
     */
    void setMapRow(int n);

    /**
     * 设置地图列数，
     * 在游戏初始化前修改才有效，
     * @param n 列数，
     */
    void setMapCol(int n);

    /**
     * 设置蛇自动移动时间间隔，单位毫秒，
     */
    void setMoveInterval(long moveInterval_);

    /**
     * 设置AI是否启用，
     */
    void setEnableAI(bool enableAI_);

    /**
     * 游戏初始化，
     * 初始化地图，蛇身，AI，
     */
    void init();

    /**
     * 启动游戏，
     */
    void run();

    /**
     * 正常退出游戏，
     */
    void exitGame();

    /**
     * 往指定方向移动一格，
     * @param direction 移动方向，
     */
    void move(Direction direction);

    /**
     * 切换暂停与否，
     */
    void pauseToggle();

private:
    /**
     * 视图类对象，蛇移动后通知视图刷新，
     */
    SnakeView *view;

    int mapRowCnt = 10;
    int mapColCnt = 10;

    Map *map = nullptr;
    Snake snake;

    /**
     * 初始化地图，
     * 判断地图不能太小，
     */
    void initMap();

    /**
     * 初始化蛇，
     */
    void initSnake();

    /**
     * AI类对象，
     */
    SnakeAI *snakeAI;

    /**
     * 初始化AI,
     */
    void initAI();

    /**
     * 调用AI方法得到下一个方向，
     */
    void decideNext();

    /**
     * 蛇移动方法，
     * 在这里判断是否已经胜利,
     */
    void moveSnake();

    /**
     * 自动移动线程，
     */
    std::thread moveThread;

    /**
     * 供自动移动线程调用的方法，
     */
    void autoMoveCallable();

    /**
     * 自动移动线程是否要继续运行，
     * 游戏启动时改成true，游戏结束时改回false通知线程结束，
     */
    volatile bool gameRunning = false;

    /**
     * 蛇自动移动间隔，单位毫秒ms,
     * 影响蛇的速度，每这么多毫秒移动一格，
     */
    long moveInterval = 30;
    /**
     * 游戏是否处于暂时状态，
     */
    bool pause = false;
    /**
     * 是否启用AI,
     * 在游戏初始化前修改才有效，
     */
    bool enableAI;

    /**
     * 当蛇吃到食物时调用，
     * 加分并产生新的食物，
     */
    void onEatenFood();

    /**
     * 得分，
     */
    int score;
};


#endif //SNAKE_SNAKEPRESENTER_H
