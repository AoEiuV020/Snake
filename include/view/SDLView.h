//
// Created by AoEiuV020 on 2017.11.08-14:28:33.
//

#ifndef SNAKE_SDL2VIEW_H
#define SNAKE_SDL2VIEW_H

#include <thread>
#include <SDL2/SDL.h>
#include "view/SnakeView.h"

class SnakePresenter;

class SDLView : public SnakeView {
public:
    /**
     * 开始绘图，
     * 游戏开始时调用，
     * 控制台清屏，并启动绘图线程和监听键盘的线程，
     */
    void start() override;

    /**
     * 结束绘图，
     * 游戏结束时调用，
     * 通知子线程结束，
     */
    void stop() override;

    void printMsg(const std::string &msg) override;

    void initSDL();

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
     * 监听SDL事件线程，
     */
    std::thread eventThread;  // Thread to receive keyboard instructions
    /**
     * 供SDL事件线程调用的方法，
     */
    void eventCallable();

    /**
     * 当键盘输入方向键时调用，
     * 通知presenter控制蛇移动，
     * @param direction 方向，
     */
    void keyboardMove(Direction direction);

    /**
     * SDL窗口，
     */
    SDL_Window *window;
    
    /**
     * SDL表面，
     */
    SDL_Surface *screenSurface;

    /**
     * SDL渲染器,
     */
    SDL_Renderer *renderer;
};


#endif //SNAKE_SDL2VIEW_H
