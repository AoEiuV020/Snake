//
// Created by AoEiuV020 on 2017.11.08-14:28:33.
//

#ifndef __WIN32

#include "view/SDLView.h"
#include "presenter/SnakePresenter.h"
#include <util/util.h>
#include <util/FileUtil.h>
#include <SDL2/SDL_image.h>

/**
 * SDL开始绘图，
 * 游戏开始时调用，
 * 启动监听SDL事件的线程，包括键盘操作和关闭窗口，
 */
void SDLView::onStart() {
    SDL_Log("onStart");
    eventThread = std::thread(&SDLView::eventCallable, this);
    eventThread.detach();
}

/**
 * 初始化配置图形库SDL,
 */
void SDLView::initSDL() {
    SDL_Log("initSDL");
    const int SCREEN_WIDTH = 400;
    const int SCREEN_HEIGHT = 400;

    SDL_Init(SDL_INIT_VIDEO);
    // 创建窗口，
    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                              SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    // 创建渲染器，
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // 渲染器涂成白底，
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    // 加载资源，各种贴图，
    // 背景，食物，墙，身体，四个方向的蛇头，
    background = IMG_LoadTexture(renderer, FileUtil::subFile(resourceDir, "background.png").c_str());
    food = IMG_LoadTexture(renderer, FileUtil::subFile(resourceDir, "food.png").c_str());
    wall = IMG_LoadTexture(renderer, FileUtil::subFile(resourceDir, "block.jpg").c_str());
    body = IMG_LoadTexture(renderer, FileUtil::subFile(resourceDir, "body.png").c_str());
    headUp = IMG_LoadTexture(renderer, FileUtil::subFile(resourceDir, "head_up.png").c_str());
    headDown = IMG_LoadTexture(renderer, FileUtil::subFile(resourceDir, "head_down.png").c_str());
    headLeft = IMG_LoadTexture(renderer, FileUtil::subFile(resourceDir, "head_left.png").c_str());
    headRight = IMG_LoadTexture(renderer, FileUtil::subFile(resourceDir, "head_right.png").c_str());

    // 初始化字体，
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

/**
 * 供SDL事件线程调用的方法，
 */
void SDLView::eventCallable() {
    SDL_Log("event thread start");
    SDL_Event event;
    // 在退出前始终循环接收消息，
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            // 判断事件类型，
            switch (event.type) {
                case SDL_QUIT:
                    // 如果是点击窗口关闭按钮，
                    exit();
                    // 退出循环，关闭界面，
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    // 如果是键盘按键按下，
                    if (!gameRunning) {
                        // 游戏结束时再点击任意键关闭界面，
                        quit = true;
                        break;
                    }
                    // 判断按下什么键，
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:
                        case SDLK_w:
                            // 移动，上，
                            keyboardMove(UP);
                            break;

                        case SDLK_DOWN:
                        case SDLK_s:
                            // 移动，下，
                            keyboardMove(DOWN);
                            break;

                        case SDLK_LEFT:
                        case SDLK_a:
                            // 移动，左，
                            keyboardMove(LEFT);
                            break;

                        case SDLK_RIGHT:
                        case SDLK_d:
                            // 移动，右，
                            keyboardMove(RIGHT);
                            break;
                        case SDLK_q:
                            // 主动结束游戏，
                            exit();
                            break;
                        case SDLK_SPACE:
                            // 游戏暂停，
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
        // 休息一下，避免死循环占用太多cpu资源，
        SDL_Delay(10);
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

/**
 * 实际的SDL绘图方法，
 */
void SDLView::drawMapContent() {
    // 先清空，
    SDL_RenderClear(renderer);

    // 得到地图的行列数，
    int row = map->getRowCount(), col = map->getColCount();
    int width = 400 / col, height = 400 / row;
    int x = 0, y = 0;
    SDL_Rect r;
    r.w = width;
    r.h = height;
    // 一个方块，从左到右，从上到下移动，一个个块贴上图片，
    for (int i = 0; i < row; ++i, y += height) {
        x = 0;
        r.y = y;
        for (int j = 0; j < col; ++j, x += width) {
            r.x = x;
            const Point &point = map->getPoint(Pos(i, j));
            // 先刷背景再刷前景，
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

    // 更新，
    SDL_RenderPresent(renderer);
}

void SDLView::init(int, char **argv) {
    std::string bin = argv[0];
    resourceDir = FileUtil::subFile(FileUtil::parent(bin), "res");
    initSDL();
    setting();
}

/**
 * 设置是否启用AI,
 */
void SDLView::setting() const {
    showMessage("Enable AI? y/n");

    SDL_Event event;
    bool flag = false;
    while (!flag) {
        while (SDL_PollEvent(&event) != 0) {
            // 判断事件，键盘按键y或n输入才有效，
            switch (event.type) {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_y:
                            // 启用AI并设置高速，
                            presenter->setEnableAI(true);
                            presenter->setMoveInterval(30);
                            flag = true;
                            break;
                        case SDLK_n:
                            // 禁用AI并设置低速，
                            presenter->setEnableAI(false);
                            presenter->setMoveInterval(150);
                            flag = true;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        // 休息一下，避免死循环占用太多cpu资源，
        SDL_Delay(10);
    }

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    showMessage("Size: 2 * (3 + ?)");

    flag = false;
    while (!flag) {
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_0:
                        case SDLK_1:
                        case SDLK_2:
                        case SDLK_3:
                        case SDLK_4:
                        case SDLK_5:
                        case SDLK_6:
                        case SDLK_7:
                        case SDLK_8:
                        case SDLK_9:
                            int input = event.key.keysym.sym - '0';
                            int size = 2 * (3 + input);
                            presenter->setMapRow(size);
                            presenter->setMapCol(size);
                            // 游戏初始化，
                            presenter->init();
                            flag = true;
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        // 休息一下，避免死循环占用太多cpu资源，
        SDL_Delay(10);
    }
}

void SDLView::showMessage(const char *message) const {
    SDL_Surface *messageSurface = TTF_RenderText_Solid(font, message, textColor);
    // 消息显示在正中，
    SDL_Rect messageRect = {400 / 2 - messageSurface->w / 2, 400 / 2 - messageSurface->h / 2, messageSurface->w,
                            messageSurface->h};
    SDL_Texture *messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);
    SDL_FreeSurface(messageSurface);
    SDL_RenderCopy(renderer, messageTexture, nullptr, &messageRect);
    // 更新，
    SDL_RenderPresent(renderer);
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

#endif // __WIN32
