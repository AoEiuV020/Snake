//
// Created by AoEiuV020 on 2017.10.31-19:35:35.
//

#include "view/ViewWrapper.h"

void ViewWrapper::addView(SnakeView *view) {
    viewList.push_back(view);
}

void ViewWrapper::setPresenter(SnakePresenter *presenter_) {
    SnakeView::setPresenter(presenter_);
    for (auto &view : viewList) {
        view->setPresenter(presenter_);
    }
}

void ViewWrapper::draw(Map *map) {
    for (auto &view : viewList) {
        view->draw(map);
    }
}

void ViewWrapper::start() {
    for (auto &view : viewList) {
        view->start();
    }
}

void ViewWrapper::stop() {
    for (auto &view : viewList) {
        view->stop();
    }
}

void ViewWrapper::printMsg(const std::string &msg) {
    for (auto &view : viewList) {
        view->printMsg(msg);
    }
}

void ViewWrapper::onScoreChanged(int score) {
    for (auto &view : viewList) {
        view->onScoreChanged(score);
    }
}
