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
        int64_t count = 0;
        for (int32_t holdTime = 1; holdTime < time; holdTime++) {
            if (getDistance(holdTime) > record) count++;
        }
        return count;
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
        while(!inputStream.eof()) {
            int64_t time;
            inputStream >> time;
            times.push_back(time);
        }

        std::getline(input, inputRow);
        if (inputRow.empty()) continue;
        inputStream.str(inputRow);
        inputStream.clear();
        std::getline(inputStream, dummy, ':');
        while(!inputStream.eof()) {
            int64_t record;
            inputStream >> record;
            records.push_back(record);
        }
    }

    for (int32_t i = 0; i < times.size(); i++)
        races.push_back({times[i], records[i]});

    int64_t mul = 1;
    for (auto race : races) {
        mul *= race.getPossibleRecordBeats();
    }

    std::cout << mul << std::endl;

    return EXIT_SUCCESS;
}