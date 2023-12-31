#include "../helpers/includes.hpp"

#define INPUT "input.txt"

std::vector<std::string> pipeMap;
helper::Point startingPosition;

#define TB '|'
#define LR '-'
#define TR 'L'
#define TL 'J'
#define BL '7'
#define BR 'F'
#define GROUND '.'
#define START 'S'
#define INVALID {-1, -1}

bool isWall(helper::Point position) {
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
    helper::Point position;
    std::array<helper::Point, 4> connections; // in and out

    Pipe(const helper::Point position) : position(position) {
        char pipe = '.';
        if (!isWall(position))
            pipe = pipeMap[position.x][position.y];
        connections[Direction::LEFT] = position.getLeft();
        connections[Direction::RIGHT] = position.getRight();
        connections[Direction::TOP] = position.getUp();
        connections[Direction::BOTTOM] = position.getDown();

        if (pipe == LR) {
            connections[Direction::TOP] = INVALID;
            connections[Direction::BOTTOM] = INVALID;
        }
        else if (pipe == TB) {
            connections[Direction::LEFT] = INVALID;
            connections[Direction::RIGHT] = INVALID;
        }
        else if (pipe == TR) {
            connections[Direction::LEFT] = INVALID;
            connections[Direction::BOTTOM] = INVALID;
        }
        else if (pipe == TL) {
            connections[Direction::BOTTOM] = INVALID;
            connections[Direction::RIGHT] = INVALID;
        }
        else if (pipe == BR) {
            connections[Direction::TOP] = INVALID;
            connections[Direction::LEFT] = INVALID;
        }
        else if (pipe == BL) {
            connections[Direction::TOP] = INVALID;
            connections[Direction::RIGHT] = INVALID;
        }

    }

    bool isValid() const {
        return !isWall(position);
    }

    Pipe get(Direction dir) const {
        return Pipe(connections[dir]);
    }
    Pipe getTop() const {
        return get(Direction::TOP);
    }
    Pipe getBottom() const {
        return get(Direction::BOTTOM);
    }
    Pipe getLeft() const {
        return get(Direction::LEFT);
    }
    Pipe getRight() const {
        return get(Direction::RIGHT);
    }

    // Get exit position for given entry position
    helper::Point getExit(helper::Point entry) const {
        // Filter invalid and entry
        helper::Point result = INVALID;
        int32_t count = 0;
        for (auto connection : connections) {
            if (connection != (helper::Point)INVALID && connection != entry) {
                result = connection;
                count++;
            }
        }
        if (count > 1) {
            return INVALID;
        }

        return result;
    }
};

std::pair<std::vector<helper::Point>, bool> getPath(Pipe pipe) {
    std::vector<helper::Point> result = {startingPosition};

    auto entryPos = startingPosition;
    auto exitPos = pipe.getExit(entryPos);
    entryPos = pipe.position;
    result.push_back(exitPos);

    while(true) {
        if (exitPos == startingPosition) break;
        if (exitPos == (helper::Point)INVALID) break;

        Pipe exitPipe(exitPos);
        auto temp = exitPipe.getExit(entryPos);
        entryPos = exitPos;
        exitPos = temp;
        result.push_back(exitPos);
    }

    return {result, exitPos == startingPosition};
}

std::vector<helper::Point> getValidPath() {
    Pipe start(startingPosition);

    for (auto pipe : {start.getLeft(), start.getRight(), start.getTop(), start.getBottom()}) {
        auto [path, valid] = getPath(pipe);
        if (valid) return path;
    }

    return {};
}

int32_t calculateAreaInside(const std::vector<helper::Point> &path) {
    // Construct polygon
    helper::GridPolygon polygon;

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

        if (inputRow.find(START) != std::string::npos) {
            startingPosition = helper::Point(pipeMap.size(), inputRow.find(START));
        }
        pipeMap.push_back(inputRow);
    }

    std::vector<helper::Point> path = getValidPath();

    std::cout << calculateAreaInside(path) << std::endl;

    return EXIT_SUCCESS;
}