#include "../helpers/includes.hpp"

#define INPUT "input.txt"

struct Map {
    std::string path;
    std::unordered_map<std::string, std::pair<std::string, std::string>> map;

    int64_t stepsFromAAAtoZZZ() {
        int64_t steps = 0;
        int64_t pos = 0;
        std::string key = "AAA";
        while (key != "ZZZ") {
            if (path[pos] == 'L') {
                key = map[key].first;
            }
            else if (path[pos] == 'R') {
                key = map[key].second;
            }
            steps++;
            pos = (pos + 1) % path.size();
        }
        return steps;
    }
};

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    Map m;

    while (!input.eof()) {

        std::string inputRow;
        std::getline(input, inputRow);
        if (inputRow.empty()) continue;

        m.path = inputRow;
        std::getline(input, inputRow); // empty line

        // JKT = (KFV, CFQ)

        while (!input.eof()) {
            std::getline(input, inputRow, '=');
            helper::string::removeSpaces(inputRow);
            std::string key = inputRow;
            std::string left;
            std::string right;
            std::getline(input, inputRow, '(');//dummy
            std::getline(input, left, ',');//dummy
            std::getline(input, right, ')');
            helper::string::removeSpaces(right);
            std::getline(input, inputRow);
            m.map[key] = {left, right};
        }

        std::cout << m.stepsFromAAAtoZZZ() << std::endl;
    }

    return EXIT_SUCCESS;
}