#include "../helpers/includes.hpp"

#define INPUT "input.txt"

struct Map {
    static const char BLOCKED = '#';

    using Traversal = std::set< std::pair<helper::Point, int32_t>, std::greater<std::pair<helper::Point, int32_t>> >;

    using Path = std::unordered_map<helper::Point, Traversal, helper::PointHash>;

    std::vector<std::string> grid;

    helper::Point start;
    helper::Point end;

    bool isValid(const helper::Point &p) const {
        return p.isValid(0, 0, grid.size() - 1, grid[0].size() - 1);
    }

    bool canFreelyMove(const helper::Point &p) const {
        return isValid(p) && grid[p.x][p.y] != BLOCKED;
    }

    std::vector<helper::Point> getNext(const helper::Point &prev, const helper::Point &current) const {

        std::vector<helper::Point> newPath;
        newPath.reserve(3);
        for (auto p : current.getAdjacent())
            if (p != prev && canFreelyMove(p))
                newPath.push_back(std::move(p));

        return newPath;
    }

    Path createPaths() const {
        Path path;

        std::function<void(helper::Point, helper::Point, helper::Point, int64_t)> f =
            [&](helper::Point start, helper::Point prev, helper::Point current, int64_t steps) {
                if (path.contains(current)) {
                    path[start].insert({current, steps});
                    return;
                }
                auto next = getNext(prev, current);

                if (next.size() == 0 || next.size() > 1) {
                    // crossing
                    path[start].insert({current, steps});
                    path[current].insert({start, steps});
                    steps = 0;
                    start = current;
                }

                for (auto &point : next) {
                    f(start, current, point, steps + 1);
                }
        };

        f(start, start.getLeft(), start, 0);

        return path;
    }

    int64_t hike() const {
        using Visited = std::unordered_map<helper::Point, bool, helper::PointHash>;

        auto paths = std::move(createPaths());

        Visited visited;

        std::function<int64_t(const helper::Point&, int64_t)> f =
            [&](const helper::Point &current, int64_t steps) {
                if (visited[current]) return (int64_t)0;
                if (current == end) return steps;

                visited[current] = true;

                int64_t max = 0;

                for (auto [point, len] : paths[current]) {
                    if (point == end) {
                        // We know that end will be first point so we just need to take length to end
                        visited[current] = false;
                        return steps + len;
                    }
                    // pass same visited set
                    auto numSteps = f(std::move(point), steps + len);
                    max = std::max(max, numSteps);
                }

                visited[current] = false;

                return max;
            };

        return f(start, 0);
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
