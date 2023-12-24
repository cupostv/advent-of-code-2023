#include "../helpers/includes.hpp"

#define INPUT "input.txt"

struct Map {
  static const char GARDEN = '.';
  static const char VISIT = 'O';
  static const char ROCK = '#';
  static const char START = 'S';

  static const int32_t maxSteps = 64;

  std::vector<std::string> grid;

  bool isRock(const helper::Point &p) const {
    return grid[p.x][p.y] == ROCK;
  }

  auto visitGardens(const helper::Point &start, const std::array<int64_t, 3> &steps) const {
    std::unordered_set<helper::Point, helper::PointHash> gardens = {start};

    int32_t size = grid.size();

    assert(size == grid[0].size());

    std::array<int64_t, 3> ret;

    // Old approach is faster to calculate 6ms vs 16ms on part 1
    // But this is more scaleable for part 2
    // For faster calculation I added to calculate 3 steps in one go
    auto step = steps[0];
    auto pos = 0;
    auto count = steps[2];
    for (int32_t i = 0; i < count; i++) {
      std::unordered_set<helper::Point, helper::PointHash> newGardens;
      for (auto garden : gardens) {
        for (auto p : garden.getAdjacent()) {
          auto testP = helper::Point(((p.x % size) + size) % size, ((p.y % size) + size) % size);
          if (!isRock(testP)) newGardens.insert(std::move(p));
        }
      }
      gardens = std::move(newGardens);
      if (i + 1 == step) {
        ret[pos++] = gardens.size();
        if (pos < steps.size())
          step = steps[pos];
      }
    }
    return ret;
  }

  int64_t getGardenCount(const helper::Point &start, int32_t maxSteps) const {
    const int32_t size = grid.size();
    const int32_t numStepsInOneGrid = size / 2;
    int32_t fullGrids = (maxSteps - numStepsInOneGrid) / size;

    assert ((maxSteps - numStepsInOneGrid) % size == 0);
    assert ((start.x == size / 2 && start.y == size /2));

    // x = step
    // yi = gardens
    // f(x) = ax2 + bx + c
    // At step = 0;
    // y0 = c             => c = y0
    // At step = 1;
    // y1 = a + b + c     => b = y1 - a - c => b => y1 - y2/2 + y1 - y0/2 - y0 => b => 2y1 - y2/2 - 3y0/2
    // At step = 2;
    // y2 = a*4 + b*2 + c => a * 4 = y2 - 2b - c => a * 4 = y2 - 2y1 + 2a + 2y0 - y0 => 2a = y2 - 2y1 + y0 => a = y2/2 - y1 + y0/2

    auto getCoeffs = [](const std::array<int64_t, 3> &y) {
      // a = y2/2 - y1 + y0/2
      // b = 2y1 - y2/2 - 3y0/2
      // c = y0
      // Reorganize calculations to not lose the precision
      // a + b = y1 - y0
      // 4a + 2b = y2 - c => 2a => y2 - c - 2(a + b)
      auto c = y[0];
      auto aPlusB = y[1] - c;
      auto twoA = y[2] - c - 2 * aPlusB; // This should be even now
      assert(twoA % 2 == 0);
      auto a = twoA / 2;
      auto b = aPlusB - a;

      return std::make_tuple(a, b, c);
    };

    auto y = visitGardens(start, {numStepsInOneGrid, numStepsInOneGrid + 1 * size, numStepsInOneGrid + 2 * size});

    auto [a, b, c] = getCoeffs({y[0], y[1], y[2]});

    return a * fullGrids * fullGrids + b * fullGrids + c;
  }

};

Map map;

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    helper::Point startPos;

    while (!input.eof()) {

        std::string inputRow;

        std::getline(input, inputRow);
        if (inputRow.empty()) break;
        auto start = inputRow.find(Map::START);
        if (start != std::string::npos) {
          startPos = helper::Point(map.grid.size(), start);
        }
        map.grid.push_back(inputRow);
    }

    std::cout << map.getGardenCount(startPos, 26501365) << std::endl;

    return EXIT_SUCCESS;
}
