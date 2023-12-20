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

#define INPUT "input.txt"

bool isAsciiDigit(char digit) {
    return digit >= '0' && digit <= '9';
}

void removeSpaces(std::string &str) {
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
}

std::vector<std::string> pipeMap;
std::pair<size_t, size_t> startingPosition;

#define TB '|'
#define LR '-'
#define TR 'L'
#define TL 'J'
#define BL '7'
#define BR 'F'
#define GROUND '.'
#define START 'S'
#define INVALID {-1, -1}

enum class Direction {
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
};

// | is a vertical pipe connecting north and south.
// - is a horizontal pipe connecting east and west.
// L is a 90-degree bend connecting north and east.
// J is a 90-degree bend connecting north and west.
// 7 is a 90-degree bend connecting south and west.
// F is a 90-degree bend connecting south and east.
// . is ground; there is no pipe in this tile.
// S is the starting position of the animal; there is a pipe on this tile, but your sketch doesn't show what shape the pipe has.

/*
    Try few combinations from every point:
    T
   LXR     RB RL TB TL
    B

*/

Direction getExit(Direction from, char pipe) {
    if (from == Direction::RIGHT && pipe == LR) return Direction::RIGHT;
    if (from == Direction::RIGHT && pipe == TL) return Direction::TOP;
    if (from == Direction::RIGHT && pipe == BL) return Direction::BOTTOM;

    if (from == Direction::LEFT && pipe == LR) return Direction::LEFT;
    if (from == Direction::LEFT && pipe == TR) return Direction::TOP;
    if (from == Direction::LEFT && pipe == BR) return Direction::BOTTOM;

    if (from == Direction::TOP && pipe == TB) return Direction::TOP;
    if (from == Direction::TOP && pipe == BR) return Direction::RIGHT;
    if (from == Direction::TOP && pipe == BL) return Direction::LEFT;

    if (from == Direction::BOTTOM && pipe == TB) return Direction::BOTTOM;
    if (from == Direction::BOTTOM && pipe == TR) return Direction::RIGHT;
    if (from == Direction::BOTTOM && pipe == TL) return Direction::LEFT;

    return Direction::RIGHT;
}

bool canMove(Direction direction, char to) {
    // Coming in a direction `to` pipe
    // Can I move?
    if (direction == Direction::RIGHT && (to == LR || to == TL || to == BL)) return true;
    if (direction == Direction::LEFT && (to == LR || to == TR || to == BR)) return true;
    if (direction == Direction::TOP && (to == TB || to == BR || to == BL)) return true;
    if (direction == Direction::BOTTOM && (to == TB || to == TL || to == TR)) return true;
    return false;
}

bool isWall(std::pair<size_t, size_t> position) {
    auto[row, col] = position;
    if (row < 0 || row >= pipeMap.size()) return true;
    if (col < 0 || col >= pipeMap[0].size()) return true;
    return false;
}

std::vector<std::pair<size_t, size_t>> loop;

int64_t floodFill(Direction direction, std::pair<size_t, size_t> position, int64_t step = 1) {
    // From every position we can test  4 directions:
    // R,B R,L T,B T,L
    // We need to find which path ends in starting position

    if (isWall(position)) return -1;
    char type = pipeMap[position.first][position.second];
    if (type == GROUND) return -1;
    if (position != startingPosition && !canMove(direction, type)) return -1;
    if (position == startingPosition) return step;//loop length?

    loop.push_back(position);

    int64_t result = -1;

    auto exit = getExit(direction, type);

    if (exit == Direction::RIGHT) {
        result = floodFill(Direction::RIGHT, {position.first, position.second + 1}, step + 1);
    }
    if (exit == Direction::LEFT) {
        result = floodFill(Direction::LEFT, {position.first, position.second - 1}, step + 1);
    }
    if (exit == Direction::TOP) {
        result = floodFill(Direction::TOP, {position.first - 1, position.second}, step + 1);
    }
    if (exit == Direction::BOTTOM) {
        result = floodFill(Direction::BOTTOM, {position.first + 1, position.second}, step + 1);
    }

    return result;
}

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    while (!input.eof()) {

        std::string inputRow;
        std::getline(input, inputRow);
        if (inputRow.empty()) continue;

        // std::stringstream inputStream(inputRow);

        if (inputRow.find(START) != std::string::npos) {
            startingPosition = {pipeMap.size(), inputRow.find(START)};
            std::cout << startingPosition.first << ", " << startingPosition.second << std::endl;
        }
        pipeMap.push_back(inputRow);
    }

    loop = {startingPosition};
    floodFill(Direction::RIGHT, {startingPosition.first, startingPosition.second + 1});
    if (loop.size() == 1) {
        loop = {startingPosition};
        floodFill(Direction::LEFT, {startingPosition.first, startingPosition.second - 1});
        if (loop.size() == 1) {
            loop = {startingPosition};
            floodFill(Direction::TOP, {startingPosition.first - 1, startingPosition.second});
            if (loop.size() == 1) {
                loop = {startingPosition};
                floodFill(Direction::BOTTOM, {startingPosition.first + 1, startingPosition.second});
            }
        }
    }

    std::cout << loop.size() / 2 << std::endl;

    return EXIT_SUCCESS;
}