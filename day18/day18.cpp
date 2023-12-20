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
#include <climits>

#define INPUT "input.txt"

bool isAsciiDigit(char digit) {
    return std::isdigit(static_cast<unsigned char>(digit));
}

void removeSpaces(std::string &str) {
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
}

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

using Direction = std::pair<int64_t, int64_t>;
using Position = std::pair<int64_t, int64_t>;

static const Direction RIGHT = {0, 1};
static const Direction LEFT = {0, -1};
static const Direction UP = {-1, 0};
static const Direction DOWN = {1, 0};

struct DigPlanEntry {
    Direction dir;
    int64_t ammount;
    std::string colorCode;

    void dump() const {
        std::cout << dir.first << ", " << dir.second << " " << ammount << " " << colorCode << std::endl;
    }

    Position dig(Position from) const {
        return {from.first + dir.first * ammount, + from.second + dir.second * ammount};
    }
};

struct Polygon {
    std::vector<Position> points;

    int64_t shoelace() const {
        int64_t area = 0;

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

    int64_t area() const {
        return shoelace() + perimeter() / 2 + 1;
    }
};

Polygon dig(const std::vector<DigPlanEntry> &plan) {
    Polygon digMap;

    Position cur = {0, 0};
    digMap.points.push_back(cur);

    for (auto &entry : plan) {
        cur = entry.dig(cur);
        digMap.points.push_back(cur);
    }

    return digMap;
}

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    int64_t sum = 0;

    std::vector<DigPlanEntry> digPlan;

    while (!input.eof()) {

        std::string inputRow;
        std::getline(input, inputRow);

        if (inputRow.empty()) continue;

        std::stringstream inputStream(inputRow);

        char ch;
        inputStream >> ch;

        DigPlanEntry entry;
        if (ch == 'R') entry.dir = RIGHT;
        else if (ch == 'L') entry.dir = LEFT;
        else if (ch == 'D') entry.dir = DOWN;
        else if (ch == 'U') entry.dir = UP;

        inputStream >> entry.ammount;

        std::string dummy;
        std::getline(inputStream, dummy, ' ');
        std::getline(inputStream, dummy, '(');
        std::getline(inputStream, dummy, ')');

        entry.colorCode = dummy;

        std::string first(entry.colorCode.begin() + 1, entry.colorCode.begin() + 6);
        std::string second(entry.colorCode.begin() + 6, entry.colorCode.end());

        std::stringstream ss;
        ss << std::hex << first;
        ss >> entry.ammount;

        int32_t direction;
        std::stringstream ss1;
        ss1 << std::hex << second;
        ss1 >> direction;

        if (direction == 0) entry.dir = RIGHT;
        else if (direction == 2) entry.dir = LEFT;
        else if (direction == 1) entry.dir = DOWN;
        else if (direction == 3) entry.dir = UP;

        digPlan.push_back(entry);
    }

    auto polygon = dig(digPlan);

    std::cout << std::fixed << polygon.area() << std::endl;

    return EXIT_SUCCESS;
}
