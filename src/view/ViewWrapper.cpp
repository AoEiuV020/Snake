//
// Created by AoEiuV020 on 2017.10.31-19:35:35.
//

#include "view/ViewWrapper.h"

void ViewWrapper::addView(SnakeView *view) {
    viewList.push_back(view);
}

void ViewWrapper::draw(Map *map) {
    for (auto &view : viewList) {
        view->draw(map);
    }
}

void ViewWrapper::init() {
    for (auto &view : viewList) {
        view->init();
    }
}

void ViewWrapper::onStart() {
    for (auto &view : viewList) {
        view->onStart();
    }
}

void ViewWrapper::onStop() {
    for (auto &view : viewList) {
        view->onStop();
    }
}

void ViewWrapper::message(std::string message) {
    for (auto &view : viewList) {
        view->message(message);
    }
}
