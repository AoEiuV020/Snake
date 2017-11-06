//
// Created by AoEiuV020 on 2017.10.31-16:20:31.
//

#ifndef SNAKE_CONSOLEVIEW_H
#define SNAKE_CONSOLEVIEW_H

#include "view/SnakeView.h"
#include <thread>

class SnakePresenter;

class ConsoleView : public SnakeView {

public:
    void start() override;

    void stop() override;

    void printMsg(const std::string &msg) override;

    void setFPS(double fps_);

private:
    void drawCallable();

    volatile bool gameRunning = false;   // Switch of sub-threads

    void drawMapContent();

    void sleepFPS();

    double fps = 60.0;

    std::thread drawThread;
    std::thread keyboardThread;  // Thread to receive keyboard instructions
    void keyboardCallable();

    void keyboardMove(Direction direction);

    void onKeyboardHit(int key);
};


#endif //SNAKE_CONSOLEVIEW_H
