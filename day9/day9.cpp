#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <sstream>
#include <unordered_map>
#include <cmath>
#include <set>
#include <numeric>
#include <ranges>
#include <algorithm>
#include <list>

#define INPUT "input.txt"

bool isAsciiDigit(char digit) {
    return digit >= '0' && digit <= '9';
}

void removeSpaces(std::string &str) {
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
}

struct Sequence {
    std::vector<int64_t> values;

    Sequence getDifference() const {
        Sequence res;
        std::adjacent_difference(values.cbegin(), values.cend(), std::back_inserter(res.values));
        res.values.erase(res.values.begin());
        return res;
    }

    bool allZeros() const {
        return all_of(values.cbegin(), values.cend(), [](int64_t i) { return i == 0; });
    }

    int64_t extrapolateForward() const {
        auto diff = std::move(getDifference());
        std::vector<int64_t> lastValues = {values.back()};
        while(!diff.allZeros()) {
            lastValues.push_back(diff.values.back());
            diff = std::move(diff.getDifference());
        }
        // X - lastValue = prev
        // X = prev + lastValue
        return std::accumulate(lastValues.crbegin(), lastValues.crend(), 0);
    }

    int64_t extrapolateBackward() const {
        auto diff = std::move(getDifference());
        std::vector<int64_t> firstValues = {values.front()};
        while(!diff.allZeros()) {
            firstValues.push_back(diff.values.front());
            diff = std::move(diff.getDifference());
        }
        // lastValue - X = prev
        // X = lastValue - prev

        return std::accumulate(firstValues.crbegin(), firstValues.crend(), 0, [](auto prev, auto last) { return last - prev; });
    }

    void dump() const {
        for (auto val : values) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
};

struct Measurement {
    std::vector<Sequence> sequences;

    int64_t getExtrapolateForwardSum() const {
        return std::accumulate(sequences.begin(), sequences.end(), 0, [](int a, const Sequence &b) {
            return a + b.extrapolateForward();
        });
    }

    int64_t getExtrapolateBackwardSum() const {
        return std::accumulate(sequences.begin(), sequences.end(), 0, [](int a, const Sequence &b) {
            return a + b.extrapolateBackward();
        });
    }

    void dump() const {
        for (auto seq : sequences) {
            seq.dump();
        }
    }
};

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    Measurement m;

    while (!input.eof()) {

        std::string inputRow;
        std::getline(input, inputRow);
        if (inputRow.empty()) continue;

        std::stringstream inputStream(inputRow);
        Sequence s;
        while(!inputStream.eof()) {
            int64_t seq;
            inputStream >> seq;
            s.values.push_back(seq);
        }
        m.sequences.push_back(s);
    }

    std::cout << m.getExtrapolateForwardSum() << std::endl;
    std::cout << m.getExtrapolateBackwardSum() << std::endl;

    return EXIT_SUCCESS;
}