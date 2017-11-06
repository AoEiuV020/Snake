#include "Game.h"

int main() {
    // 启动游戏相关的封装在Game类，
    // getInstance是单例模式常用做法，确保Game只有一个对象，
    auto game = Game::getInstance();
    // 启动游戏，
    return game->launch();
}
