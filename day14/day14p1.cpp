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
            if (map[row][col] != '#') {
                // simulate tilt
                int32_t j = row - 1; // -+ tiltV
                while (j >= -1 && (j != -1 && (map[j][col] == '.' || map[j][col] == 'O'))) j--;
                cache[{row, col}] = {j + 1, col};
            }
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

    moveRocksUp(map, upCache);
    std::cout << getWeight(map) << std::endl;

    return EXIT_SUCCESS;
}