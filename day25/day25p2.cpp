#include "../helpers/includes.hpp"
#include <random>

#define INPUT "input.txt"

using Graph = std::unordered_map<int32_t, std::set<int32_t>>;

Graph graph;

std::vector<int32_t> getMinPath(const int32_t start, const int32_t end) {
    if (start == end) return {};

    std::set<int32_t> visited;

    // BFS
    std::queue<std::vector<int32_t>> queue;
    queue.push({start});

    while (!queue.empty()) {
        auto path = queue.front();
        queue.pop();
        auto node = path.back();
        if (node == end)
            return path;
        if (helper::find(visited, node)) continue;
        visited.insert(node);

        for (auto adjacent : graph[node]) {
            auto newPath = path;
            newPath.push_back(adjacent);
            queue.push(newPath);
        }
    }
    return {};
}

int32_t getGraphSize(const int32_t start) {
    std::set<int32_t> visited;
    int32_t count = 0;

    std::function<void(int32_t)> visit =
        [&](int32_t node) {
            if (helper::find(visited, node)) return;
            count++;
            visited.insert(node);
            for (auto adj : graph[node]) {
                visit(adj);
            }
        };

    visit(start);

    return count;
}

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    std::map<std::string, int32_t> nameMap;

    int32_t idx = 0;
    while (!input.eof()) {

        std::string inputRow;

        std::getline(input, inputRow);
        if (inputRow.empty()) break;

        std::stringstream inputStream(inputRow);

        std::string key;
        std::getline(inputStream, key, ':');

        std::string temp;

        std::getline(inputStream, temp, ' ');

        while (!inputStream.eof()) {
            std::getline(inputStream, temp, ' ');

            auto keyIdx = nameMap.contains(key) ? nameMap[key] : nameMap[key] = idx++;
            auto tempIdx = nameMap.contains(temp) ? nameMap[temp] : nameMap[temp] = idx++;
            graph[keyIdx].insert(tempIdx);
            graph[tempIdx].insert(keyIdx);
        }
    }

    std::vector<std::pair<int32_t, int32_t>> counts(graph.size(), {0,0});

    for (int32_t iter = 0; iter < 500; iter++) {
        // Get two nodes randomly
        auto rand1 = std::next(graph.begin(), std::rand() % graph.size());
        auto rand2 = std::next(graph.begin(), std::rand() % graph.size());
        auto path = getMinPath(rand1->first, rand2->first);
        for (auto n : path) {
            counts[n].first++;
            counts[n].second = n;
        }
    }

    std::ranges::sort(counts, std::greater());

    // If we remove top 6 nodes, we will remain with two independent graphs
    // Find their size, add 3 nodes on each side, and multiply
    for (auto [count, key] : counts | std::ranges::views::take(6)) {
        for (auto adj : graph[key])
            graph[adj].erase(key);
        graph.erase(key);
    }

    auto left = getGraphSize(graph.begin()->first);
    auto right = graph.size() - left;
    // Removed 3 nodes from each side, add them back
    left += 3;
    right += 3;

	std::cout << left * right << std::endl;

    return EXIT_SUCCESS;
}
