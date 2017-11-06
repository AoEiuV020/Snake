//
// Created by AoEiuV020 on 2017.10.31-19:32:13.
//

#ifndef SNAKE_FILEVIEW_H
#define SNAKE_FILEVIEW_H

#include "view/SnakeView.h"
#include <thread>

class SnakePresenter;

/**
 * 以文件形式保存游戏过程，
 * 不心要，
 */
class FileView : public SnakeView {
public:
    ~FileView();

    void draw(Map *map) override;

    void start() override;

private:
    static const std::string MAP_INFO_FILENAME;

    FILE *movementFile = nullptr;  // File to save snake movements

    void writeMapToFile();

    void initFiles();
};


#endif //SNAKE_FILEVIEW_H
