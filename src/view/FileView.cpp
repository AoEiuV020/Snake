//
// Created by AoEiuV020 on 2017.10.31-19:32:13.
//

#include <cstdio>
#include "view/FileView.h"

const std::string FileView::MAP_INFO_FILENAME = "movements.txt";

FileView::~FileView() {
    if (movementFile) {
        fclose(movementFile);
        movementFile = nullptr;
    }
}

void FileView::draw(Map *map) {
    SnakeView::draw(map);
    writeMapToFile();
}

void FileView::writeMapToFile() {
    if (!movementFile) {
        return;
    }
    int rows = map->getRowCount(), cols = map->getColCount();
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            switch (map->getPoint(Pos(i, j)).getType()) {
                case Point::Type::EMPTY:
                    fwrite("  ", sizeof(char), 2, movementFile);
                    break;
                case Point::Type::WALL:
                    fwrite("# ", sizeof(char), 2, movementFile);
                    break;
                case Point::Type::FOOD:
                    fwrite("F ", sizeof(char), 2, movementFile);
                    break;
                case Point::Type::SNAKE_HEAD:
                    fwrite("H ", sizeof(char), 2, movementFile);
                    break;
                case Point::Type::SNAKE_BODY:
                    fwrite("B ", sizeof(char), 2, movementFile);
                    break;
                case Point::Type::SNAKE_TAIL:
                    fwrite("T ", sizeof(char), 2, movementFile);
                    break;
                default:
                    break;
            }
        }
        fwrite("\n", sizeof(char), 1, movementFile);
    }
    fwrite("\n", sizeof(char), 1, movementFile);
}

void FileView::init(int, char **) {
    initFiles();
}

void FileView::initFiles() {
    movementFile = fopen(MAP_INFO_FILENAME.c_str(), "w");
    if (!movementFile) {
        throw std::runtime_error("FileView.initFiles(): Fail to open file: " + MAP_INFO_FILENAME);
    } else {
        // Write content description to the file
        std::string str = "Content description:\n";
        str += "#: wall\nH: snake head\nB: snake body\nT: snake tail\nF: food\n\n";
        str += "Movements:\n\n";
        fwrite(str.c_str(), sizeof(char), str.length(), movementFile);
    }
}
