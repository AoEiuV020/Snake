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
    if (gameRunning) {
        return;
    }
    gameRunning = true;

    map->createRandFood();

    moveThread = std::thread(&SnakePresenter::autoMoveCallable, this);
    moveThread.detach();
}

void SnakePresenter::autoMoveCallable() {
    try {
        while (gameRunning) {
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
        // 自动移动线程停止，
        gameRunning = false;
        printMsg(msg);
        // 停止绘图，
        view->stop();
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
    // 简单的互斥锁，确保本方法同时只有一个线程在运行，
    // 主要是自动移动线程和监听键盘输入手动前进线程可能同时调用本方法，
    mutexMove.lock();
    // 如果全地图都是蛇身则胜利，不继续移动蛇身，
    if (map->isAllBody()) {
        mutexMove.unlock();
        exitGame(MSG_WIN);
        return;
    }
    try {
        Direction d = snake.getDirection();
        // 如果蛇已经死了则不继续移动，这个判断似乎是多余的，
        // 如果蛇没有方向，则不移动，比如手动游戏刚开始时，
        if (!snake.isDead() && d != NONE) {
            // 得到蛇头下一格，
            Pos nextPos = snake.getHead().getAdj(d);
            Point nextPoint = map->getPoint(nextPos);
            // 判断下一格是否安全，不安全就死，游戏结束，
            if (!map->isSafe(nextPos)) {
                snake.setDead(true);
                exitGame(MSG_LOSE);
            } else {
                // 判断下一格是否是食物，并控制蛇移动，
                if (nextPoint.getType() == Point::FOOD) {
                    snake.move(true);
                    onEatenFood();
                } else {
                    snake.move(false);
                }
            }
            // 通知view绘图，
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
        // 任何原因初始化失败都简单设置状态码-1，
        exitCode = -1;
    }
}

void SnakePresenter::initMap() {
    // 规定地图大小不小于5*5,
    if (mapRowCnt < 5 || mapColCnt < 5) {
        string msg = "SnakePresenter.initMap(): Map size at least 5*5. Current size "
                     + util::toString(mapRowCnt) + "*" + util::toString(mapColCnt) + ".";
        throw std::range_error(msg.c_str());
    }
    map = new Map(mapRowCnt, mapColCnt);
    // 创建对象失败可能是地图太大，申请空间失败，
    if (!map) {
        exitGameErr(MSG_BAD_ALLOC);
    } else {
        // 如果要在地图添加额外的墙可以在这里添加，
        // 现在没有，
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
    // 蛇头只能前进左右拐，不能后退，反正后退必死，
    if(direction != snake.getDirection()) {
        switch (snake.getDirection()) {
            case UP:
            case DOWN:
                if (direction == UP || direction == DOWN) {
                    return;
                }
                break;
            case LEFT:
            case RIGHT:
                if (direction == LEFT || direction == RIGHT) {
                    return;
                }
                break;
            default:
                break;
        }
    }
    if (pause) {
        // 如果游戏处于暂停状态，直接移动，
        snake.setDirection(direction);
        moveSnake();  // Accelerate
    } else if (!enableAI) {
        // 如果是手动游戏，
        if (snake.getDirection() == direction) {
            // 如果是移动方向和原本的一致，则向前移动一格，
            // 比如蛇头向右时，敲右方向键，则蛇多向右移动一格，
            moveSnake();
        } else {
            // 设置蛇头方向，等下次自动移动，
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

