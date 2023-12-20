#include "../helpers/includes.hpp"

#define CALIBRATION "calibration.txt"

const std::array<std::string, 10> numbers = {
    "zero",
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine"
};

int32_t getNumberSubstring(const std::string_view str) {
    if (str.size() < 3) return -1;
    int32_t actualNum = -1;
    int32_t minPosition = str.size();
    for (int32_t i = 0; i < numbers.size(); i++) {
        auto test = str.find(numbers[i]);
        if (test != std::string_view::npos && test < minPosition) {
            minPosition = test;
            actualNum = i;
        }
    }

    return actualNum;
}

int32_t decodeCalibrationEntry(const std::string_view entry) {

    int32_t front = 0;
    int32_t back = entry.size() - 1;

    int32_t firstDigit = -1;
    int32_t lastDigit = -1;

    while (firstDigit < 0 || lastDigit < 0) {
        if (firstDigit < 0) {
            auto strNumber = getNumberSubstring(entry.substr(0, front));
            if (strNumber > 0)
                firstDigit = strNumber;
            else if (std::isdigit(entry[front]))
                firstDigit = entry[front] - '0';
        }
        if (lastDigit < 0) {
            auto strNumber = getNumberSubstring(entry.substr(back, entry.size() - 1));
            if (strNumber > 0)
                lastDigit = strNumber;
            else if (std::isdigit(entry[back]))
                lastDigit = entry[back] - '0';
        }

        front++;
        back--;
    }

    return firstDigit * 10 + lastDigit;
}

int32_t main() {

    std::ifstream calibration;
    calibration.open(CALIBRATION);

    if (!calibration) return EXIT_FAILURE;

    int32_t sum = 0;
    while (!calibration.eof()) {
        std::string calibrationEntry;
        calibration >> calibrationEntry;
        sum += decodeCalibrationEntry(calibrationEntry);
    }

    std::cout << sum << std::endl;

    getNumberSubstring("jedantwo");

    return EXIT_SUCCESS;
}