//
// Created by AoEiuV020 on 2017.10.31-16:20:31.
//

#ifndef SNAKE_CONSOLEVIEW_H
#define SNAKE_CONSOLEVIEW_H

#include "view/SnakeView.h"
#include <thread>

class SnakePresenter;

/**
 * 控制台游戏视图，
 * 在控制台打印游戏地图，
 */
class ConsoleView : public SnakeView {

public:
    /**
     * 开始绘图，
     * 游戏开始时调用，
     * 控制台清屏，并启动绘图线程和监听键盘的线程，
     */
    void onStart() override;

    void message(std::string message) override;

    /**
     * 设置游戏显示帧数，
     * 即每秒绘图多少次，
     * @param fps_ 帧数，
     */
    void setFPS(double fps_);

private:
    /**
     * 实际的绘图方法，
     */
    void drawMapContent();

    /**
     * 为了满足指定帧数刷新率而让线程休息，
     * 在方法内计算需要休息的时间，
     */
    void sleepFPS();

    /**
    * 游戏显示帧数，
    * 即每秒绘图多少次，
    */
    double fps = 60.0;

    /**
     * 绘图线程，
     */
    std::thread drawThread;
    /**
     * 供绘图线程调用的方法，
     */
    void drawCallable();
    /**
     * 监听键盘输入线程，
     */
    std::thread keyboardThread;  // Thread to receive keyboard instructions
    /**
     * 供监听键盘线程调用的方法，
     */
    void keyboardCallable();

    /**
     * 当键盘输入方向键时调用，
     * 通知presenter控制蛇移动，
     * @param direction 方向，
     */
    void keyboardMove(Direction direction);

    /**
     * 当键盘有输入时调用，
     * 根据键值判断是游戏蛇移动还是暂停或者结束，
     * @param key 键值，
     */
    void onKeyboardHit(int key);
};


#endif //SNAKE_CONSOLEVIEW_H
