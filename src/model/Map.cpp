#include "model/Map.h"
#include "util/util.h"

using std::vector;
using std::string;
using std::list;

Map::Map(const int rowCnt_, const int colCnt_)
        : content(rowCnt_, vector<Point>(colCnt_)) {
    size = (rowCnt_ - 2) * (colCnt_ - 2);
    // Add boundary walls
    int row = getRowCount(), col = getColCount();
    for (int i = 0; i < row; ++i) {
        if (i == 0 || i == row - 1) {  // The first and last row
            for (int j = 0; j < col; ++j) {
                content[i][j].setType(Point::Type::WALL);
            }
        } else {  // Rows in the middle
            content[i][0].setType(Point::Type::WALL);
            content[i][col - 1].setType(Point::Type::WALL);
        }
    }
}

Map::~Map() {}

Point &Map::getPoint(const Pos &p) {
    return content[p.getX()][p.getY()];
}

const Point &Map::getPoint(const Pos &p) const {
    return content[p.getX()][p.getY()];
}

int Map::getRowCount() const {
    return content.size();
}

int Map::getColCount() const {
    return content[0].size();
}

int Map::getSize() const {
    return size;
}

bool Map::isInside(const Pos &p) const {
    return p.getX() > 0 && p.getY() > 0
           && p.getX() < getRowCount() - 1
           && p.getY() < getColCount() - 1;
}

bool Map::isEmpty(const Pos &p) const {
    return isInside(p) && getPoint(p).getType() == Point::Type::EMPTY;
}

bool Map::isEmptyNotVisit(const Pos &p) const {
    return isEmpty(p) && !getPoint(p).isVisit();
}

bool Map::isSafe(const Pos &p) const {
    const Point &point = getPoint(p);
    return isInside(p) && (point.getType() == Point::Type::EMPTY
                           || point.getType() == Point::Type::FOOD
                           || point.getType() == Point::Type::SNAKE_TAIL);
}

bool Map::isAllBody() const {
    int row = getRowCount(), col = getColCount();
    for (int i = 1; i < row - 1; ++i) {
        for (int j = 1; j < col - 1; ++j) {
            Point::Type type = content[i][j].getType();
            if (!(type == Point::Type::SNAKE_HEAD
                  || type == Point::Type::SNAKE_BODY
                  || type == Point::Type::SNAKE_TAIL)) {
                return false;
            }
        }
    }
    return true;
}

/**
 * 创建新的食物，
 */
void Map::createRandFood() {
    auto random = util::Random<>::getInstance();
    // 得到地图上所有空白的点的集合，
    vector<Pos> emptyPoints = getEmptyPoints();
    if (!emptyPoints.empty()) {
        // 随机取一个点，
        int i = random->nextInt((int) 0, (int) emptyPoints.size() - 1);
        // 将这个点设为食物，
        createFood(emptyPoints[i]);
    }
}

void Map::createFood(const Pos &pos) {
    food = pos;
    content[food.getX()][food.getY()].setType(Point::Type::FOOD);
}

void Map::removeFood() {
    if (hasFood()) {
        content[food.getX()][food.getY()].setType(Point::Type::EMPTY);
    }
}

bool Map::hasFood() const {
    return isInside(food) && getPoint(food).getType() == Point::Type::FOOD;
}

const Pos &Map::getFood() const {
    return food;
}

int Map::distance(const Pos &from, const Pos &to) {
    int fromX = from.getX(), toX = to.getX();
    int fromY = from.getY(), toY = to.getY();
    int dx = fromX > toX ? fromX - toX : toX - fromX;
    int dy = fromY > toY ? fromY - toY : toY - fromY;
    return dx + dy;
}

vector<Pos> Map::getEmptyPoints() const {
    vector<Pos> points;
    int row = getRowCount(), col = getColCount();
    for (int i = 1; i < row - 1; ++i) {
        for (int j = 1; j < col - 1; ++j) {
            if (content[i][j].getType() == Point::Type::EMPTY) {
                points.push_back(Pos(i, j));
            }
        }
    }
    return points;
}
