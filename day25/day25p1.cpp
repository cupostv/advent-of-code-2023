#include "../helpers/includes.hpp"

#define INPUT "input.txt"

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    helper::Point startPos;

    while (!input.eof()) {

        std::string inputRow;

        std::getline(input, inputRow);
        if (inputRow.empty()) break;
    }

    return EXIT_SUCCESS;
}
