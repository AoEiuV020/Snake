//
// Created by AoEiuV020 on 2017.10.31-15:41:44.
//

#include "ai/SnakeAI.h"
#include "presenter/SnakePresenter.h"
#include "view/ConsoleView.h"
#include "util/util.h"

using std::string;
using std::list;

SnakePresenter::SnakePresenter() = default;

SnakePresenter::~SnakePresenter() {
    delete map;
    map = nullptr;
}

void SnakePresenter::attach(SnakeView *view_) {
    view = view_;
}

void SnakePresenter::setMapRow(const int n) {
    mapRowCnt = n;
}

void SnakePresenter::setMapCol(const int n) {
    mapColCnt = n;
}

void SnakePresenter::setMoveInterval(long moveInterval_) {
    moveInterval = moveInterval_;
}

void SnakePresenter::setEnableAI(bool enableAI_) {
    enableAI = enableAI_;
}

/**
 * 启动游戏，
 */
void SnakePresenter::run() {
    if (gameRunning) {
        return;
    }
    gameRunning = true;

    // 创建第一个食物，
    map->createRandFood();

    // 创建一个负责控制蛇自动移动的线程，
    moveThread = std::thread(&SnakePresenter::autoMoveCallable, this);
    moveThread.detach();
}

/**
 * 供自动移动线程调用的方法，
 */
void SnakePresenter::autoMoveCallable() {
    while (gameRunning) {
        // 每次移动都休息指定时间，
        util::sleep(moveInterval);
        // 如果游戏暂停了，就不继续，
        if (!pause) {
            if (enableAI) {
                // 如果启用了AI就让AI判断下一步的方向，
                decideNext();
            }
            // 移动蛇，
            moveSnake();
        }
    }
}

void SnakePresenter::exitGame() {
    gameRunning = false;
}

/**
 * 蛇移动方法，
 */
void SnakePresenter::moveSnake() {
    // 如果全地图都是蛇身则胜利，不继续移动蛇身，
    if (map->isAllBody()) {
        view->win();
        return;
    }
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
            view->lose();
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
        view->draw(map, &snake);
    }
}

/**
 * 当蛇吃到食物时调用，
 */
void SnakePresenter::onEatenFood() {
    // 加分，
    score += 1;
    // 修改视图上的
    view->setScore(score);
    // 产生新的食物，
    map->createRandFood();
}

void SnakePresenter::init() {
    initMap();
    initSnake();
    initAI();
    view->draw(map, &snake);
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
    // 如果要在地图添加额外的墙可以在这里添加，
    // 现在没有，
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
    snakeAI->init();
}

void SnakePresenter::move(Direction direction) {
    // 蛇头只能前进左右拐，不能后退，反正后退必死，
    if (direction != snake.getDirection()) {
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

void SnakePresenter::decideNext() {
    snakeAI->decideNext();
}

void SnakePresenter::pauseToggle() {
    pause = !pause;
}

