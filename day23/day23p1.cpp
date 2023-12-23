#include "../helpers/includes.hpp"

#define INPUT "input.txt"

struct Map {
    static const char PATH = '.';
    static const char SLOPE_DOWN = 'v';
    static const char SLOPE_RIGHT = '>';
    static const char BLOCKED = '#';

    std::vector<std::string> grid;

    helper::Point start;
    helper::Point end;

    bool isValid(const helper::Point p) const {
        return p.isValid(0, 0, grid.size() - 1, grid[0].size() - 1);
    }

    bool canFreelyMove(const helper::Point p) const {
        return isValid(p) && grid[p.x][p.y] == PATH;
    }

    bool isSlope(const helper::Point p) const {
        return isValid(p) && (grid[p.x][p.y] == SLOPE_DOWN || grid[p.x][p.y] == SLOPE_RIGHT);
    }

    std::vector<helper::Point> getNext(const helper::Point &prev, const helper::Point &current) const {

        for (auto p : current.getAdjacent())
            if (p != prev && canFreelyMove(p))
                return {std::move(p)};

        std::vector<helper::Point> newPath;
        for (auto p : {current.getRight(), current.getDown()})
            if (p != prev && isSlope(p))
                newPath.push_back(std::move(p));

        return newPath;

    }

    int64_t hike() {

        std::function<int64_t(helper::Point, helper::Point, int64_t)> f =
            [&](helper::Point &&prev, helper::Point &&current, int64_t steps) {
                auto next = std::move(getNext(prev, current));
                if (next.size() == 0) return steps;

                int64_t max = 0;

                for (auto point : next) {
                    max = std::max(max, f(std::move(current), std::move(point), steps + 1));
                }

                return max;
            };

        return f(start.getLeft(), start, 0);
    }

};

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    Map map;

    while (!input.eof()) {

        std::string inputRow;

        std::getline(input, inputRow);
        if (inputRow.empty()) break;

        map.grid.push_back(inputRow);
    }

    map.start = {0, (int64_t)map.grid[0].find('.')};
    map.end = {(int64_t)map.grid.size() - 1, (int64_t)map.grid[map.grid.size() - 1].find('.')};

    std::cout << map.hike() << std::endl;

    return EXIT_SUCCESS;
}
