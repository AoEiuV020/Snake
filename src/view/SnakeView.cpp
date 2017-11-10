//
// Created by AoEiuV020 on 2017.10.31-16:03:20.
//

#include "view/SnakeView.h"
#include "presenter/SnakePresenter.h"

const std::string SnakeView::MSG_LOSE = "Oops! You lose!";
const std::string SnakeView::MSG_WIN = "Congratulations! You Win!";
const std::string SnakeView::MSG_EXIT = "Game ended.";
SnakePresenter *SnakeView::presenter = nullptr;

bool SnakeView::gameRunning;

/**
 * 绘图，
 * 不是马上绘图，先保存这个地图和蛇,并标记有地图需要绘制，
 */
void SnakeView::draw(Map *map_, Snake *snake_) {
    // 简单保存地图指针，
    // 考虑到线程安全，之后要改成深度复制，
    map = map_;
    snake = snake_;
    drown = false;
}

void SnakeView::setPresenter(SnakePresenter *presenter_) {
    presenter = presenter_;
}

void SnakeView::init(int, char **) {
    presenter->init();
}

void SnakeView::start() {
    gameRunning = true;
    presenter->run();
    onStart();
}

void SnakeView::onStart() {

}

void SnakeView::stop() {
    gameRunning = false;
    presenter->exitGame();
    onStop();
}

void SnakeView::onStop() {

}

void SnakeView::setScore(int score_) {
    score = score_;
}

void SnakeView::loop() {
    while (gameRunning) {
    }
}

void SnakeView::win() {
    message(MSG_WIN);
    stop();
}

void SnakeView::lose() {
    message(MSG_LOSE);
    stop();
}

void SnakeView::exit() {
    message(MSG_EXIT);
    stop();
}

void SnakeView::message(const std::string) {
}
