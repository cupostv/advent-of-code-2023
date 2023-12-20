#include "../helpers/includes.hpp"

#define INPUT "input.txt"

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

using Direction = std::pair<int32_t, int32_t>;
using Position = std::pair<int32_t, int32_t>;
using Movement = std::pair<Position, Direction>;

static const Direction RIGHT = {0, 1};
static const Direction LEFT = {0, -1};
static const Direction UP = {-1, 0};
static const Direction DOWN = {1, 0};

static std::vector<std::string> grid;

struct Node {
    Position pos;
    Direction dir;
    int32_t steps;

    void dump() {
        std::cout << "(" << pos.first << ", " << pos.second << ")(" << dir.first << ", " << dir.second << ") " << steps << std::endl;
    }

    bool operator==(const Node &n) const {
        return pos == n.pos && steps == n.steps;
    }
};

struct NodeHash {
public:
  std::size_t operator()(const Node &x) const
  {
    std::size_t seed = 0;
    hash_combine(seed, x.pos.first);
    hash_combine(seed, x.pos.second);
    // hash_combine(seed, x.dir.first);
    // hash_combine(seed, x.dir.second);
    hash_combine(seed, x.steps);

    return seed;
  }
};

using Edge = std::pair<Node, int32_t>;
using Graph = std::unordered_map<Node, std::vector<Edge>, NodeHash>;

Node getNextSameDir(Node node) {
    auto [row, col] = node.pos;
    Direction currentDir = node.dir;
    if (currentDir == Direction{0, 0}) currentDir = RIGHT;
    return {{row + currentDir.first, col + currentDir.second}, currentDir, node.steps + 1};
}

std::array<Node, 2> getNextRotateDir(Node node) {
    auto [row, col] = node.pos;
    Direction currentDir = node.dir;
    if (currentDir == Direction{0, 0}) currentDir = RIGHT;
    std::array<Node, 2> ret;
    ret[0] = {{row - currentDir.second, col - currentDir.first}, {-currentDir.second, -currentDir.first}, 0};
    ret[1] = {{row + currentDir.second, col + currentDir.first}, {currentDir.second, currentDir.first}, 0};
    return ret;
}

std::vector<Node> getNext(Node node) {
    std::vector<Node> ret;

    Direction currentDir = node.dir;

    auto [row, col] = node.pos;
    if (currentDir == Direction{0, 0}) currentDir = RIGHT;

    // We can go and rotate in the other direction
    ret.push_back({{row - currentDir.second, col - currentDir.first}, {-currentDir.second, -currentDir.first}, 0});
    ret.push_back({{row + currentDir.second, col + currentDir.first}, {currentDir.second, currentDir.first}, 0});

    // we can keep going in same direction
    ret.push_back({{row + currentDir.first, col + currentDir.second}, currentDir, node.steps + 1});

    return ret;
}

Graph createGraph() {
    Graph graph;

    Node start = {{0, 0}, {0, 0}, 0};

    auto isValid = [](Position pos) {
        return pos.first < grid.size() && pos.first >= 0 && pos.second < grid[0].size() && pos.second >= 0;
    };

    int32_t cnt = 0;

    auto isValidNode = [&](Node n) {
        return isValid(n.pos) && n.steps <= 3 && !graph.contains(n);
    };

    std::function<void(Node)> visit =
        [&](Node node) {

            if (!isValidNode(node)) return;

            auto n1 = getNextSameDir(node);
            if (isValidNode(n1)) {
                visit(n1);
                int32_t weight = grid[n1.pos.first][n1.pos.second] - '0';
                graph[node].push_back({n1, weight});
                n1.dump();
            }


            auto rotated = getNextRotateDir(node);
            for (auto n : rotated) {
                if (isValidNode(n)) {
                    n.dump();
                    visit(n);
                    int32_t weight = grid[n.pos.first][n.pos.second] - '0';
                    graph[node].push_back({n, weight});
                    n.dump();
                }
            }

        };
    visit(start);
    return graph;
}

// std::unordered_map<Position, std::vector<std::pair<Position, int32_t>>> createGraph() {
//     auto isValid = [](Position pos) {
//         return pos.first < grid.size() && pos.first >= 0 && pos.second < grid[0].size() && pos.second >= 0;
//     };

//     std::unordered_map<Position, std::tuple<int32_t, Position, int32_t>, pairhash> tentativeDistance;
//     std::unordered_map<Position, std::vector<std::pair<Position, int32_t>>> graph;

//     // One way graph
//     Position curNode = {0, 0};
//     Position curDir = {0, 0};

//     std::function<void(Position, Direction, int32_t)> move =
//         [&](Position pos, Direction dir, int32_t length) {
//             if (length >= 3) return;
//             if (!isValid(pos)) return;
//             // if (graph.contains(pos)) return;

//             auto neighbours = getNext(pos, dir, length);

//             for (auto [nPos, nDir] : neighbours) {
//                 if (nDir == dir) move(nPos, nDir, length + 1);
//                 else (nDir == dir) move(nPos, nDir, 0);
//             }

//             graph[pos].push_back({nPos, grid[nPos.first][nPos.second]});
//         };

//     // getNext(Position currentPos, Direction currentDir, int32_t sameDirLen) {

//     return graph;
// }

// auto dijkstra() -> vector<vector<vector<vector<int>>>>
// {
//     // distance, position, steps, direction
//     std::set<tuple<int32_t, Position, int32_t, Direction>> S;
//     int maxStepsInOneDirection = 3;
//     vector<vector<vector<vector<int>>>> dist(grid.size(), vector<vector<vector<int>>>(grid[0].size(), vector<vector<int>>(maxStepsInOneDirection + 1, vector<int>(4, INT_MAX))));

//     for (auto dir : {UP, DOWN, LEFT, RIGHT})
//     {
//         S.insert(make_tuple(0, {0, 0}, 0, dir));
//         dist[0][0][0][dir] = 0;
//     }

//     while (!S.empty())
//     {
//         auto [d, x, y, steps, dir] = *S.begin();
//         S.erase(S.begin());

//         if (d > dist[x][y][steps][dir])
//             continue;

//         for (auto [nx, ny, nsteps, ndir] : getAdj(x, y, dir, steps))
//         {
//             if (nx < 0 || nx >= this->input.size() || ny < 0 || ny >= this->input[0].size())
//                 continue;

//             if (nsteps > maxStepsInOneDirection)
//                 continue;

//             int weight = this->input[nx][ny] - '0';
//             if (dist[nx][ny][nsteps][ndir] > dist[x][y][steps][dir] + weight)
//             {
//                 dist[nx][ny][nsteps][ndir] = dist[x][y][steps][dir] + weight;
//                 S.insert({dist[nx][ny][nsteps][ndir], nx, ny, nsteps, ndir});
//             }
//         }
//     }

//     return dist;
// }


// int32_t calculateShortestDistance() {
//     constexpr int32_t maxStepsInOneDirection = 3;
//     std::unordered_map<Position, std::array<std::vector<Direction>, maxStepsInOneDirection + 1>, pairhash> visited;
//     // Value and direction and distace we have visited this node
//     std::unordered_map<Position, std::tuple<int32_t, Position, int32_t>, pairhash> tentativeDistance;

//     // std::set<tuple<int32_t, Position, int32_t, Direction>> S;

//     auto isValid = [](Position pos) {
//         return pos.first < grid.size() && pos.first >= 0 && pos.second < grid[0].size() && pos.second >= 0;
//     };

//     auto isVisited = [&](Position pos, int32_t steps, Direction dir) {
//         return visited.contains(pos) && std::find(visited[pos][steps].begin(), visited[pos][steps].end(), dir) != visited[pos][steps].end();
//     };

//     Position curNode = {0, 0};
//     Position endNode = {grid.size() - 1, grid[0].size() - 1};

//     // S.insert({0, curNode, 0, {0, 0}});

//     tentativeDistance[curNode] = {0, {0, 0}, 0};

//     do {

//         std::cout << curNode.first << ", " << curNode.second << " = " << std::get<2>(tentativeDistance[curNode]) << std::endl;

//         auto &[curDistance, curDirection, curSteps] = tentativeDistance[curNode];

//         auto neighbours = getNext(curNode, curDirection, curSteps);
//         Position minNode = curNode;

//         for (auto &[pos, dir, steps] : neighbours) {
//             if (!isValid(pos)) continue;
//             if (steps > 3) continue;
//             if (isVisited(pos, steps, dir)) continue;

//             int32_t weight = grid[pos.first][pos.second] - '0';
//             auto tentDistance = curDistance + weight;
//             std::tuple<int32_t, Position, int32_t> minTup = {tentDistance, dir, steps};
//             if (tentativeDistance.contains(pos)) {
//                 auto &[neighbourTentDistance, neighbourTentDirection, neighbourDirDistance] = tentativeDistance[pos];
//                 if (neighbourTentDistance < tentDistance) {
//                     minTup = tentativeDistance[pos];
//                 }
//             }
//             tentativeDistance[pos] = minTup;
//             if (curNode == minNode) {
//                 minNode = pos;
//             } else {
//                 if (std::get<0>(minTup) < std::get<0>(tentativeDistance[pos])) minNode = pos;
//             }
//         }

//         visited[curNode][curSteps].push_back(curDirection);
//         curNode = minNode;

//     } while(curNode != endNode);


//     return 0;
// }

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    int64_t sum = 0;

    while (!input.eof()) {

        std::string inputRow;
        std::getline(input, inputRow);

        if (inputRow.empty()) continue;

        // std::stringstream inputStream(inputRow);

        grid.push_back(inputRow);

        // std::cout << inputRow << std::endl;
    }

    Graph g = createGraph();

    // std::cout << calculateShortestDistance() << std::endl;

    return EXIT_SUCCESS;
}