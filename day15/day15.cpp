#include "../helpers/includes.hpp"

#define INPUT "input.txt"

template <typename T>
void printVector(const std::vector<T>& c)
{
    for (auto i : c)
        std::cout << i << ' ';
    std::cout << '\n';
}

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

struct pairhash {
public:
  template <typename T, typename U>
  std::size_t operator()(const std::pair<T, U> &x) const
  {
    std::size_t seed = 0;
    hash_combine(seed, x.first);
    hash_combine(seed, x.second);

    return seed;
  }
};

// Determine the ASCII code for the current character of the string.
// Increase the current value by the ASCII code you just determined.
// Set the current value to itself multiplied by 17.
// Set the current value to the remainder of dividing itself by 256.

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

    friend auto operator<=>(const Lens &l1, const Lens &l2) {
        return l1.label <=> l2.label;
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
            std::cout << "Remove " << lens.label << std::endl;
            lenses.erase(res);
        }
    }
};

std::array<Box, 256> boxes;

void printBoxes() {
    for (int32_t box = 0; box < boxes.size(); box++) {
        auto &lenses = boxes[box].lenses;
        for (int32_t lens = 0; lens < lenses.size(); lens++) {
            std::cout << "Box " << box << ": [" << lenses[lens].label << " " << lenses[lens].focalLength << "]" << std::endl;
        }
    }
}

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    int64_t sum = 0;
    // std::vector<std::string> map;
    while (!input.eof()) {

        std::string inputRow;
        std::getline(input, inputRow);

        if (inputRow.empty()) continue;

        std::stringstream inputStream(inputRow);
        // map.push_back(inputRow);
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

        // std::cout << inputRow << std::endl;
    }

    printBoxes();

    for (int32_t box = 0; box < boxes.size(); box++) {
        auto &lenses = boxes[box].lenses;
        for (int32_t lens = 0; lens < lenses.size(); lens++) {
            sum += (box + 1) * (lens + 1) * lenses[lens].focalLength;
        }
    }

    std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}