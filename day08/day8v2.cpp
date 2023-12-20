#include "../helpers/includes.hpp"

#define INPUT "input.txt"

bool isAsciiDigit(char digit) {
    return digit >= '0' && digit <= '9';
}

void removeSpaces(std::string &str) {
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
}

struct Map {
    static const char LEFT = 'L';
    static const char RIGHT = 'R';

    std::string path;
    std::unordered_map<std::string, std::pair<std::string, std::string>> map;

    int64_t stepsFromAllAtoAllZ() {

        auto endsWithA = [](const auto &k) {
            return k.ends_with('A');
        };

        int64_t leastCommonMul = 1;

        for (auto key : map | std::views::keys | std::views::filter(endsWithA)) {
            // Find ending path
            int64_t len = 0;
            int64_t pos = 0;

            while(!key.ends_with('Z')) {
                key = path[pos] == LEFT ? map[key].first : map[key].second;

                len++;
                pos = (pos + 1) % path.size();
            }

            leastCommonMul = std::lcm(leastCommonMul, len);
        }

        return leastCommonMul;
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