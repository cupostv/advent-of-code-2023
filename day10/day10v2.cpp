#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <sstream>
#include <unordered_map>
#include <cmath>
#include <set>
#include <numeric>
#include <ranges>
#include <algorithm>
#include <list>
#include <queue>

#define INPUT "input.txt"

bool isAsciiDigit(char digit) {
    return digit >= '0' && digit <= '9';
}

void removeSpaces(std::string &str) {
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
}

using Point = std::pair<int64_t, int64_t>;

std::vector<std::string> pipeMap;
Point startingPosition;

#define TB '|'
#define LR '-'
#define TR 'L'
#define TL 'J'
#define BL '7'
#define BR 'F'
#define GROUND '.'
#define START 'S'
#define INVALID {-1, -1}

bool isWall(Point position) {
    auto[row, col] = position;
    if (row < 0 || row >= pipeMap.size()) return true;
    if (col < 0 || col >= pipeMap[0].size()) return true;
    return false;
}

enum Direction {
    LEFT = 0,
    RIGHT,
    TOP,
    BOTTOM,
};

struct Pipe {
    Point position;
    std::array<Point, 4> connections; // in and out

    Pipe(const Point position) : position(position) {
        char pipe = '.';
        if (!isWall(position))
            pipe = pipeMap[position.first][position.second];
        connections[Direction::LEFT] = INVALID;
        connections[Direction::RIGHT] = INVALID;
        connections[Direction::TOP] = INVALID;
        connections[Direction::BOTTOM] = INVALID;
        if (pipe == LR) {
            connections[Direction::LEFT] = {position.first, position.second - 1};
            connections[Direction::RIGHT] = {position.first, position.second + 1};
        }
        else if (pipe == TB) {
            connections[Direction::TOP] = {position.first - 1, position.second};
            connections[Direction::BOTTOM] = {position.first + 1, position.second};
        }
        else if (pipe == TR) {
            connections[Direction::TOP] = {position.first - 1, position.second};
            connections[Direction::RIGHT] = {position.first, position.second + 1};
        }
        else if (pipe == TL) {
            connections[Direction::TOP] = {position.first - 1, position.second};
            connections[Direction::LEFT] = {position.first, position.second - 1};
        }
        else if (pipe == BR) {
            connections[Direction::BOTTOM] = {position.first + 1, position.second};
            connections[Direction::RIGHT] = {position.first, position.second + 1};
        }
        else if (pipe == BL) {
            connections[Direction::BOTTOM] = {position.first + 1, position.second};
            connections[Direction::LEFT] = {position.first, position.second - 1};
        }
        else if(pipe == START) {
            connections[Direction::LEFT] = {position.first, position.second - 1};
            connections[Direction::RIGHT] = {position.first, position.second + 1};
            connections[Direction::TOP] = {position.first - 1, position.second};
            connections[Direction::BOTTOM] = {position.first + 1, position.second};
        }
    }

    bool isValid() {
        if (position.first >= 0 && position.first < pipeMap.size() &&
            position.second >= 0 && position.second < pipeMap[0].size()) {
                return true;
        }
        return false;
    }

    Pipe get(Direction dir) {
        return Pipe(connections[dir]);
    }
    Pipe getTop() {
        return get(Direction::TOP);
    }
    Pipe getBottom() {
        return get(Direction::BOTTOM);
    }
    Pipe getLeft() {
        return get(Direction::LEFT);
    }
    Pipe getRight() {
        return get(Direction::RIGHT);
    }

    // Get exit position for given entry position
    Point getExit(Point entry) {
        // Filter invalid and entry
        Point result = INVALID;
        int32_t count = 0;
        for (auto connection : connections) {
            if (connection != (Point)INVALID && connection != entry) {
                result = connection;
                count++;
            }
        }
        if (count > 1) {
            return INVALID;
        }

        return result;
    }

    void dump() {
        std::cout << "(" << position.first << ", " << position.second << ")" << std::endl;
        std::cout << "Left: " << connections[Direction::LEFT].first << ", " <<  connections[Direction::LEFT].second << std::endl;
        std::cout << "Right: " << connections[Direction::RIGHT].first << ", " <<  connections[Direction::RIGHT].second << std::endl;
        std::cout << "Top: " << connections[Direction::TOP].first << ", " <<  connections[Direction::TOP].second << std::endl;
        std::cout << "Bottom: " << connections[Direction::BOTTOM].first << ", " <<  connections[Direction::BOTTOM].second << std::endl;
    }

};

std::pair<std::vector<Point>, bool> getPath(Pipe pipe) {
    std::vector<Point> result = {startingPosition};

    auto entryPos = startingPosition;
    auto exitPos = pipe.getExit(entryPos);
    entryPos = pipe.position;
    result.push_back(exitPos);

    while(true) {
        if (exitPos == startingPosition) break;
        if (exitPos == (Point)INVALID) break;

        Pipe exitPipe(exitPos);
        auto temp = exitPipe.getExit(entryPos);
        entryPos = exitPos;
        exitPos = temp;
        result.push_back(exitPos);
    }

    return {result, exitPos == startingPosition};
}

std::vector<Point> getValidPath() {
    Pipe start(startingPosition);

    auto [lPath, lValid] = getPath(start.getLeft());
    auto [rPath, rValid] = getPath(start.getRight());
    auto [tPath, tValid] = getPath(start.getTop());
    auto [bPath, bValid] = getPath(start.getBottom());

    if (tValid && bValid) pipeMap[startingPosition.first][startingPosition.second] = TB;
    if (tValid && lValid) pipeMap[startingPosition.first][startingPosition.second] = TL;
    if (tValid && rValid) pipeMap[startingPosition.first][startingPosition.second] = TR;
    if (lValid && rValid) pipeMap[startingPosition.first][startingPosition.second] = LR;
    if (bValid && lValid) pipeMap[startingPosition.first][startingPosition.second] = BL;
    if (bValid && rValid) pipeMap[startingPosition.first][startingPosition.second] = BR;
    // std::cout << pipeMap[startingPosition.first][startingPosition.second];
    if (lValid) return lPath;
    if (rValid) return rPath;
    if (tValid) return tPath;
    if (bValid) return bPath;

    return {};
}

struct Polygon {
    std::vector<Point> points;

    double shoelace() const {
        double area = 0;

        // Calculate value of shoelace formula
        int64_t j = points.size() - 1;
        for (int64_t i = 0; i < points.size(); i++)
        {
            area += (points[j].first + points[i].first) * (points[j].second - points[i].second);
            j = i;
        }

        return std::abs(area / 2);
    }

    int64_t perimeter() const {
        int64_t perimeter = 0;
        auto prev = points[0];

        for (int64_t i = 1; i < points.size(); i++) {
            perimeter += std::abs(prev.first - points[i].first) + std::abs(prev.second - points[i].second);
            prev = points[i];
        }
        return perimeter;
    }

    double area() const {
        return shoelace() + perimeter() / 2 + 1;
    }

    double areaInside() const {
        return shoelace() - perimeter() / 2 + 1;
    }
};

int32_t calculateAreaInside(const std::vector<Point> &path) {
    // Construct polygon
    Polygon polygon;

    polygon.points = path;

    return polygon.areaInside();
}

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    std::vector<std::vector<Pipe>> pipes;

    while (!input.eof()) {

        std::string inputRow;
        std::getline(input, inputRow);
        if (inputRow.empty()) continue;

        // std::stringstream inputStream(inputRow);

        if (inputRow.find(START) != std::string::npos) {
            startingPosition = {pipeMap.size(), inputRow.find(START)};
        }
        pipeMap.push_back(inputRow);
    }

    std::vector<Point> path = getValidPath();

    std::cout << path.size() / 2 << std::endl;

    std::cout << calculateAreaInside(path) << std::endl;

    return EXIT_SUCCESS;
}