//
// Created by AoEiuV020 on 2017.10.31-16:20:31.
//

#include <util/Console.h>
#include <util/util.h>
#include "view/ConsoleView.h"
#include "presenter/SnakePresenter.h"

void ConsoleView::printMsg(const std::string &msg) {
    int mapRowCnt = 0;
    if (map != nullptr) {
        mapRowCnt = (int) map->getRowCount();
    }
    Console::setCursor(0, mapRowCnt);
    Console::writeWithColor(msg + "\n", ConsoleColor(WHITE, BLACK, true, false));
}

void ConsoleView::drawCallable() {
    try {
        while (gameRunning) {
            drawMapContent();
            sleepFPS();
        }
    } catch (const std::exception &e) {
        presenter->exitGameErr(e.what());
    }
}

void ConsoleView::drawMapContent() {
    Console::setCursor();
    SizeType row = map->getRowCount(), col = map->getColCount();
    for (SizeType i = 0; i < row; ++i) {
        for (SizeType j = 0; j < col; ++j) {
            const Point &point = map->getPoint(Pos(i, j));
            switch (point.getType()) {
                case Point::Type::EMPTY:
                    Console::writeWithColor("  ", ConsoleColor(BLACK, BLACK));
                    break;
                case Point::Type::WALL:
                    Console::writeWithColor("  ", ConsoleColor(WHITE, WHITE, true, true));
                    break;
                case Point::Type::FOOD:
                    Console::writeWithColor("  ", ConsoleColor(YELLOW, YELLOW, true, true));
                    break;
                case Point::Type::SNAKE_HEAD:
                    Console::writeWithColor("  ", ConsoleColor(RED, RED, true, true));
                    break;
                case Point::Type::SNAKE_BODY:
                    Console::writeWithColor("  ", ConsoleColor(GREEN, GREEN, true, true));
                    break;
                case Point::Type::SNAKE_TAIL:
                    Console::writeWithColor("  ", ConsoleColor(BLUE, BLUE, true, true));
                    break;
                default:
                    break;
            }
        }
        Console::write("\n");
    }
}

void ConsoleView::keyboardCallable() {
    try {
        while (gameRunning) {
            if (Console::kbhit()) {
                switch (Console::getch()) {
                    case 'w':
                        keyboardMove(Direction::UP);
                        break;
                    case 'a':
                        keyboardMove(Direction::LEFT);
                        break;
                    case 's':
                        keyboardMove(Direction::DOWN);
                        break;
                    case 'd':
                        keyboardMove(Direction::RIGHT);
                        break;
                    case ' ':
                        presenter->pauseToggle();
                        break;
                    case 27:  // Esc
                        presenter->exitGame();
                        break;
                    default:
                        break;
                }
            }
        }
    } catch (const std::exception &e) {
        presenter->exitGameErr(e.what());
    }
}

void ConsoleView::sleepFPS() {
    util::sleep((long)((1.0 / fps) * 1000));
}

void ConsoleView::start() {
    gameRunning = true;
    drawThread = std::thread(&ConsoleView::drawCallable, this);
    drawThread.detach();
    keyboardThread = std::thread(&ConsoleView::keyboardCallable, this);
    keyboardThread.detach();
}

void ConsoleView::setFPS(double fps_) {
    fps = fps_;
}

void ConsoleView::stop() {
    gameRunning = false;
}

void ConsoleView::keyboardMove(Direction direction) {
    presenter->move(direction);
}
