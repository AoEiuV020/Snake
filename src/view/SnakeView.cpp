//
// Created by AoEiuV020 on 2017.10.31-16:03:20.
//

#include "view/SnakeView.h"
#include "presenter/SnakePresenter.h"
#include <string>

const std::string SnakeView::MSG_BAD_ALLOC = "Not enough memory to run the game.";
const std::string SnakeView::MSG_LOSE = "Oops! You lose!";
const std::string SnakeView::MSG_WIN = "Congratulations! You Win!";
const std::string SnakeView::MSG_EXIT = "Game ended.";

bool SnakeView::gameRunning;

void SnakeView::draw(Map *map_) {
    // 简单保存地图指针，
    // 考虑到线程安全，之后要改成深度复制，
    map = map_;
    drown = false;
}

void SnakeView::setPresenter(SnakePresenter *presenter_) {
    presenter = presenter_;
}

void SnakeView::init() {

}

void SnakeView::start() {
    gameRunning = true;
    presenter->run();
}

void SnakeView::stop() {
    gameRunning = false;
    presenter->exitGame();
}

void SnakeView::onScoreChanged(int score_) {
    score = score_;
}

void SnakeView::loop() {
     while(gameRunning) {
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
