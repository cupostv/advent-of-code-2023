#include "../helpers/includes.hpp"

#define INPUT "input.txt"

auto split(const std::vector<std::string> &grid, int32_t splitPoint) {
    // Split vector into two vectors [0, splitPoint) [splitPoint, end]
    // But make them equal by cutting `left` to the left or `right` to the right
    auto min = std::min(splitPoint, (int32_t)grid.size() - splitPoint);
    return std::make_tuple(
            grid.begin() + splitPoint - min,                // left.begin
            grid.begin() + splitPoint,                      // left.end
            grid.begin() + splitPoint,                      // right.begin
            grid.end() - (grid.size() - splitPoint - min)); // right.end
}

std::vector<std::string> transpose(const std::vector<std::string> &grid) {
    std::vector<std::string> transposed(grid[0].size());
    for (int32_t i = 0; i < grid[0].size(); i++) {
        for (int32_t j = 0; j < grid.size(); j++) {
            transposed[i] += grid[j][i];
        }
    }
    return transposed;
}

int32_t strHammingDistance(const std::string &left, const std::string &right) {
    int32_t distance = 0;
    for (int32_t i = 0; i < left.size(); i++) {
        distance += (left[i] != right[i]);
    }
    return distance;
}

template<class iterator_type1, class iterator_type2>
int32_t hammingDistance(const iterator_type1 &leftB, const iterator_type1 &leftE, const iterator_type2 &rightB) {
    // Assuming vectors are of same length
    int32_t distance = 0;
    auto j = rightB;
    auto i = leftB;
    for (; i != leftE; i++, j++) {
        distance += strHammingDistance(*i, *j);
    }
    return distance;
}

int32_t findReflection(const std::vector<std::string> &grid, bool hasSmudge) {
    // Smudge allows for 1 different element
    for (int32_t row = 1; row < grid.size(); row++) {
        // Get equal subgrid iters by splitting grid on row point
        const auto [lBegin, lEnd, rBegin, rEnd] = split(grid, row);
        // Get hamming distance between left and reversed right
        if (hammingDistance(lBegin, lEnd, std::make_reverse_iterator(rEnd)) == hasSmudge)
            // Found reflection
            return row;
    }
    return -1;
}

int32_t solve(const std::vector<std::string> &grid, bool smudge = false) {
    // row reflection
    auto reflection = findReflection(grid, smudge);
    if (reflection != -1) {
        return reflection * 100;
    }
    else {
        // column reflection
        reflection = findReflection(transpose(grid), smudge);
        if (reflection == -1) {
            std::cout << "No reflection at all in the pattern!\n";
        }
        return reflection;
    }
}

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    std::vector<std::string> pattern;

    int32_t sum1 = 0;
    int32_t sum2 = 0;
    while (!input.eof()) {

        std::string inputRow;
        std::getline(input, inputRow);

        if (input.eof()) {
            pattern.push_back(inputRow);
        }
        if (inputRow.empty() || input.eof()) {
            // call the function for pattern
            sum1 += solve(pattern, false);
            sum2 += solve(pattern, true);
            pattern.clear();
        } else {
            pattern.push_back(inputRow);
        }
    }

    std::cout << "Part 1: " << sum1 << std::endl;
    std::cout << "Part 2: " << sum2 << std::endl;

    return EXIT_SUCCESS;
}