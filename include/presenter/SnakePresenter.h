//
// Created by AoEiuV020 on 2017.10.31-15:41:44.
//

#ifndef SNAKE_SNAKEPRESENTER_H
#define SNAKE_SNAKEPRESENTER_H

#include "model/Snake.h"
#include "util/Console.h"
#include <thread>
#include <mutex>

class SnakeView;

/*
Game controller.
*/
class SnakePresenter {
public:
    typedef Map::SizeType SizeType;

    static const Point::ValueType EMPTY_VALUE = 99999;

    ~SnakePresenter();

    /*
    Return the only instance.
    */
    static SnakePresenter *getInstance();

    void attach(SnakeView *view_);

    void setEnableHamilton(const bool enableHamilton_);

    void setMapRow(const SizeType n);

    void setMapCol(const SizeType n);

    /*
    Run the game.

    @return The exit status of the program.
    */
    void run();

    /*
    Print an error message and exit the game.
    */
    void exitGameErr(const std::string &err);

    /**
     * 正常退出游戏，
     */
    void exitGame();

    /*
    Initialize.
    */
    void init();

    int getExitCode();

    void setDirection(Direction direction);

    /*
    Move the snake and check if the game is over.
    */
    void moveSnake();

    void decideNext();

    void move(Direction direction);

private:
    static const std::string MSG_BAD_ALLOC;
    static const std::string MSG_LOSE;
    static const std::string MSG_WIN;
    static const std::string MSG_ESC;

    SnakeView *view;

    bool enableHamilton = true;
    bool recordMovements = true;
    SizeType mapRowCnt = 10;
    SizeType mapColCnt = 10;

    Map *map = nullptr;
    Snake snake;

    volatile bool runMainThread = true;  // Switch of the main thread

    std::mutex mutexMove;  // Mutex of moveSnake()
    std::mutex mutexExit;  // Mutex of exitGame()

    SnakePresenter();

    /*
    Print a message and exit the game.
    */
    void exitGame(const std::string &msg);

    /*
    Print a message to the terminal.
    */
    void printMsg(const std::string &msg);

    void initMap();

    void initSnake();

    int exitCode;
    SnakeAI *snakeAI;

    void initAI();
};


#endif //SNAKE_SNAKEPRESENTER_H
