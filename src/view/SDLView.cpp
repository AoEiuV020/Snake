//
// Created by AoEiuV020 on 2017.11.08-14:28:33.
//

#include "view/SDLView.h"
#include "presenter/SnakePresenter.h"
#include <util/util.h>
#include <util/FileUtil.h>
#include <SDL2/SDL_image.h>

void SDLView::onStart() {
    SDL_Log("onStart");
    eventThread = std::thread(&SDLView::eventCallable, this);
    eventThread.detach();
}

void SDLView::initSDL() {
    SDL_Log("initSDL");
    const int SCREEN_WIDTH = 400;
    const int SCREEN_HEIGHT = 400;

    //Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    //Create window
    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                              SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    background = IMG_LoadTexture(renderer, FileUtil::subFile(resourceDir, "background.png").c_str());
    food = IMG_LoadTexture(renderer, FileUtil::subFile(resourceDir, "food.png").c_str());
    wall = IMG_LoadTexture(renderer, FileUtil::subFile(resourceDir, "block.jpg").c_str());
    headUp = IMG_LoadTexture(renderer, FileUtil::subFile(resourceDir, "head_up.png").c_str());
    headDown = IMG_LoadTexture(renderer, FileUtil::subFile(resourceDir, "head_down.png").c_str());
    headLeft = IMG_LoadTexture(renderer, FileUtil::subFile(resourceDir, "head_left.png").c_str());
    headRight = IMG_LoadTexture(renderer, FileUtil::subFile(resourceDir, "head_right.png").c_str());
    body = IMG_LoadTexture(renderer, FileUtil::subFile(resourceDir, "body.png").c_str());
}

void SDLView::onStop() {
    SnakeView::stop();
    SDL_Log("onStop");
    SDL_Quit();
}

void SDLView::eventCallable() {
    SDL_Log("event thread start");
    SDL_Event event;
    while (gameRunning) {
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                case SDL_QUIT:
                    exit();
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:
                        case SDLK_w:
                            keyboardMove(UP);
                            break;

                        case SDLK_DOWN:
                        case SDLK_s:
                            keyboardMove(DOWN);
                            break;

                        case SDLK_LEFT:
                        case SDLK_a:
                            keyboardMove(LEFT);
                            break;

                        case SDLK_RIGHT:
                        case SDLK_d:
                            keyboardMove(RIGHT);
                            break;
                        case SDLK_q:
                            exit();
                            break;
                        case SDLK_SPACE:
                            presenter->pauseToggle();
                            break;

                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }
        }
    }
}

void SDLView::drawCallable() {
    SDL_Log("draw thread start");
    while (gameRunning) {
        if (!drown) {
            // 改标识避免重复绘图，
            drown = true;
            drawMapContent();
        }
        // 每次画完都让线程休息一下，为了满足刷新率，
        sleepFPS();
    }
}

void SDLView::keyboardMove(Direction direction) {
    presenter->move(direction);
}

void SDLView::sleepFPS() {
    SDL_Delay((Uint32) ((1.0 / fps) * 1000));
}

void SDLView::drawMapContent() {
    int row = map->getRowCount(), col = map->getColCount();
    int width = 400 / col, height = 400 / row;
    int x = 0, y = 0;
    SDL_Rect r = {};
    r.w = width;
    r.h = height;
    for (int i = 0; i < row; ++i, y += height) {
        x = 0;
        for (int j = 0; j < col; ++j, x += width) {
            r.x = x;
            r.y = y;
            const Point &point = map->getPoint(Pos(i, j));
            // 刷上背景，
            SDL_RenderCopy(renderer, background, nullptr, &r);
            SDL_Texture *texture = nullptr;
            switch (point.getType()) {
                case Point::Type::EMPTY:
                    texture = background;
                    break;
                case Point::Type::WALL:
                    texture = wall;
                    break;
                case Point::Type::FOOD:
                    texture = food;
                    break;
                case Point::Type::SNAKE_HEAD:
                    switch (snake->getDirection()) {
                        case UP:
                            texture = headUp;
                            break;
                        default:
                        case DOWN:
                            texture = headDown;
                            break;
                        case LEFT:
                            texture = headLeft;
                            break;
                        case RIGHT:
                            texture = headRight;
                            break;
                    }
                    break;
                case Point::Type::SNAKE_BODY:
                case Point::Type::SNAKE_TAIL:
                    texture = body;
                    break;
                default:
                    break;
            }
            if (texture != nullptr) {
                // 刷上方块，
                SDL_RenderCopy(renderer, texture, nullptr, &r);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

void SDLView::init(int, char **argv) {
    std::string bin = argv[0];
    resourceDir = FileUtil::subFile(FileUtil::parent(bin), "res");
    initSDL();
}

void SDLView::loop() {
    drawCallable();
}
