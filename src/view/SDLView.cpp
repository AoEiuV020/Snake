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

    // 白底，
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    background = IMG_LoadTexture(renderer, FileUtil::subFile(resourceDir, "background.png").c_str());
    food = IMG_LoadTexture(renderer, FileUtil::subFile(resourceDir, "food.png").c_str());
    wall = IMG_LoadTexture(renderer, FileUtil::subFile(resourceDir, "block.jpg").c_str());
    headUp = IMG_LoadTexture(renderer, FileUtil::subFile(resourceDir, "head_up.png").c_str());
    headDown = IMG_LoadTexture(renderer, FileUtil::subFile(resourceDir, "head_down.png").c_str());
    headLeft = IMG_LoadTexture(renderer, FileUtil::subFile(resourceDir, "head_left.png").c_str());
    headRight = IMG_LoadTexture(renderer, FileUtil::subFile(resourceDir, "head_right.png").c_str());
    body = IMG_LoadTexture(renderer, FileUtil::subFile(resourceDir, "body.png").c_str());

    TTF_Init();
    font = TTF_OpenFont(FileUtil::subFile(resourceDir, "font.ttf").c_str(), 28);
}

void SDLView::destroy() {
    SDL_Log("destroy");

    TTF_CloseFont(font);

    SDL_DestroyTexture(background);
    SDL_DestroyTexture(food);
    SDL_DestroyTexture(wall);
    SDL_DestroyTexture(headUp);
    SDL_DestroyTexture(headDown);
    SDL_DestroyTexture(headLeft);
    SDL_DestroyTexture(headRight);
    SDL_DestroyTexture(body);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void SDLView::eventCallable() {
    SDL_Log("event thread start");
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                case SDL_QUIT:
                    exit();
                    // 关闭界面，
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    if (!gameRunning) {
                        // 游戏结束时再点击任意键关闭界面，
                        quit = true;
                        break;
                    }
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
    while (!quit) {
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
    SDL_RenderClear(renderer);

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

    // 显示分数，
    SDL_Surface *scoreSurface = TTF_RenderText_Solid(font, ("Score: " + util::toString(score)).c_str(), textColor);
    // 分数显示在右上角，
    SDL_Rect scoreRect = {400 - scoreSurface->w, 0, scoreSurface->w, scoreSurface->h};
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    SDL_FreeSurface(scoreSurface);
    SDL_RenderCopy(renderer, textTexture, nullptr, &scoreRect);

    if (!msg.empty()) {
        // 显示消息，胜利失败之类的，
        SDL_Surface *messageSurface = TTF_RenderText_Solid(font, msg.c_str(), textColor);
        // 消息显示在正中，
        SDL_Rect messageRect = {400 / 2 - messageSurface->w / 2, 400 / 2 - messageSurface->h / 2, messageSurface->w,
                                messageSurface->h};
        SDL_Texture *messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);
        SDL_FreeSurface(messageSurface);
        SDL_RenderCopy(renderer, messageTexture, nullptr, &messageRect);
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
    destroy();
}

void SDLView::message(std::string message) {
    SDL_Log("%s", message.c_str());
    msg = message;
    drown = false;
}
