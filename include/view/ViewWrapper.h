//
// Created by AoEiuV020 on 2017.10.31-19:35:35.
//

#ifndef SNAKE_VIEWWRAPPER_H
#define SNAKE_VIEWWRAPPER_H

#include "view/SnakeView.h"
#include <thread>

class SnakePresenter;

/**
 * 给view包装一层，以便同时启动多个view,
 * 不心要，
 */
class ViewWrapper : public SnakeView {
public:
    void addView(SnakeView *view);

    void draw(Map *map) override;

    void init(int, char **) override;

    void message(std::string message) override;

    void onStart() override;

    void onStop() override;

private:
    std::vector<SnakeView *> viewList;
};


#endif //SNAKE_VIEWWRAPPER_H
