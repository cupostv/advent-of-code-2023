#include "../helpers/includes.hpp"

#define INPUT "input.txt"

const std::unordered_map<std::string, int32_t> numbers = {
    {"0", 0},
    {"1", 1},
    {"2", 2},
    {"3", 3},
    {"4", 4},
    {"5", 5},
    {"6", 6},
    {"7", 7},
    {"8", 8},
    {"9", 9},
    {"zero", 0},
    {"one", 1},
    {"two", 2},
    {"three", 3},
    {"four", 4},
    {"five", 5},
    {"six", 6},
    {"seven", 7},
    {"eight", 8},
    {"nine", 9},
};

int32_t decodeCalibrationEntry(const std::string_view entry) {

    int32_t minFrontPos = entry.size();
    int32_t frontNumber = 0;
    int32_t maxBackPos = -1;
    int32_t backNumber = 0;

    for (const auto &[key, value] : numbers) {
        int32_t frontPos = entry.find(key);
        if (frontPos != std::string_view::npos && frontPos < minFrontPos) {
            minFrontPos = frontPos;
            frontNumber = value;
        }
        int32_t backPos = entry.rfind(key);
        if (backPos != std::string_view::npos && backPos > maxBackPos) {
            maxBackPos = backPos;
            backNumber = value;
        }
    }

    return frontNumber * 10 + backNumber;
}

int32_t main() {

    std::ifstream calibration;
    calibration.open(INPUT);

    if (!calibration) return EXIT_FAILURE;

    int32_t sum = 0;
    while (!calibration.eof()) {
        std::string calibrationEntry;
        calibration >> calibrationEntry;
        sum += decodeCalibrationEntry(calibrationEntry);
    }

    std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}