//
// Created by AoEiuV020 on 2017.10.31-16:05:12.
//

#ifndef _WIN32
#include <view/SDLView.h>
#else
#include "view/ConsoleView.h"
#endif
#include "view/FileView.h"
#include "Game.h"
#include "presenter/SnakePresenter.h"
#include "view/ViewWrapper.h"

/**
 * 启动游戏，
 */
int Game::launch(int argc, char **argv) {
    SnakeView *view;

#ifndef _WIN32
    // 非Windows才启用SDL, 因为没有在Windows上测试，
    auto *sdlView = new SDLView();
    view = sdlView;
#else
    auto *consoleView = new ConsoleView();
    view = consoleView;
#endif


    auto *presenter = new SnakePresenter();

    // mvp模式，关联view和presenter,
    // view负责绘图和用户交互，
    // presenter负责游戏本身的逻辑，并通知view绘图，
    view->setPresenter(presenter);
    presenter->attach(view);


    // 视图绘图，
    view->init(argc, argv);

    // 启动游戏，
    view->start();

    // 进入游戏循环，其实就是个死循环，
    view->loop();

    return 0;
}
