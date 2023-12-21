#include "../helpers/includes.hpp"

#define INPUT "input.txt"

struct Map {
    static const char PATH = '.';
    static const char SLOPE_DOWN = 'v';
    static const char SLOPE_RIGHT = '>';
    static const char BLOCKED = '#';

    std::vector<std::string> grid;

    helper::Point start = {0, 1};

    bool isValid(const helper::Point p) const {
        if (p.x >= 0 && p.x < grid.size() && p.y >= 0 && p.y < grid[0].size()) return true;
        return false;
    }

    bool canFreelyMove(const helper::Point p) const {
        if (!isValid(p)) return false;
        auto c = grid[p.x][p.y];
        if (c == PATH) return true;
        return false;
    }

    bool isSlope(const helper::Point p) const {
        if (!isValid(p)) return false;
        if (grid[p.x][p.y] == SLOPE_DOWN || grid[p.x][p.y] == SLOPE_RIGHT) return true;
        return false;
    }

    std::vector<helper::Point> getNext(const helper::Point prev, helper::Point current) const {
        auto r = current.getRight();
        auto l = current.getLeft();
        auto d = current.getDown();
        auto u = current.getUp();

        if (r != prev && canFreelyMove(r)) {
            return {r};
        } else if (l != prev && canFreelyMove(l)) {
            return {l};
        } else if (d != prev && canFreelyMove(d)) {
            return {d};
        } else if (u != prev && canFreelyMove(u)) {
            return {u};
        } else {
            std::vector<helper::Point> newPath;
            if (r != prev && isSlope(r)) {
                newPath.push_back(r);
            }
            if (d != prev && isSlope(d)) {
                newPath.push_back(d);
            }
            return newPath;
        }
        // End
        return {};
    }

    int64_t hike() {
        using Visited = std::unordered_map<helper::Point, bool, helper::PointHash>;

        Visited visited;

        std::function<int64_t(helper::Point, helper::Point, int64_t, Visited&)> f =
            [&](helper::Point prev, helper::Point current, int64_t steps, Visited &visited) {
                if (visited[current]) return (int64_t)0;
                auto next = getNext(prev, current);
                if (next.size() == 0) return steps;

                visited[current] = true;

                int64_t max = steps;

                for (auto point : next) {
                    if (next.size() == 1) {
                        // pass same visited set
                        max = std::max(max, f(current, point, steps + 1, visited));
                    } else {
                        // make copy for every new path
                        Visited temp = visited;
                        max = std::max(max, f(current, point, steps + 1, temp));
                    }

                }

                return max;
            };

        return f(start.getLeft(), start, 0, visited);
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

        // std::cout << inputRow << std::endl;
    }

    std::cout << map.hike() << std::endl;

    return EXIT_SUCCESS;
}
