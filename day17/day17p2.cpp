#include "../helpers/includes.hpp"

#define INPUT "input.txt"

enum Direction {
    RIGHT,
    DOWN,
    LEFT,
    UP
};

std::array<helper::Point, 4> dXY = {
    helper::Point::RIGHT,
    helper::Point::DOWN,
    helper::Point::LEFT,
    helper::Point::UP};

struct HeatMap {
    std::fstream fin;
    std::vector<std::string> map;

    using Distance = std::vector<std::vector<std::vector<std::vector<int32_t>>>>;

    auto getAdj(helper::Point p, Direction dir, int32_t steps) const {
        std::vector<std::tuple<helper::Point, int32_t, Direction>> ret;

        if (steps < 10)
            ret.push_back({p + dXY[dir], steps + 1, dir});
        if (steps >= 4) {
            int32_t newDir = (dir + 1) % dXY.size();
            ret.push_back({p + dXY[newDir], 1, static_cast<Direction>(newDir)});

            newDir = (dir + 3) % dXY.size();
            ret.push_back({p + dXY[newDir], 1, static_cast<Direction>(newDir)});
        }

        return ret;
    }

    auto dijkstra() const {
        const uint32_t maxStepsInOneDirection = 10;
        std::queue<std::tuple<int32_t, helper::Point, int32_t, Direction>> toVisit;
        Distance dist(map.size(), {map[0].size(), {maxStepsInOneDirection + 1, std::vector<int32_t>(4, INT_MAX)}});

        helper::Point start = {0, 0};
        for (auto dir : {UP, DOWN, LEFT, RIGHT}) {
            toVisit.push({0, start, 0, dir});
            dist[0][0][0][dir] = 0;
        }

        while (!toVisit.empty()) {
            auto [d, pos, steps, dir] = toVisit.front();
            toVisit.pop();

            int32_t curDist = dist[pos.x][pos.y][steps][dir];
            if (d > curDist)
                continue;

            for (auto &[newP, nsteps, ndir] : getAdj(pos, dir, steps)) {
                if (!newP.isValid(0, 0, map.size() - 1, map[0].size() - 1))
                    continue;

                if (nsteps > maxStepsInOneDirection)
                    continue;

                int weight = map[newP.x][newP.y] - '0';
                auto &newDist = dist[newP.x][newP.y][nsteps][ndir];

                if (newDist > curDist + weight) {
                    newDist = curDist + weight;
                    toVisit.push({newDist, newP, nsteps, ndir});
                }
            }
        }

        return dist;
    }

    int32_t getLeastHeatLoss() const {
        int32_t sol = INT_MAX;

        Distance dist = dijkstra();

        for (auto dir : {UP, DOWN, LEFT, RIGHT})
            for (int steps = 4; steps <= 10; ++steps)
                sol = std::min(sol, dist[map.size() - 1][map[0].size() - 1][steps][dir]);

        return sol;
    }
};

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    HeatMap heat;

    while (!input.eof()) {

        std::string inputRow;
        std::getline(input, inputRow);

        if (inputRow.empty()) continue;

        heat.map.push_back(inputRow);
    }

    std::cout << heat.getLeastHeatLoss() << std::endl;

    return 0;
}
