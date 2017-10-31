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

    void setMoveInterval(int moveInterval_);

    void setEnableAI(bool enableAI_);

private:
    void drawCallable();

    volatile bool runSubThread = false;   // Switch of sub-threads

    void drawMapContent();

    void sleepFPS();

    double fps = 60.0;

    std::thread drawThread;
    std::thread keyboardThread;  // Thread to receive keyboard instructions
    std::thread moveThread;      // Thread to move the snake
    void keyboardCallable();

    void autoMoveCallable();

    long moveInterval = 30;
    volatile bool pause = false;  // Control pause/resume game
    bool enableAI;

    void keyboardMove(Direction direction);
};


#endif //SNAKE_CONSOLEVIEW_H
