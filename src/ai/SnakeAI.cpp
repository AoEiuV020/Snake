//
// Created by AoEiuV020 on 2017.10.31-21:08:32.
//

#include "model/Pos.h"
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
}

void SnakeAI::buildHamilton() {
    directionMap = std::vector<std::vector<Direction>>(map->getRowCount(), std::vector<Direction>(map->getColCount()));
    visitMap = std::vector<std::vector<bool>>(map->getRowCount(), std::vector<bool>(map->getColCount()));
    distanceMap = std::vector<std::vector<uint32_t >>(map->getRowCount(), std::vector<uint32_t>(map->getColCount()));
    parentMap = std::vector<std::vector<Pos>>(map->getRowCount(), std::vector<Pos>(map->getColCount()));
    // 得到从蛇头到蛇尾的最长路径，
    std::list<Direction> maxPath;
    findMaxPathToTail(maxPath);

    // 沿途记录每个点的位置，
    for (auto it = snake->bodies.crbegin(); it != snake->bodies.crend(); ++it) {
        directionMap[it->getX()][it->getY()] = RIGHT;
    }
    Pos cur = snake->getHead();
    for (const Direction d : maxPath) {
        Pos next = cur.getAdj(d);
        directionMap[cur.getX()][cur.getY()] = d;
        cur = next;
    }
}

void SnakeAI::decideNext() {
    Pos head = snake->getHead(), tail = snake->getTail();
    snake->setDirection(directionMap[head.getX()][head.getY()]);
}

void SnakeAI::findMaxPathToTail(std::list<Direction> &path) {
    const Pos &goal = snake->getTail();
    findMaxPath(snake->getHead(), goal, path);
}

void SnakeAI::findMinPath(const Pos &from, const Pos &to, std::list<Direction> &path) {
    // Init
    int row = map->getRowCount(), col = map->getColCount();
    for (int i = 1; i < row - 1; ++i) {
        for (int j = 1; j < col - 1; ++j) {
            distanceMap[i][j] = UINT32_MAX;
        }
    }
    path.clear();
    distanceMap[from.getX()][from.getY()] = 0;
    std::queue<Pos> openList;
    openList.push(from);
    // BFS
    while (!openList.empty()) {
        Pos curPos = openList.front();
        openList.pop();
        if (curPos == to) {
            buildPath(from, to, path);
            break;
        }
        std::vector<Pos> adjPositions = curPos.getAllAdj();
        util::Random<>::getInstance()->shuffle(adjPositions.begin(), adjPositions.end());
        // Arrange the order of traversing to make the result path as straight as possible
        Direction bestDirec = (curPos == from ? snake->direc : parentMap[curPos.getX()][curPos.getY()].getDirectionTo(
                curPos));
        for (int i = 0; i < (int) adjPositions.size(); ++i) {
            if (bestDirec == curPos.getDirectionTo(adjPositions[i])) {
                util::swap(adjPositions[0], adjPositions[i]);
                break;
            }
        }
        // Traverse the adjacent positions
        for (const Pos &adjPos : adjPositions) {
            if (map->isSafe(adjPos) && distanceMap[adjPos.getX()][adjPos.getY()] == UINT32_MAX) {
                parentMap[adjPos.getX()][adjPos.getY()] = curPos;
                distanceMap[adjPos.getX()][adjPos.getY()] = distanceMap[curPos.getX()][curPos.getY()] + 1;
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
            visitMap[i][j] = false;
        }
    }
    // Make all points on the path visited
    Pos cur = from;
    for (const Direction d : path) {
        visitMap[cur.getX()][cur.getY()] = true;
        cur = cur.getAdj(d);
    }
    visitMap[cur.getX()][cur.getY()] = true;
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
                if (map->isSafe(curUp) && !visitMap[curUp.getX()][curUp.getY()]
                    && map->isSafe(nextUp) && !visitMap[nextUp.getX()][nextUp.getY()]) {
                    visitMap[curUp.getX()][curUp.getY()] = true;
                    visitMap[nextUp.getX()][nextUp.getY()] = true;
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
                    if (map->isSafe(curDown) && !visitMap[curDown.getX()][curDown.getY()]
                        && map->isSafe(nextDown) && !visitMap[nextDown.getX()][nextDown.getY()]) {
                        visitMap[curDown.getX()][curDown.getY()] = true;
                        visitMap[nextDown.getX()][nextDown.getY()] = true;
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
                if (map->isSafe(curLeft) && !visitMap[curLeft.getX()][curLeft.getY()]
                    && map->isSafe(nextLeft) && !visitMap[nextLeft.getX()][nextLeft.getY()]) {
                    visitMap[curLeft.getX()][curLeft.getY()] = true;
                    visitMap[nextLeft.getX()][nextLeft.getY()] = true;
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
                    if (map->isSafe(curRight) && !visitMap[curRight.getX()][curRight.getY()]
                        && map->isSafe(nextRight) && !visitMap[nextRight.getX()][nextRight.getY()]) {
                        visitMap[curRight.getX()][curRight.getY()] = true;
                        visitMap[nextRight.getX()][nextRight.getY()] = true;
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
        parent = parentMap[tmp.getX()][tmp.getY()];
        path.push_front(parent.getDirectionTo(tmp));
        tmp = parent;
    }
}
