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

        std::string dummy;
        std::getline(inputStream, dummy, ' ');
        std::getline(inputStream, dummy, '(');
        std::getline(inputStream, dummy, ')');

        entry.colorCode = dummy;

        std::string first(entry.colorCode.begin() + 1, entry.colorCode.begin() + 6);
        std::string second(entry.colorCode.begin() + 6, entry.colorCode.end());

        std::stringstream ss;
        ss << std::hex << first;
        ss >> entry.ammount;

        int32_t direction;
        std::stringstream ss1;
        ss1 << std::hex << second;
        ss1 >> direction;

        if (direction == 0) entry.dir = helper::Point::RIGHT;
        else if (direction == 2) entry.dir = helper::Point::LEFT;
        else if (direction == 1) entry.dir = helper::Point::DOWN;
        else if (direction == 3) entry.dir = helper::Point::UP;

        digPlan.push_back(entry);
    }

    auto polygon = dig(digPlan);

    std::cout << std::fixed << polygon.area() << std::endl;

    return EXIT_SUCCESS;
}
