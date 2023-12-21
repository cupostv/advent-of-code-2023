#include "../helpers/includes.hpp"

#define INPUT "input.txt"

struct Map {
  static const char GARDEN = '.';
  static const char VISIT = 'O';
  static const char ROCK = '#';
  static const char START = 'S';

  static const int32_t maxSteps = 64;

  std::vector<std::string> grid;

  bool isWall(const helper::Point &p) const {
    return p.x < 0 || p.x >= grid.size() || p.y < 0 || p.y >= grid[0].size() || grid[p.x][p.y] == ROCK;
  }

  int64_t visitGardens(helper::Point start, int64_t count = 0) {
    std::vector<std::vector<std::vector<bool>>> cache(grid.size(), {grid[0].size(), std::vector<bool>(maxSteps + 1, false)});

    std::function<int64_t(helper::Point, int64_t)> visit =
      [&](helper::Point point, int64_t cnt) -> int64_t {

      if (isWall(point)) return 0;
      if (cnt > maxSteps) return 0;
      if (cache[point.x][point.y][cnt]) return 0;

      cache[point.x][point.y][cnt] = true;
      int64_t garden = 0;
      garden += visit(point.getLeft(), cnt + 1);
      garden += visit(point.getRight(), cnt + 1);
      garden += visit(point.getDown(), cnt + 1);
      garden += visit(point.getUp(), cnt + 1);

      if (cnt == maxSteps) {
        garden++;
      }
      return garden;
    };
    return visit(start, count);
  }

  int64_t getGardenCount(helper::Point start) {
    return visitGardens(start, 0);
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

    std::cout << map.getGardenCount(startPos) << std::endl;
    // helper::dumpRow(map.grid);

    return EXIT_SUCCESS;
}
