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

/*
Game controller.
*/
class SnakePresenter {
public:
    typedef Map::SizeType SizeType;

    static const Point::ValueType EMPTY_VALUE = 99999;

    ~SnakePresenter();

    /**
     * @return 返回单例对象，并没有必要，可以考虑不用单例模式，
     */
    static SnakePresenter *getInstance();

    /**
     * 关联view,
     * 保存这个view, 需要绘图时通知这个view, 本类不管绘图相关细节，
     */
    void attach(SnakeView *view_);

    void setEnableHamilton(const bool enableHamilton_);

    /**
     * 设置地图行数，
     * 在游戏初始化前修改才有效，
     * @param n 行数，
     */
    void setMapRow(const SizeType n);

    /**
     * 设置地图列数，
     * 在游戏初始化前修改才有效，
     * @param n 列数，
     */
    void setMapCol(const SizeType n);

    void setMoveInterval(long moveInterval_);

    void setEnableAI(bool enableAI_);

    /**
     * 游戏初始化，
     * 初始化地图，蛇身，AI，
     */
    void init();

    /**
     * 启动游戏，并进入游戏循环，
     */
    void run();

    /**
     * 异常退出游戏，
     * 退出并打印消息，
     * @param err 错误消息，
     */
    void exitGameErr(const std::string &err);

    /**
     * 正常退出游戏，
     */
    void exitGame();

    /**
     * @return 返回游戏退出状态码，目前只有成功0 和初始化失败-1 两种状态，
     */
    int getExitCode();

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
    static const std::string MSG_BAD_ALLOC;
    static const std::string MSG_LOSE;
    static const std::string MSG_WIN;
    static const std::string MSG_ESC;

    SnakeView *view;

    bool enableHamilton = true;
    SizeType mapRowCnt = 10;
    SizeType mapColCnt = 10;

    Map *map = nullptr;
    Snake snake;

    volatile bool runMainThread = true;  // Switch of the main thread

    std::mutex mutexMove;  // Mutex of moveSnake()
    std::mutex mutexExit;  // Mutex of exitGame()

    SnakePresenter();

    /**
     * 带着消息退出游戏，
     * @param msg 消息，可能是成功，失败，或主动退出，
     */
    void exitGame(const std::string &msg);

    /**
     * 打印消息，
     * 直接通知view去打印，
     * @param msg 消息，
     */
    void printMsg(const std::string &msg);

    /**
     * 初始化地图，
     * 判断地图不能太小，
     */
    void initMap();

    /**
     * 初始化蛇，
     */
    void initSnake();

    int exitCode;
    SnakeAI *snakeAI;

    /**
     * 初始化AI,
     */
    void initAI();

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
    volatile bool runMoveThread = false;

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
