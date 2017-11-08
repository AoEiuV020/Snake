//
// Created by AoEiuV020 on 2017.10.31-16:05:12.
//

#include <view/SDLView.h>
#include "view/FileView.h"
#include "Game.h"
#include "presenter/SnakePresenter.h"
#include "view/ConsoleView.h"
#include "view/ViewWrapper.h"

Game *Game::getInstance() {
    static Game instance;
    return &instance;
}

int Game::launch() {
    SnakeView *view;

    auto *viewWrapper = new ViewWrapper();

#ifndef _WIN32
    // 非Windows才启用SDL, 因为没有在Windows上测试，
    auto *sdlView = new SDLView();
    viewWrapper->addView(sdlView);
#else
    auto *consoleView = new ConsoleView();
    viewWrapper->addView(consoleView);
#endif

    view = viewWrapper;


    SnakePresenter *presenter = SnakePresenter::getInstance();

    // 设置默认地图大小，默认就是10 * 10,
    presenter->setMapRow(10);
    presenter->setMapCol(10);

    // 设置自动移动间隔，单位是毫秒ms, 默认30ms，
    // 也就是说蛇每30毫秒移动一格，
    presenter->setMoveInterval(30);

    // 设置是否启用AI自动移动，默认是，
    presenter->setEnableAI(true);

    // mvp模式，关联view和presenter,
    // view负责绘图和用户交互，
    // presenter负责游戏本身的逻辑，并通知view绘图，
    view->setPresenter(presenter);
    presenter->attach(view);

    // 游戏初始化，
    presenter->init();

    // 视图绘图，
    view->init();

    // 启动游戏，
    view->start();

    // 进入游戏循环，其实就是个死循环，
    view->loop();

    return 0;
}
