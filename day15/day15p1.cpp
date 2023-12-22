#include "../helpers/includes.hpp"

#define INPUT "input.txt"

int32_t hash(char c, int32_t current = 0) {
    current += c;
    current *= 17;
    current %= 256;
    return current;
}

int32_t hash(const std::string &s) {
    int32_t current = 0;
    for (auto c : s) {
        current = hash(c, current);
    }
    return current;
}

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    int64_t sum = 0;

    while (!input.eof()) {

        std::string inputRow;
        std::getline(input, inputRow);

        if (inputRow.empty()) continue;
        std::stringstream inputStream(inputRow);

        while (!inputStream.eof()) {
            std::string seq;
            std::getline(inputStream, seq, ',');

            sum += hash(seq);
        }
    }

    std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}