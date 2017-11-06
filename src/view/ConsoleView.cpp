//
// Created by AoEiuV020 on 2017.10.31-16:20:31.
//

#include "util/Console.h"
#include "util/util.h"
#include "view/ConsoleView.h"
#include "presenter/SnakePresenter.h"

void ConsoleView::printMsg(const std::string &msg) {
    int mapRowCnt = 0;
    if (map != nullptr) {
        mapRowCnt = (int) map->getRowCount();
    }
    Console::setCursor(0, mapRowCnt + 1);
    Console::writeWithColor(msg + "\n", ConsoleColor(WHITE, BLACK, true, false));
}

void ConsoleView::drawCallable() {
    try {
        while (gameRunning) {
            if (!drown) {
                drawMapContent();
                drown = true;
            }
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
    Console::write("Score: " + util::toString(score));
    Console::write("\n");
}

void ConsoleView::keyboardCallable() {
    try {
        while (gameRunning) {
            if (Console::kbhit()) {
                onKeyboardHit(Console::getch());
            }
            sleepFPS();
        }
    } catch (const std::exception &e) {
        presenter->exitGameErr(e.what());
    }
}

void ConsoleView::sleepFPS() {
    util::sleep((long) ((1.0 / fps) * 1000));
}

void ConsoleView::start() {
    gameRunning = true;
    Console::clear();
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

void ConsoleView::onKeyboardHit(char key) {
    static char cachedKey[3];

    // 方向键第一个字符，Esc,
    if (key == 27) {
        cachedKey[0] = key;
        cachedKey[1] = 0;
        cachedKey[2] = 0;
        return;
    }
    // 方向键第二个字符，91,
    if (cachedKey[0] == 27 && key == 91) {
        cachedKey[1] = key;
        cachedKey[2] = 0;
        return;
    }
    // 方向键第二个字符，65 - 68,
    if (cachedKey[0] == 27 && cachedKey[1] == 91
        && key >= 65 && key <= 68) {
        Direction d;
        switch (key) {
            case 65:
                d = Direction::UP;
                break;
            case 66:
                d = Direction::DOWN;
                break;
            case 68:
                d = Direction::LEFT;
                break;
            case 67:
                d = Direction::RIGHT;
                break;
        }
        keyboardMove(d);
        return;
    }
    switch (key) {
        case ' ':
            presenter->pauseToggle();
            break;
        case 'q':
            presenter->exitGame();
            break;
        default:
            break;
    }
}
