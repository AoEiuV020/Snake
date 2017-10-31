//
// Created by AoEiuV020 on 2017.10.31-16:05:12.
//

#include "Game.h"
#include "presenter/SnakePresenter.h"
#include "view/ConsoleView.h"

Game *Game::getInstance() {
    static Game instance;
    return &instance;
}

int Game::launch() {
    SnakePresenter *presenter = SnakePresenter::getInstance();

    auto *view = new ConsoleView();

    view->setPresenter(presenter);

    presenter->init();

    // Set FPS. Default is 60.0
    view->setFPS(60.0);

    // Set the interval time between each snake's movement. Default is 30 ms.
    // To play classic snake presenter, set to 150 ms is perfect.
    view->setMoveInterval(30);

    // Set whether to enable the snake AI. Default is true.
    view->setEnableAI(true);

    // Set whether to use a hamiltonian cycle to guide the AI. Default is true.
    presenter->setEnableHamilton(true);

    // Set whether to record the snake's movements to file. Default is true.
    // The movements will be written to a file named "movements.txt".
    presenter->setRecordMovements(true);

    // Set map's size(including boundaries). Default is 10*10. Minimum is 5*5.
    presenter->setMapRow(10);
    presenter->setMapCol(10);

    // 开始绘图，
    view->start();
    // 进入游戏循环，
    presenter->run();
    return presenter->getExitCode();
}
