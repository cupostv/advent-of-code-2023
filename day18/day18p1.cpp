#include "../helpers/includes.hpp"

#define INPUT "input.txt"

struct DigPlanEntry {
    helper::Point dir;
    int64_t ammount;
    std::string colorCode;

    helper::Point dig(const helper::Point &from) const {
        return from + dir * ammount;
    }
};

helper::GridPolygon dig(const std::vector<DigPlanEntry> &plan) {
    helper::GridPolygon digMap;

    helper::Point cur = {0, 0};
    digMap.points.push_back(cur);

    for (auto &entry : plan) {
        cur = entry.dig(cur);
        digMap.points.push_back(cur);
    }

    return digMap;
}

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    int64_t sum = 0;

    std::vector<DigPlanEntry> digPlan;

    while (!input.eof()) {

        std::string inputRow;
        std::getline(input, inputRow);

        if (inputRow.empty()) continue;

        std::stringstream inputStream(inputRow);

        char ch;
        inputStream >> ch;

        DigPlanEntry entry;
        if (ch == 'R') entry.dir = helper::Point::RIGHT;
        else if (ch == 'L') entry.dir = helper::Point::LEFT;
        else if (ch == 'D') entry.dir = helper::Point::DOWN;
        else if (ch == 'U') entry.dir = helper::Point::UP;

        inputStream >> entry.ammount;

        std::string dummy;
        std::getline(inputStream, dummy, ' ');
        std::getline(inputStream, dummy, '(');
        std::getline(inputStream, dummy, ')');

        entry.colorCode = dummy;

        digPlan.push_back(entry);
    }

    auto polygon = dig(digPlan);

    std::cout << std::fixed << polygon.area() << std::endl;

    return EXIT_SUCCESS;
}
