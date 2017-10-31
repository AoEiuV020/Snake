#include "Game.h"

int main() {
    auto game = Game::getInstance();
    return game->launch();
}
