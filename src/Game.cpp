//
// Created by AoEiuV020 on 2017.10.31-16:05:12.
//

#include <view/FileView.h>
#include <ai/SnakeAI.h>
#include <util/util.h>
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
    view = viewWrapper;

    auto *consoleView = new ConsoleView();

    // Set FPS. Default is 60.0
    consoleView->setFPS(60.0);

    viewWrapper->addView(consoleView);


//    auto *fileView = new FileView();
//    viewWrapper->addView(fileView);


    SnakePresenter *presenter = SnakePresenter::getInstance();

    // Set map's size(including boundaries). Default is 10*10. Minimum is 5*5.
    presenter->setMapRow(10);
    presenter->setMapCol(10);

    // Set the interval time between each snake's movement. Default is 30 ms.
    // To play classic snake presenter, set to 150 ms is perfect.
    presenter->setMoveInterval(30);

    // Set whether to enable the snake AI. Default is true.
    presenter->setEnableAI(true);

    // Set whether to use a hamiltonian cycle to guide the AI. Default is true.
    presenter->setEnableHamilton(true);

    viewWrapper->setPresenter(presenter);
    presenter->attach(viewWrapper);

    presenter->init();

    // 开始绘图，
    view->start();
    // 进入游戏循环，
    presenter->run();
    return presenter->getExitCode();
}
