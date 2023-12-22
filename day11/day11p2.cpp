#include "../helpers/includes.hpp"

#define INPUT "input.txt"

struct Galaxy {
    static const char SYMBOL = '#';
    helper::Point position;

    int32_t getDistance(Galaxy other) {
        // Manhattan distance
        return position.manhattanDistance(other.position);
    }
};

std::vector<Galaxy> loadGalaxies(const std::vector<std::string> &map, const int32_t expandMul) {
    std::vector<Galaxy> galaxies;

    const int32_t expandSize = expandMul;

    int32_t rowOffset = 0;
    for (int32_t row = 0; row < map.size(); row++) {
        bool hasGalaxy = false;
        for (int32_t col = 0; col < map[row].size(); col++) {
            if (map[row][col] == Galaxy::SYMBOL) {
                galaxies.push_back({{col, row + rowOffset}});
                hasGalaxy = true;
            }
        }
        if (!hasGalaxy) {
            rowOffset += (expandSize - 1);
        }
    }

    std::vector<int32_t> emptyCols = {0};

    for (int32_t col = 1; col < map[0].size(); col++) {
        bool hasGalaxy = false;
        for (int32_t row = 0; row < map.size(); row++) {
            if (map[row][col] == Galaxy::SYMBOL) {
                hasGalaxy = true;
                break;
            }
        }
        if (!hasGalaxy) {
            emptyCols.push_back(emptyCols[col - 1] + (expandSize - 1));
        } else {
            emptyCols.push_back(emptyCols[col - 1]);
        }
    }

    for (auto &g : galaxies) {
        g.position.x += emptyCols[g.position.x];
    }

    return galaxies;
}

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    std::vector<std::string> inputMap;

    while (!input.eof()) {

        std::string inputRow;
        std::getline(input, inputRow);
        if (inputRow.empty()) continue;

        inputMap.push_back(inputRow);
    }

    std::vector<Galaxy> galaxies = loadGalaxies(inputMap, 1000000);

    int64_t sum = 0;
    for (int32_t i = 0; i < galaxies.size(); i++) {
        for (int32_t j = i + 1; j < galaxies.size(); j++) {
            sum += galaxies[i].getDistance(galaxies[j]);
        }
    }

    std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}