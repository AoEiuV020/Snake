//
// Created by AoEiuV020 on 2017.10.31-15:41:44.
//

#include "ai/SnakeAI.h"
#include "presenter/SnakePresenter.h"
#include "view/ConsoleView.h"

#include "util/util.h"

#ifdef _WIN32
#include <Windows.h>
#endif

using std::string;
using std::list;

const string SnakePresenter::MSG_BAD_ALLOC = "Not enough memory to run the game.";
const string SnakePresenter::MSG_LOSE = "Oops! You lose!";
const string SnakePresenter::MSG_WIN = "Congratulations! You Win!";
const string SnakePresenter::MSG_ESC = "Game ended.";

SnakePresenter::SnakePresenter() {}

SnakePresenter::~SnakePresenter() {
    delete map;
    map = nullptr;
}

SnakePresenter *SnakePresenter::getInstance() {
    static SnakePresenter instance;
    return &instance;
}

void SnakePresenter::attach(SnakeView *view_) {
    view = view_;
}

void SnakePresenter::setEnableHamilton(const bool enableHamilton_) {
    enableHamilton = enableHamilton_;
}

void SnakePresenter::setMapRow(const SizeType n) {
    mapRowCnt = n;
}

void SnakePresenter::setMapCol(const SizeType n) {
    mapColCnt = n;
}

void SnakePresenter::setMoveInterval(long moveInterval_) {
    moveInterval = moveInterval_;
}

void SnakePresenter::setEnableAI(bool enableAI_) {
    enableAI = enableAI_;
}

void SnakePresenter::run() {
    runMoveThread = true;

    map->createRandFood();

    moveThread = std::thread(&SnakePresenter::autoMoveCallable, this);
    moveThread.detach();

    while (runMainThread) {}
}

void SnakePresenter::autoMoveCallable() {
    try {
        while (runMoveThread) {
            util::sleep(moveInterval);
            if (!pause) {
                if (enableAI) {
                    decideNext();
                }
                moveSnake();
            }
        }
    } catch (const std::exception &e) {
        exitGameErr(e.what());
    }
}

void SnakePresenter::exitGame() {
    exitGame(MSG_ESC);
}

void SnakePresenter::exitGame(const std::string &msg) {
    mutexExit.lock();
    if (runMainThread) {
        // 游戏结果，
        util::sleep(100);
        // 自动移动线程停止，
        runMoveThread = false;
        // 停止绘图，
        view->stop();
        util::sleep(100);
        printMsg(msg);
    }
    runMainThread = false;
    mutexExit.unlock();
}

void SnakePresenter::exitGameErr(const std::string &err) {
    exitGame("ERROR: " + err);
}

void SnakePresenter::printMsg(const std::string &msg) {
    view->printMsg(msg);
}

void SnakePresenter::moveSnake() {
    mutexMove.lock();
    if (map->isAllBody()) {
        mutexMove.unlock();
        exitGame(MSG_WIN);
        return;
    }
    try {
        Direction d = snake.getDirection();
        if (!snake.isDead() && d != NONE) {
            Pos nextPos = snake.getHead().getAdj(d);
            Point nextPoint = map->getPoint(nextPos);
            if (!map->isSafe(nextPos)) {
                snake.setDead(true);
                exitGame(MSG_LOSE);
            } else {
                if (nextPoint.getType() == Point::FOOD) {
                    snake.move(true);
                    onEatenFood();
                } else {
                    snake.move(false);
                }
            }
            view->draw(map);
        }
        mutexMove.unlock();
    } catch (const std::exception &e) {
        mutexMove.unlock();
        throw;
    }
}

void SnakePresenter::onEatenFood() {
    score += 1;
    view->onScoreChanged(score);
    map->createRandFood();
}

void SnakePresenter::init() {
    try {
        initMap();
        initSnake();
        initAI();
        view->draw(map);
    } catch (const std::exception &e) {
        exitGameErr(e.what());
        exitCode = -1;
    }
}

void SnakePresenter::initMap() {
    if (mapRowCnt < 5 || mapColCnt < 5) {
        string msg = "SnakePresenter.initMap(): Map size at least 5*5. Current size "
                     + util::toString(mapRowCnt) + "*" + util::toString(mapColCnt) + ".";
        throw std::range_error(msg.c_str());
    }
    map = new Map(mapRowCnt, mapColCnt);
    if (!map) {
        exitGameErr(MSG_BAD_ALLOC);
    } else {
        // Add some extra walls manully
    }
}

void SnakePresenter::initSnake() {
    snake.setMap(map);
    snake.addBody(Pos(1, 3));
    snake.addBody(Pos(1, 2));
    snake.addBody(Pos(1, 1));
}

void SnakePresenter::initAI() {
    snakeAI = new SnakeAI();
    snakeAI->setMap(map);
    snakeAI->setSnake(&snake);
    if (enableHamilton) {
        snakeAI->enableHamilton();
    }
}

void SnakePresenter::move(Direction direction) {
    if (pause) {
        snake.setDirection(direction);
        moveSnake();  // Accelerate
    } else if (!enableAI) {
        if (snake.getDirection() == direction) {
            moveSnake();  // Accelerate
        } else {
            snake.setDirection(direction);
        }
    }
}

int SnakePresenter::getExitCode() {
    return exitCode;
}

void SnakePresenter::decideNext() {
    snakeAI->decideNext();
}

void SnakePresenter::pauseToggle() {
    pause = !pause;
}

