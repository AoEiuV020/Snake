//
// Created by AoEiuV020 on 2017.10.31-21:08:32.
//

#include "model/Pos.h"
#include "model/Point.h"
#include <stdexcept>
#include "model/Snake.h"
#include "util/util.h"
#include <queue>
#include "ai/SnakeAI.h"

void SnakeAI::setMap(Map *map_) {
    map = map_;
}

void SnakeAI::setSnake(Snake *snake_) {
    snake = snake_;
}

void SnakeAI::init() {
    if (map->getRowCount() % 2 == 1 && map->getColCount() % 2 == 1) {
        throw std::range_error("SnakeAI.init(): require even amount of rows or columns.");
    }
    buildHamilton();

    // 打印哈密顿回路走的顺序，
    int row = map->getRowCount(), col = map->getColCount();
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            const Point &point = map->getPoint(Pos(i, j));
            printf("%3d", point.getIndex());
        }
        printf("\n");
    }

}

void SnakeAI::buildHamilton() {
    // 得到从蛇头到蛇尾的最长路径，
    std::list<Direction> maxPath;
    findMaxPathToTail(maxPath);

    // 从蛇尾到蛇头标记顺序，
    Point::ValueType index = 0;
    for (auto it = snake->bodies.crbegin(); it != snake->bodies.crend(); ++it) {
        map->getPoint(*it).setIndex(index++);
    }
    // 沿途按顺序记录下来，
    int size = map->getSize();
    Pos cur = snake->getHead();
    for (const Direction d : maxPath) {
        Pos next = cur.getAdj(d);
        map->getPoint(next).setIndex((map->getPoint(cur).getIndex() + 1) % size);
        cur = next;
    }
}

void SnakeAI::decideNext() {
    int size = map->getSize();
    Pos head = snake->getHead(), tail = snake->getTail();
    Point::ValueType headIndex = map->getPoint(head).getIndex();
    // Move along the hamitonian cycle
    headIndex = map->getPoint(head).getIndex();
    std::vector<Pos> adjPositions = head.getAllAdj();
    for (const Pos &adjPos : adjPositions) {
        const Point &adjPoint = map->getPoint(adjPos);
        Point::ValueType adjIndex = adjPoint.getIndex();
        if (adjIndex == (headIndex + 1) % size) {
            snake->direc = head.getDirectionTo(adjPos);
        }
    }
}

void SnakeAI::findMaxPathToTail(std::list<Direction> &path) {
    const Pos &goal = snake->getTail();
    Point::Type oriType = map->getPoint(goal).getType();
    map->getPoint(goal).setType(Point::Type::EMPTY);
    findMaxPath(snake->getHead(), goal, path);
    map->getPoint(goal).setType(oriType);  // Retore point type
}

void SnakeAI::findMinPath(const Pos &from, const Pos &to, std::list<Direction> &path) {
    // Init
    int row = map->getRowCount(), col = map->getColCount();
    for (int i = 1; i < row - 1; ++i) {
        for (int j = 1; j < col - 1; ++j) {
            map->getPoint(Pos(i, j)).setDist(Point::MAX_VALUE);
        }
    }
    path.clear();
    map->getPoint(from).setDist(0);
    std::queue<Pos> openList;
    openList.push(from);
    // BFS
    while (!openList.empty()) {
        Pos curPos = openList.front();
        const Point &curPoint = map->getPoint(curPos);
        openList.pop();
        if (curPos == to) {
            buildPath(from, to, path);
            break;
        }
        std::vector<Pos> adjPositions = curPos.getAllAdj();
        util::Random<>::getInstance()->shuffle(adjPositions.begin(), adjPositions.end());
        // Arrange the order of traversing to make the result path as straight as possible
        Direction bestDirec = (curPos == from ? snake->direc : curPoint.getParent().getDirectionTo(curPos));
        for (int i = 0; i < (int) adjPositions.size(); ++i) {
            if (bestDirec == curPos.getDirectionTo(adjPositions[i])) {
                util::swap(adjPositions[0], adjPositions[i]);
                break;
            }
        }
        // Traverse the adjacent positions
        for (const Pos &adjPos : adjPositions) {
            Point &adjPoint = map->getPoint(adjPos);
            if (map->isEmpty(adjPos) && adjPoint.getDist() == Point::MAX_VALUE) {
                adjPoint.setParent(curPos);
                adjPoint.setDist(curPoint.getDist() + 1);
                openList.push(adjPos);
            }
        }
    }
}

void SnakeAI::findMaxPath(const Pos &from, const Pos &to, std::list<Direction> &path) {
    // Get the shortest path
    findMinPath(from, to, path);
    // Init
    int row = map->getRowCount(), col = map->getColCount();
    for (int i = 1; i < row - 1; ++i) {
        for (int j = 1; j < col - 1; ++j) {
            map->getPoint(Pos(i, j)).setVisit(false);
        }
    }
    // Make all points on the path visited
    Pos cur = from;
    for (const Direction d : path) {
        map->getPoint(cur).setVisit(true);
        cur = cur.getAdj(d);
    }
    map->getPoint(cur).setVisit(true);
    // Extend the path between each pair of the points
    for (auto it = path.begin(); it != path.end();) {
        if (it == path.begin()) {
            cur = from;
        }
        bool extended = false;
        Direction curDirec = *it;
        Pos next = cur.getAdj(curDirec);
        switch (curDirec) {
            case LEFT:
            case RIGHT: {
                Pos curUp = cur.getAdj(UP);
                Pos nextUp = next.getAdj(UP);
                // Check two points above
                if (map->isEmptyNotVisit(curUp) && map->isEmptyNotVisit(nextUp)) {
                    map->getPoint(curUp).setVisit(true);
                    map->getPoint(nextUp).setVisit(true);
                    it = path.erase(it);
                    it = path.insert(it, DOWN);
                    it = path.insert(it, curDirec);
                    it = path.insert(it, UP);
                    it = path.begin();
                    extended = true;
                } else {
                    Pos curDown = cur.getAdj(DOWN);
                    Pos nextDown = next.getAdj(DOWN);
                    // Check two points below
                    if (map->isEmptyNotVisit(curDown) && map->isEmptyNotVisit(nextDown)) {
                        map->getPoint(curDown).setVisit(true);
                        map->getPoint(nextDown).setVisit(true);
                        it = path.erase(it);
                        it = path.insert(it, UP);
                        it = path.insert(it, curDirec);
                        it = path.insert(it, DOWN);
                        it = path.begin();
                        extended = true;
                    }
                }
                break;
            }
            case UP:
            case DOWN: {
                Pos curLeft = cur.getAdj(LEFT);
                Pos nextLeft = next.getAdj(LEFT);
                // Check two points on the left
                if (map->isEmptyNotVisit(curLeft) && map->isEmptyNotVisit(nextLeft)) {
                    map->getPoint(curLeft).setVisit(true);
                    map->getPoint(nextLeft).setVisit(true);
                    it = path.erase(it);
                    it = path.insert(it, RIGHT);
                    it = path.insert(it, curDirec);
                    it = path.insert(it, LEFT);
                    it = path.begin();
                    extended = true;
                } else {
                    Pos curRight = cur.getAdj(RIGHT);
                    Pos nextRight = next.getAdj(RIGHT);
                    // Check two points on the right
                    if (map->isEmptyNotVisit(curRight) && map->isEmptyNotVisit(nextRight)) {
                        map->getPoint(curRight).setVisit(true);
                        map->getPoint(nextRight).setVisit(true);
                        it = path.erase(it);
                        it = path.insert(it, LEFT);
                        it = path.insert(it, curDirec);
                        it = path.insert(it, RIGHT);
                        it = path.begin();
                        extended = true;
                    }
                }
                break;
            }
            default:
                break;
        }
        if (!extended) {
            ++it;
            cur = next;
        }
    }
}

void SnakeAI::buildPath(const Pos &from, const Pos &to, std::list<Direction> &path) const {
    Pos tmp = to, parent;
    while (tmp != from) {
        parent = map->getPoint(tmp).getParent();
        path.push_front(parent.getDirectionTo(tmp));
        tmp = parent;
    }
}
