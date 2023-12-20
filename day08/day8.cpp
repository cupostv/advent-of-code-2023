#include "../helpers/includes.hpp"

#define INPUT "input.txt"

bool isAsciiDigit(char digit) {
    return digit >= '0' && digit <= '9';
}

void removeSpaces(std::string &str) {
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
}

// struct Node {
//     std::string name;

//     auto operator<=>(const Hand& r) const = default;
// };

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

    int64_t stepsFromAllAtoAllZ() {

        std::vector<std::string> keys;
        for (auto key : map | std::views::keys) {
            if (key.ends_with('A')) {
                keys.push_back(key);
            }
        }

        int64_t steps = 0;
        int64_t pos = 0;

        while (!std::all_of(keys.begin(), keys.end(), [](std::string &key) { return key.ends_with('Z'); })) {
            if (path[pos] == 'L') {
                for (auto &key : keys) {
                    key = map[key].first;
                }
            }
            else if (path[pos] == 'R') {
                for (auto &key : keys) {
                    key = map[key].second;
                }
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
            removeSpaces(inputRow);
            std::string key = inputRow;
            std::string left;
            std::string right;
            std::getline(input, inputRow, '(');//dummy
            std::getline(input, left, ',');//dummy
            std::getline(input, right, ')');
            removeSpaces(right);
            std::getline(input, inputRow);
            m.map[key] = {left, right};
        }

        // std::stringstream inputStream(inputRow);

        std::cout << m.stepsFromAllAtoAllZ() << std::endl;
    }

    return EXIT_SUCCESS;
}