#include "../helpers/includes.hpp"

#define INPUT "input.txt"

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
        auto diff = getDifference();
        std::vector<int64_t> lastValues = {values.back()};
        while(!diff.allZeros()) {
            lastValues.push_back(diff.values.back());
            diff = diff.getDifference();
        }
        // X - lastValue = prev
        // X = prev + lastValue
        return std::accumulate(lastValues.crbegin(), lastValues.crend(), 0);
    }
};

struct Measurement {
    std::vector<Sequence> sequences;

    int64_t getExtrapolateForwardSum() const {
        return std::accumulate(sequences.begin(), sequences.end(), 0, [](int a, const Sequence &b) {
            return a + b.extrapolateForward();
        });
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

    return EXIT_SUCCESS;
}