#include "Game.h"

int main(int, char **) {
    // 启动游戏相关的封装在Game类，
    auto game = new Game();
    // 启动游戏，
    return game->launch();
}
