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
    // 光标跳到地图下两行打印消息，
    // 控制台光标行列是0开始的，所以是+1，
    Console::setCursor(0, mapRowCnt + 1);
    Console::writeWithColor(msg + "\n", ConsoleColor(WHITE, BLACK, true, false));
}

void ConsoleView::drawCallable() {
    try {
        while (gameRunning) {
            if (!drown) {
                // 改标识避免重复绘图，
                drown = true;
                drawMapContent();
            }
            // 每次画完都让线程休息一下，为了满足刷新率，
            sleepFPS();
        }
    } catch (const std::exception &e) {
        presenter->exitGameErr(e.what());
    }
}

void ConsoleView::drawMapContent() {
    // 光标跳转到(0,0)即左上角，
    Console::setCursor(0, 0);
    // 简单的两层循环遍历地图每个点，
    // 一个一个点绘制,
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
        // 换行,
        Console::write("\n");
    }
    // 画完地图再展示得分，
    Console::write("Score: " + util::toString(score));
    Console::write("\n");
}

void ConsoleView::keyboardCallable() {
    try {
        while (gameRunning) {
            // getch方法阻塞，有输入时传给onKeyboardHit处理，
            onKeyboardHit(Console::getch());
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

void ConsoleView::onKeyboardHit(int key) {
    static int cachedKey[3];

    // Linux下的的方向键键键值是27, 91开头的三个字符，
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
    // 方向键第三个字符，65 - 68,
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
        // Windows下的的方向键键键值是224开头的两个字符，
        case 224:
            Direction d;
            d = Direction::NONE;
            switch (Console::getch()) {
                case 72:
                    d = Direction::UP;
                    break;
                case 80:
                    d = Direction::DOWN;
                    break;
                case 75:
                    d = Direction::LEFT;
                    break;
                case 77:
                    d = Direction::RIGHT;
                    break;
                default:
                    break;
            }
            keyboardMove(d);
            break;
        case 'w':
            keyboardMove(Direction::UP);
            break;
        case 's':
            keyboardMove(Direction::DOWN);
            break;
        case 'a':
            keyboardMove(Direction::LEFT);
            break;
        case 'd':
            keyboardMove(Direction::RIGHT);
            break;
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
