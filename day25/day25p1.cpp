#include "../helpers/includes.hpp"
#include <random>

#define INPUT "input.txt"

using Graph = std::unordered_map<int32_t, std::set<int32_t>>;

Graph graph;

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
        }
    }

    auto globalMinCut = [](std::vector<std::vector<int>>& mat) {
		std::pair<int, std::vector<int>> best = { std::numeric_limits<int>::max(), {} };
		int n = mat.size();
		std::vector<std::vector<int>> co(n);

		for (int i = 0; i < n; i++)
			co[i] = { i };

		for (int ph = 1; ph < n; ph++) {
			std::vector<int> w = mat[0];
			size_t s = 0, t = 0;
			for (int it = 0; it < n - ph; it++) {
				w[t] = std::numeric_limits<int>::min();
				s = t, t = max_element(w.begin(), w.end()) - w.begin();
				for (int i = 0; i < n; i++) w[i] += mat[t][i];
			}
			best = min(best, { w[t] - mat[t][t], co[t] });
			co[s].insert(co[s].end(), co[t].begin(), co[t].end());
			for (int i = 0; i < n; i++) mat[s][i] += mat[t][i];
			for (int i = 0; i < n; i++) mat[i][s] = mat[s][i];
			mat[0][t] = std::numeric_limits<int>::min();
		}

		return best;
	};

	std::vector<std::vector<int32_t>> adj(nameMap.size(), std::vector<int32_t>(nameMap.size(), 0));

	for (const auto& [k, connections] : graph) {
        for (auto c : connections) {
            adj[k][c] = adj[c][k] = 1;
        }
    }

	auto min = globalMinCut(adj);

	std::cout << min.second.size() * (nameMap.size() - min.second.size()) << std::endl;

    return EXIT_SUCCESS;
}
