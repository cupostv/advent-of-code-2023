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

struct Lens {
    std::string label;
    int64_t focalLength;

    int32_t boxIdx() const {
        return hash(label);
    }

    friend auto operator==(const Lens &l1, const Lens &l2) {
        return l1.label == l2.label;
    }
};

struct Box {
    std::vector<Lens> lenses;

    void addLens(const Lens &lens) {
        auto res = std::find(lenses.begin(), lenses.end(), lens);
        if (res == lenses.end()) {
            lenses.push_back(lens);
        } else {
            // replace lens
            *res = lens;
        }
    }

    void removeLens(const Lens &lens) {
        auto res = std::find(lenses.begin(), lenses.end(), lens);
        if (res != lenses.end()) {
            lenses.erase(std::find(lenses.begin(), lenses.end(), lens));
        }
    }
};

std::array<Box, 256> boxes;

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

            std::string tmp;
            std::stringstream seqStream(seq);
            std::getline(seqStream, tmp, '=');
            if (tmp != seq) {
                // id = focal length
                Lens lens;
                lens.label = tmp;
                std::getline(seqStream, tmp, '=');
                lens.focalLength = std::stoi(tmp);
                boxes[lens.boxIdx()].addLens(lens);
            } else {
                seq.pop_back();
                Lens lens;
                lens.label = seq;
                boxes[lens.boxIdx()].removeLens(lens);
            }
        }
    }

    for (int32_t box = 0; box < boxes.size(); box++) {
        auto &lenses = boxes[box].lenses;
        for (int32_t lens = 0; lens < lenses.size(); lens++) {
            sum += (box + 1) * (lens + 1) * lenses[lens].focalLength;
        }
    }

    std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}