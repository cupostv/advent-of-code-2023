#include "../helpers/includes.hpp"

#define INPUT "input.txt"

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