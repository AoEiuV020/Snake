//
// Created by AoEiuV020 on 2017.11.08-14:28:33.
//

#ifndef SNAKE_SDL2VIEW_H
#define SNAKE_SDL2VIEW_H

#ifndef __WIN32

#include <thread>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "view/SnakeView.h"

class SnakePresenter;

/**
 * 使用SDL绘制游戏界面，
 */
class SDLView : public SnakeView {
public:
    /**
     * 开始绘图，
     * 游戏开始时调用，
     * 启动监听事件的线程，
     */
    void onStart() override;

    void init(int, char **) override;

    /**
     * 游戏循环，
     * 游戏结束前循环绘图，
     */
    void loop() override;

    /**
     * 结束绘图，
     * 游戏结束时调用，
     */
    void destroy();

    /**
     * 初始化配置SDL,
     */
    void initSDL();

    /**
     * 打印消息，比如游戏胜利失败，
     */
    void message(std::string message) override;

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
     * 供绘图线程调用的方法，
     */
    void drawCallable();

    /**
     * 监听SDL事件线程，
     */
    std::thread eventThread;

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
     * 是否退出SDL界面，
     */
    bool quit = false;

    /**
     * SDL窗口，
     */
    SDL_Window *window;

    /**
     * SDL渲染器，
     */
    SDL_Renderer *renderer;

    /**
     * 资源目录，
     */
    std::string resourceDir;

    // 各种贴图，背景，食物，墙，身体，四个方向的蛇头，
    SDL_Texture *background;
    SDL_Texture *food;
    SDL_Texture *wall;
    SDL_Texture *body;
    SDL_Texture *headUp;
    SDL_Texture *headDown;
    SDL_Texture *headLeft;
    SDL_Texture *headRight;

    /**
     * 字体，
     */
    TTF_Font *font;
    /**
     * 文字颜色，
     */
    SDL_Color textColor = {0x2b, 0x2b, 0x2b, 0xff};

    /**
     * 缓存消息，绘图时画出来，
     */
    std::string msg;

    void setting() const;
};

#endif // __WIN32

#endif //SNAKE_SDL2VIEW_H
