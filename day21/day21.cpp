#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <sstream>
#include <unordered_map>
#include <map>
#include <cmath>
#include <set>
#include <numeric>
#include <ranges>
#include <algorithm>
#include <list>
#include <climits>
#include <cassert>

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

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    while (!input.eof()) {

        std::string inputRow;

        std::getline(input, inputRow);
        if (inputRow.empty()) break;

        std::cout << inputRow << std::endl;
    }

    // std::cout << min << std::endl;
    return EXIT_SUCCESS;
}
