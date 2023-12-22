#include "../helpers/includes.hpp"

#define INPUT "input.txt"

using Movement = std::tuple<helper::Point, helper::Point>;

static std::vector<std::string> grid;

std::vector<Movement> getNext(const helper::Point &currentPos, const helper::Point &currentDir) {
    std::vector<Movement> ret;
    auto [row, col] = currentPos;
    if (!currentPos.isValid(0, 0, grid.size() - 1, grid[0].size() - 1)) return ret;

    auto symbol = grid[currentPos.x][currentPos.y];

    if (symbol == '/') {
        // Reflect
        ret.push_back({{row - currentDir.y, col - currentDir.x}, {-currentDir.y, -currentDir.x}});
    }
    else if (symbol == '\\') {
        ret.push_back({{row + currentDir.y, col + currentDir.x}, {currentDir.y, currentDir.x}});
    }
    else if (symbol == '|' && (currentDir == helper::Point::RIGHT || currentDir == helper::Point::LEFT)) {
        // Split up and down
        ret.push_back({currentPos.getUp(), helper::Point::UP});
        ret.push_back({currentPos.getDown(), helper::Point::DOWN});
    } else if (symbol == '-' && (currentDir == helper::Point::DOWN || currentDir == helper::Point::UP)) {
        ret.push_back({currentPos.getLeft(), helper::Point::LEFT});
        ret.push_back({currentPos.getRight(), helper::Point::RIGHT});
    } else {
        ret.push_back({currentPos + currentDir, currentDir});
    }

    return ret;
}

int32_t calculateEnergy(const helper::Point &startPos, const helper::Point &startDir) {
    std::unordered_map<helper::Point, std::vector<helper::Point>, helper::PointHash> visited;
    auto posStart = startPos;
    helper::Point dirStart = startDir;

    std::function<void(const helper::Point&, const helper::Point&)> move =
        [&](const helper::Point &pos, const helper::Point &dir) -> void {
            if (visited.contains(pos) && helper::find(visited[pos], dir)) return;
            if (!pos.isValid(0, 0, grid.size() - 1, grid[0].size() - 1)) return;

            visited[pos].push_back(dir);

            for (auto [nextPos, nextDir] : getNext(pos, dir)) {
                move(nextPos, nextDir);
            }
        };

    move(posStart, dirStart);

    return visited.size();
}

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    while (!input.eof()) {

        std::string inputRow;
        std::getline(input, inputRow);

        if (inputRow.empty()) continue;

        grid.push_back(inputRow);
    }

    int32_t max = 0;

    for (int32_t col = 0; col < grid[0].size(); col++) {
        max = std::max(max, calculateEnergy({0, col}, helper::Point::DOWN));
        max = std::max(max, calculateEnergy({(int32_t)grid.size() - 1, col}, helper::Point::UP));
    }

    for (int32_t row = 0; row < grid.size(); row++) {
        max = std::max(max, calculateEnergy({row, 0}, helper::Point::RIGHT));
        max = std::max(max, calculateEnergy({row, (int32_t)grid[0].size() - 1}, helper::Point::LEFT));
    }

    std::cout << max << std::endl;

    return EXIT_SUCCESS;
}