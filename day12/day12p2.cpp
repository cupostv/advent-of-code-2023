#include "../helpers/includes.hpp"

#define INPUT "input.txt"

using Cache = std::vector<std::vector<std::vector<int64_t>>>;

struct Springs {
  std::string map;
  std::vector<int32_t> ammount;

  int64_t getArrangements() const {
    int32_t mapSize = map.size();
    int32_t ammountSize = ammount.size();

    int32_t maxLen = *std::max_element(ammount.begin(), ammount.end());

    Cache cache(mapSize, {(uint32_t)ammountSize + 1, std::vector<int64_t>(maxLen + 1, -1)});

    std::function<int64_t(int32_t, int32_t, int32_t)> f =
      [&](int32_t i, int32_t j, int32_t cur) -> int64_t {
        // i = current index in map
        // j = current index in ammount
        // cur = length of current run
        if (i >= mapSize) return (int64_t)(j == ammountSize);
        if (cache[i][j][cur] != -1) return cache[i][j][cur];
        auto& res = cache[i][j][cur];
        res = 0;
        // Case 1: place a . (reset cur)
        if ((map[i] == '.' || map[i] == '?') && cur == 0)
          res += f(i + 1, j, 0);
        // Case 2: place a # (increment cur / increment j and reset cur)
        if ((map[i] == '#' || map[i] == '?') && j < ammountSize) {
          if (cur + 1 == ammount[j])
            res += (i + 1 == mapSize || map[i + 1] != '#') * f(i + 2, j + 1, 0);
          else
            res += f(i + 1, j, cur + 1);
        }
        return res;
      };

    return f(0, 0, 0);
  }

};

#define repeat(n) for (int _=0; _<(n); _++)

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    int64_t sum = 0;
    std::string t;
    while (!input.eof()) {

        std::string inputRow;
        std::getline(input, inputRow);
        if (inputRow.empty()) continue;

        std::stringstream inputStream(inputRow);

        Springs springs;
        std::getline(inputStream, springs.map, ' ');

        while (getline(inputStream, t, ',')) springs.ammount.push_back(std::stoi(t));

        std::string newMap = springs.map;
        repeat(4) newMap += "?" + springs.map;
        springs.map = newMap;
        std::vector<int> newAmmount = springs.ammount;
        repeat(4) for (auto x: springs.ammount) newAmmount.push_back(x);
        springs.ammount = newAmmount;
        sum += springs.getArrangements();
    }

    std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}