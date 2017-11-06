//
// Created by AoEiuV020 on 2017.10.31-16:03:20.
//

#include "view/SnakeView.h"
#include "presenter/SnakePresenter.h"

void SnakeView::draw(Map *map_) {
    // 简单保存地图指针，
    // 考虑到线程安全，之后要改成深度复制，
    map = map_;
    drown = false;
}

void SnakeView::setPresenter(SnakePresenter *presenter_) {
    presenter = presenter_;
}

void SnakeView::start() {

}

void SnakeView::stop() {

}

void SnakeView::printMsg(const std::string &msg) {
    // 无意义语句，只为避免一个警告，
    (void)msg;
}

void SnakeView::onScoreChanged(int score_) {
    score = score_;
}
