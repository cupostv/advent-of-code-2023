#include "../helpers/includes.hpp"

#define INPUT "input.txt"

struct Map {
    static const char BLOCKED = '#';

    using Visited = std::unordered_map<helper::Point, bool, helper::PointHash>;

    std::vector<std::string> grid;

    helper::Point start = {0, 1};

    bool isValid(const helper::Point p) const {
        if (p.x >= 0 && p.x < grid.size() && p.y >= 0 && p.y < grid[0].size()) return true;
        return false;
    }

    bool canFreelyMove(const helper::Point p) const {
        if (!isValid(p)) return false;
        if (grid[p.x][p.y] != BLOCKED) return true;
        return false;
    }

    std::vector<helper::Point> getNext(const helper::Point prev, helper::Point current) const {
        auto r = current.getRight();
        auto l = current.getLeft();
        auto d = current.getDown();
        auto u = current.getUp();

        std::vector<helper::Point> newPath;
        if (r != prev && canFreelyMove(r))
            newPath.push_back(r);
        if (l != prev && canFreelyMove(l))
            newPath.push_back(l);
        if (d != prev && canFreelyMove(d))
            newPath.push_back(d);
        if (u != prev && canFreelyMove(u))
            newPath.push_back(u);
        // End
        return newPath;
    }

    int64_t hike() {

        Visited visited;

        std::function<int64_t(helper::Point, helper::Point, int64_t)> f =
            [&](helper::Point prev, helper::Point current, int64_t steps) {
                if (visited[current]) return (int64_t)0;
                auto next = getNext(prev, current);
                if (next.size() == 0) return steps;

                visited[current] = true;

                int64_t max = steps;

                bool allWrongWay = true;
                for (auto point : next) {
                    // pass same visited set
                    auto numSteps = f(current, point, steps + 1);
                    max = std::max(max, numSteps);
                    if (numSteps != 0) allWrongWay = false;
                }

                visited[current] = false;

                return allWrongWay ? 0 : max;
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

    std::cout << map.hike() << std::endl;

    return EXIT_SUCCESS;
}
