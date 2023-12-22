#include "../helpers/includes.hpp"

#define INPUT "input.txt"

struct BoatRace {
    int64_t time;
    int64_t record;

    int64_t getRemainingTime(int64_t holdTime) const {
        return std::max(time - holdTime, (int64_t)0);
    }

    int64_t getSpeed(int64_t holdTime) const {
        return 1 * holdTime;
    }

    int64_t getDistance(int64_t holdTime) const {
        return getRemainingTime(holdTime) * getSpeed(holdTime);
    }

    int64_t getPossibleRecordBeats() const {
        // ax^{2}+bx+c=0
        // D=b^{2}-4ac
        // (-speed*speed + speed*time - record) > 0
        auto D = time * time + 4 * -record;
        auto sqrtD = (int64_t)std::sqrt(D); //+-
        auto x1 = (-time + sqrtD) / 2;
        auto x2 = (-time - sqrtD) / 2;
        return x1 - x2;
    }
};

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    std::vector<int64_t> times;
    std::vector<int64_t> records;
    std::vector<BoatRace> races;
    while (!input.eof()) {

        std::string inputRow;
        std::getline(input, inputRow);
        if (inputRow.empty()) continue;

        std::string dummy;
        std::stringstream inputStream(inputRow);
        std::getline(inputStream, dummy, ':');
        std::getline(inputStream, dummy);

        dummy.erase(remove_if(dummy.begin(), dummy.end(), isspace), dummy.end());
        times.push_back(std::stoll(dummy));

        std::getline(input, inputRow);
        if (inputRow.empty()) continue;
        std::stringstream inputStream1(inputRow);
        std::getline(inputStream1, dummy, ':');
        std::getline(inputStream1, dummy);

        dummy.erase(remove_if(dummy.begin(), dummy.end(), isspace), dummy.end());
        records.push_back(std::stoll(dummy));
    }

    races.push_back({times[0], records[0]});

    int64_t mul = 1;
    for (auto race : races) {
        mul *= race.getPossibleRecordBeats();
    }

    std::cout << mul << std::endl;

    return EXIT_SUCCESS;
}