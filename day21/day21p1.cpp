#include "../helpers/includes.hpp"

#define INPUT "input.txt"

struct Map {
  static const char GARDEN = '.';
  static const char VISIT = 'O';
  static const char ROCK = '#';
  static const char START = 'S';

  std::vector<std::string> grid;

  bool isWall(const helper::Point &p) const {
    return p.isValid(0, 0, grid.size() - 1, grid[0].size() - 1) && grid[p.x][p.y] == ROCK;
  }

  int64_t visitGardens(helper::Point start, int64_t maxSteps, int64_t count = 0) const {
    std::vector<std::vector<std::vector<bool>>> cache(grid.size(), {grid[0].size(), std::vector<bool>(maxSteps + 1, false)});

    std::function<int64_t(helper::Point, int64_t)> visit =
      [&](helper::Point point, int64_t cnt) -> int64_t {
      if (cnt > maxSteps) return 0;
      if (isWall(point)) return 0;
      if (cache[point.x][point.y][cnt]) return 0;

      cache[point.x][point.y][cnt] = true;

      if (cnt == maxSteps)
        return 1;

      int64_t garden = 0;
      for (auto p : point.getAdjacent())
        garden += visit(p, cnt + 1);

      return garden;
    };
    return visit(start, count);
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

    std::cout << map.visitGardens(startPos, 64) << std::endl;

    return EXIT_SUCCESS;
}
