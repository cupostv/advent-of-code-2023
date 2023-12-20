#include "../helpers/includes.hpp"

#define INPUT "input.txt"

template <typename T>
void printVector(const std::vector<T>& c)
{
    for (auto i : c)
        std::cout << i << ' ';
    std::cout << '\n';
}

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

struct pairhash {
public:
  template <typename T, typename U>
  std::size_t operator()(const std::pair<T, U> &x) const
  {
    std::size_t seed = 0;
    hash_combine(seed, x.first);
    hash_combine(seed, x.second);

    return seed;
  }
};

using Pos = std::pair<int32_t, int32_t>;
using Cache = std::unordered_map<Pos, Pos, pairhash>;

void moveRocksUp(std::vector<std::string> &map, Cache &cache) {
    for (int32_t i = 0; i < map.size(); i++)
        for (int32_t c = 0; c < map[0].size(); c++) {
            if (map[i][c] == 'O') {
                if (cache.count({i, c}) == 0) std::cout << "HELP!!";
                auto pos = cache[{i, c}];
                map[i][c] = '.';
                while(map[pos.first][pos.second] == 'O') pos.first++;

                map[pos.first][pos.second] = 'O';
            }
        }
}

void moveRocksDown(std::vector<std::string> &map, Cache &cache) {
    for (int32_t i = map.size() - 1; i >= 0; i--)
        for (int32_t c = 0; c < map[0].size(); c++) {
            if (map[i][c] == 'O') {
                if (cache.count({i, c}) == 0) std::cout << "HELP!!";
                auto pos = cache[{i, c}];
                map[i][c] = '.';
                while(map[pos.first][pos.second] == 'O') pos.first--;

                map[pos.first][pos.second] = 'O';
            }
        }
}

void moveRocksLeft(std::vector<std::string> &map, Cache &cache) {
    for (int32_t i = 0; i < map.size(); i++)
        for (int32_t c = 0; c < map[0].size(); c++) {
            if (map[i][c] == 'O') {
                if (cache.count({i, c}) == 0) std::cout << "HELP!!";
                auto pos = cache[{i, c}];
                map[i][c] = '.';
                while(map[pos.first][pos.second] == 'O') pos.second++;

                map[pos.first][pos.second] = 'O';
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
                while(map[pos.first][pos.second] == 'O') pos.second--;

                map[pos.first][pos.second] = 'O';
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
    // tilt == -1, tilt == 1
    Cache cache;

    std::vector<int32_t> rockCount(map[0].size(), 0);
    for (int32_t row; row < map.size(); row++) {
        for (int32_t col; col < map[0].size(); col++) {
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

auto getCacheLeft(const std::vector<std::string> &map) {
    // tilt == -1, tilt == 1
    Cache cache;

    for (int32_t row; row < map.size(); row++) {
        for (int32_t col; col < map[0].size(); col++) {
            // For every space different than cube rock
            if (map[row][col] != '#') {
                // simulate tilt
                int32_t j = col - 1; // -+ tiltV
                while (j >= -1 && (j != -1 && (map[row][j] == '.' || map[row][j] == 'O'))) j--;
                cache[{row, col}] = {row, j + 1};
            }
        }
    }

    return cache;
}

auto getCacheDown(const std::vector<std::string> &map) {
    // tilt == -1, tilt == 1
    Cache cache;

    for (int32_t row; row < map.size(); row++) {
        for (int32_t col; col < map[0].size(); col++) {
            // For every space different than cube rock
            if (map[row][col] != '#') {
                // simulate tilt
                int32_t j = row + 1; // -+ tiltV
                while (j <= map.size() && (j != map.size() && (map[j][col] == '.' || map[j][col] == 'O'))) j++;
                cache[{row, col}] = {j - 1, col};
            }
        }
    }

    return cache;
}

auto getCacheRight(const std::vector<std::string> &map) {
    // tilt == -1, tilt == 1
    Cache cache;

    for (int32_t row = 0; row < map.size(); row++) {
        for (int32_t col = 0; col < map[0].size(); col++) {
            // For every space different than cube rock
            if (map[row][col] != '#') {
                // simulate tilt
                int32_t j = col + 1; // -+ tiltV
                while (j <= map[0].size() && (j != map[0].size() && (map[row][j] == '.' || map[row][j] == 'O'))) j++;
                cache[{row, col}] = {row, j - 1};
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

        // std::cout << inputRow << std::endl;
    }

    auto upCache = getCacheUp(map);
    auto downCache = getCacheDown(map);
    auto rightCache = getCacheRight(map);
    auto leftCache = getCacheLeft(map);

    // moveRocksRight(map, rightCache);
    // moveRocksCycle(map, upCache, leftCache, downCache, rightCache);
    // moveRocksCycle(map, upCache, leftCache, downCache, rightCache);
    // moveRocksCycle(map, upCache, leftCache, downCache, rightCache);

    std::vector<int32_t> weight;
    // TODO FIND weight loop programatically and recalculate!!!
    for (int32_t i = 0; i < 200; i++) {
        moveRocksCycle(map, upCache, leftCache, downCache, rightCache);
        std::cout << getWeight(map) << std::endl;
        // weight.push_back(getWeight(map));


        // // if (i % 1000000 == 0) {
        // //     std::cout << i << std::endl;
        // // }
    }

    // moveRocksCycle(map, upCache, leftCache, downCache, rightCache);
    // moveRocksCycle(map, upCache, leftCache, downCache, rightCache);
    // moveRocksCycle(map, upCache, leftCache, downCache, rightCache);
    // moveRocksCycle(map, upCache, leftCache, downCache, rightCache);
    // moveRocksCycle(map, upCache, leftCache, downCache, rightCache);
    // moveRocksCycle(map, upCache, leftCache, downCache, rightCache);
    // moveRocksCycle(map, upCache, leftCache, downCache, rightCache);

    // for (auto row : map) {
    //     std::cout << row << std::endl;
    // }

    // std::cout << getWeightNorth(map) << std::endl;

    return EXIT_SUCCESS;
}