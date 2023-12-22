#include "../helpers/includes.hpp"

#define INPUT "input.txt"

using Cache = std::unordered_map<helper::Point, helper::Point, helper::PointHash>;

void moveRocksUp(std::vector<std::string> &map, Cache &cache) {
    for (int32_t i = 0; i < map.size(); i++)
        for (int32_t c = 0; c < map[0].size(); c++) {
            if (map[i][c] == 'O') {
                auto pos = cache[{i, c}];
                map[i][c] = '.';
                while(map[pos.x][pos.y] == 'O') pos.x++;

                map[pos.x][pos.y] = 'O';
            }
        }
}

void moveRocksDown(std::vector<std::string> &map, Cache &cache) {
    for (int32_t i = map.size() - 1; i >= 0; i--)
        for (int32_t c = 0; c < map[0].size(); c++) {
            if (map[i][c] == 'O') {
                auto pos = cache[{i, c}];
                map[i][c] = '.';
                while(map[pos.x][pos.y] == 'O') pos.x--;

                map[pos.x][pos.y] = 'O';
            }
        }
}

void moveRocksLeft(std::vector<std::string> &map, Cache &cache) {
    for (int32_t i = 0; i < map.size(); i++)
        for (int32_t c = 0; c < map[0].size(); c++) {
            if (map[i][c] == 'O') {
                auto pos = cache[{i, c}];
                map[i][c] = '.';
                while(map[pos.x][pos.y] == 'O') pos.y++;

                map[pos.x][pos.y] = 'O';
            }
        }
}

void moveRocksRight(std::vector<std::string> &map, Cache &cache) {
    for (int32_t i = 0; i < map.size(); i++)
        for (int32_t c = map[0].size() - 1; c >= 0; c--) {
            if (map[i][c] == 'O') {
                if (cache.count({i, c}) == 0) std::cout << "HELP!!";
                auto pos = cache[{i, c}];
                map[i][c] = '.';
                while(map[pos.x][pos.y] == 'O') pos.y--;

                map[pos.x][pos.y] = 'O';
            }
        }
}

void moveRocksCycle(std::vector<std::string> &map, Cache &up, Cache &left, Cache &down, Cache &right) {
    // up left down right
    moveRocksUp(map, up);
    moveRocksLeft(map, left);
    moveRocksDown(map, down);
    moveRocksRight(map, right);
}

int32_t getWeight(const std::vector<std::string> &map) {
    int32_t sum = 0;
    for (int32_t i = 0, weight = map.size(); i <= map.size(); i++, weight--) {
        sum += std::count(map[i].begin(), map[i].end(), 'O') * weight;
    }

    return sum;
}

auto getCacheUp(const std::vector<std::string> &map) {
    Cache cache;

    std::vector<int32_t> rockCount(map[0].size(), 0);
    for (int32_t row = 0; row < map.size(); row++) {
        for (int32_t col = 0; col < map[0].size(); col++) {
            // For every space different than cube rock
            if (map[row][col] == '#') continue;
            // simulate tilt
            int32_t j = row - 1;
            while (j >= -1 && (j != -1 && (map[j][col] == '.' || map[j][col] == 'O'))) j--;
            cache[{row, col}] = {j + 1, col};
        }
    }

    return cache;
}

auto getCacheLeft(const std::vector<std::string> &map) {
    Cache cache;

    for (int32_t row = 0; row < map.size(); row++) {
        for (int32_t col = 0; col < map[0].size(); col++) {
            // For every space different than cube rock
            if (map[row][col] == '#') continue;
            // simulate tilt
            int32_t j = col - 1;
            while (j >= -1 && (j != -1 && (map[row][j] == '.' || map[row][j] == 'O'))) j--;
            cache[{row, col}] = {row, j + 1};
        }
    }

    return cache;
}

auto getCacheDown(const std::vector<std::string> &map) {
    Cache cache;

    for (int32_t row = 0; row < map.size(); row++) {
        for (int32_t col = 0; col < map[0].size(); col++) {
            // For every space different than cube rock
            if (map[row][col] == '#') continue;
            // simulate tilt
            int32_t j = row + 1;
            while (j <= map.size() && (j != map.size() && (map[j][col] == '.' || map[j][col] == 'O'))) j++;
            cache[{row, col}] = {j - 1, col};
        }
    }

    return cache;
}

auto getCacheRight(const std::vector<std::string> &map) {
    Cache cache;

    for (int32_t row = 0; row < map.size(); row++) {
        for (int32_t col = 0; col < map[0].size(); col++) {
            // For every space different than cube rock
            if (map[row][col] == '#') continue;
            // simulate tilt
            int32_t j = col + 1;
            while (j <= map[0].size() && (j != map[0].size() && (map[row][j] == '.' || map[row][j] == 'O'))) j++;
            cache[{row, col}] = {row, j - 1};
        }
    }

    return cache;
}

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    int32_t sum = 0;
    std::vector<std::string> map;
    while (!input.eof()) {

        std::string inputRow;
        std::getline(input, inputRow);

        if (inputRow.empty()) continue;

        map.push_back(inputRow);
    }

    auto upCache = getCacheUp(map);
    auto downCache = getCacheDown(map);
    auto rightCache = getCacheRight(map);
    auto leftCache = getCacheLeft(map);

    std::vector<int32_t> weight;

    // After some time, rocks will stabilize and begin to form a cycle
    // Find cycle and recalculate
    // Assume cycle length of minimum 10
    const int32_t minCycleLen = 5;
    int32_t cycleLen = 0;
    int32_t cycleStart = 0;

    while(true) {
        moveRocksCycle(map, upCache, leftCache, downCache, rightCache);

        weight.push_back(getWeight(map));

        // Try to find last 10 values and see if they repeat somewhere else
        if (weight.size() > minCycleLen) {
            auto res = std::search(weight.begin(), weight.end(), weight.end() - minCycleLen, weight.end());
            if (res < weight.end() - minCycleLen) {
                // res is beggining of new cycle
                cycleLen = std::abs(std::distance(res, weight.end() - minCycleLen)) + 1;
                cycleStart = std::distance(weight.begin(), res);
                break;
            }
        }
    }

    int64_t cycleTarget = 1000000000;

    std::cout << weight[cycleStart + (cycleTarget - cycleStart) % (cycleLen - 1) - 1] << std::endl;

    return EXIT_SUCCESS;
}