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

using Direction = std::pair<int32_t, int32_t>;
using Position = std::pair<int32_t, int32_t>;
using Movement = std::pair<Position, Direction>;

static const Direction RIGHT = {0, 1};
static const Direction LEFT = {0, -1};
static const Direction UP = {-1, 0};
static const Direction DOWN = {1, 0};

static std::vector<std::string> grid;

std::vector<Movement> getNext(Position currentPos, Direction currentDir) {
    std::vector<Movement> ret;
    auto [row, col] = currentPos;
    if (row >= grid.size() || row < 0 || col >= grid[0].size() || col < 0) return ret;
    auto symbol = grid[row][col];
    if (symbol == '.') {
        ret.push_back({{row + currentDir.first, col + currentDir.second}, currentDir});
    }
    else if (symbol == '/') {
        ret.push_back({{row - currentDir.second, col - currentDir.first}, {-currentDir.second, -currentDir.first}});
    }
    else if (symbol == '\\') {
        ret.push_back({{row + currentDir.second, col + currentDir.first}, {currentDir.second, currentDir.first}});
    }
    else if (symbol == '|') {
        if (currentDir == RIGHT || currentDir == LEFT) {
            // Split up and down
            ret.push_back({{row + UP.first, col + UP.second}, UP});
            ret.push_back({{row + DOWN.first, col + DOWN.second}, DOWN});
        } else {
            // Pass through
            ret.push_back({{row + currentDir.first, col + currentDir.second}, currentDir});
        }
    } else if (symbol == '-') {
        if (currentDir == DOWN || currentDir == UP) {
            // Split left and right
            ret.push_back({{row + LEFT.first, col + LEFT.second}, LEFT});
            ret.push_back({{row + RIGHT.first, col + RIGHT.second}, RIGHT});
        } else {
            // Pass through
            ret.push_back({{row + currentDir.first, col + currentDir.second}, currentDir});
        }
    }

    return ret;
}

int32_t calculateEnergy(Position startPos, Direction startDir) {
    std::unordered_map<Position, std::vector<Direction>, pairhash> visited;
    Position posStart = startPos;
    Direction dirStart = startDir;

    auto isValid = [](Position pos) {
        return pos.first < grid.size() && pos.first >= 0 && pos.second < grid[0].size() && pos.second >= 0;
    };

    std::function<void(Position, Direction)> move =
        [&](Position pos, Direction dir) -> void {
            if (std::ranges::find(visited[pos], dir) != visited[pos].end()) return;
            if (!isValid(pos)) return;
            visited[pos].push_back(dir);// = true;

            auto next = std::move(getNext(pos, dir));
            for (auto [nextPos, nextDir] : next) {
                move(nextPos, nextDir);
            }
        };

    move(posStart, dirStart);

    return std::ranges::count_if(visited | std::ranges::views::values, [](const auto &value) { return value.size() > 0; });
}

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    int64_t sum = 0;

    while (!input.eof()) {

        std::string inputRow;
        std::getline(input, inputRow);

        if (inputRow.empty()) continue;

        // std::stringstream inputStream(inputRow);

        grid.push_back(inputRow);

        // std::cout << inputRow << std::endl;
    }

    int32_t max = 0;

    for (int32_t col = 0; col < grid[0].size(); col++) {
        auto energy = calculateEnergy({0, col}, DOWN);
        if (energy > max) {
            max = energy;
        }
        energy = calculateEnergy({grid.size() - 1, col}, UP);
        if (energy > max) {
            max = energy;
        }
    }

    for (int32_t row = 0; row < grid.size(); row++) {
        auto energy = calculateEnergy({row, 0}, RIGHT);
        if (energy > max) {
            max = energy;
        }
        energy = calculateEnergy({row, grid[0].size() - 1}, LEFT);
        if (energy > max) {
            max = energy;
        }
    }


    std::cout << max << std::endl;

    return EXIT_SUCCESS;
}