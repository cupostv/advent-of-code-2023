#include "../helpers/includes.hpp"

#define INPUT "input.txt"

bool isAsciiDigit(char digit) {
    return digit >= '0' && digit <= '9';
}

struct EngineSchematic {
    static const int NOT_NUMBER = -1;

    std::unordered_map<char, std::vector<int>> engineMap;
    std::unordered_map<int, std::vector<int>> gears;

    std::pair<std::vector<int>, char> getAdjacentSymbol(const std::vector<std::vector<char>> &engineSchematic, int row, int col) const {
        char tl = '.', tm = '.', tr = '.';
        char ml = '.', mr = '.';
        char bl = '.', bm = '.', br = '.';

        if (row - 1 >= 0 && col - 1 >= 0)
            tl = engineSchematic[row - 1][col - 1];
        if (row - 1 >= 0 && col >= 0)
            tm = engineSchematic[row - 1][col];
        if (row - 1 >= 0 && col + 1 <= engineSchematic[row - 1].size() - 1)
            tr = engineSchematic[row - 1][col + 1];

        if (row >= 0 && col - 1 >= 0)
            ml = engineSchematic[row][col - 1];
        if (row >= 0 && col + 1 <= engineSchematic[row].size() - 1)
            mr = engineSchematic[row][col + 1];

        if (row + 1 <= engineSchematic.size() - 1 && col - 1 >= 0)
            bl = engineSchematic[row + 1][col - 1];
        if (row + 1 <= engineSchematic.size() - 1 && col >= 0)
            bm = engineSchematic[row + 1][col];
        if (row + 1 <= engineSchematic.size() - 1 && col + 1 <= engineSchematic[row + 1].size() - 1)
            br = engineSchematic[row + 1][col + 1];

        if (tl != '.' && !isAsciiDigit(tl)) {
            return std::make_pair(std::vector{row - 1, col - 1}, tl);
        }
        if (tm != '.' && !isAsciiDigit(tm)) {
            return std::make_pair(std::vector{row - 1, col}, tm);
        }
        if (tr != '.' && !isAsciiDigit(tr)) {
            return std::make_pair(std::vector{row - 1, col + 1}, tr);
        }

        if (ml != '.' && !isAsciiDigit(ml)) {
            return std::make_pair(std::vector{row, col -1 }, ml);
        }
        if (mr != '.' && !isAsciiDigit(mr)) {
            return std::make_pair(std::vector{row, col + 1}, mr);
        }

        if (bl != '.' && !isAsciiDigit(bl)) {
            return std::make_pair(std::vector{row + 1, col - 1}, bl);
        }
        if (bm != '.' && !isAsciiDigit(bm)) {
            return std::make_pair(std::vector{row + 1, col}, bm);
        }
        if (br != '.' && !isAsciiDigit(br)) {
            return std::make_pair(std::vector{row + 1, col + 1}, br);
        }

        return std::make_pair(std::vector{0, 0}, '.');

    }

    void addPart(int id, char symbol, int partNumber) {
        if (symbol != '.') {
            engineMap[symbol].push_back(partNumber);
            if (symbol == '*') {
                gears[id].push_back(partNumber);
            }
        }
    }

    void loadEngineSchematic(const std::vector<std::vector<char>> &engineSchematic) {
        std::string currentPart = "";
        char currentSymbol = '.';
        int currentId = 0;

        for (int32_t row = 0; row < engineSchematic.size(); row++) {
            for (int32_t col = 0; col < engineSchematic[row].size(); col++) {
                if (isAsciiDigit(engineSchematic[row][col])) {
                        // Find surrounding symbol not .
                        currentPart += engineSchematic[row][col];

                        auto [location, symbol] = getAdjacentSymbol(engineSchematic, row, col);
                        int id = location[0] * engineSchematic[row].size() + location[1];
                        if (currentSymbol == '.' && symbol != '.') {
                            currentSymbol = symbol;
                            currentId = id;
                        }
                } else {
                    // Broken part reset everything
                    if (currentSymbol != '.') {
                        addPart(currentId, currentSymbol, std::stoi(currentPart));
                    }
                    currentSymbol = '.';
                    currentPart = "";
                }
            }
            // Broken part reset everything
            if (currentSymbol != '.') {
                addPart(currentId, currentSymbol, std::stoi(currentPart));
            }
            currentSymbol = '.';
            currentPart = "";
        }
    }

    long long getGearRatio() const {
        int ratio = 0;
        for (const auto&[id, parts] : gears) {
            if (parts.size() == 2) {
                ratio += parts[0] * parts[1];
            }
        }
        return ratio;
    }

    int getPartsSum() const {
        int sum = 0;
        for (const auto &[symbol, parts] : engineMap) {
            sum += std::accumulate(parts.begin(), parts.end(), 0);
        }

        return sum;
    }

    void dumpEngineParts() const {
        std::cout << "Parts sum: " << getPartsSum() << std::endl;

        std::cout << "Gear ration: " << getGearRatio() << std::endl;
    }
};

class Engine {
    public:
        Engine(const std::vector<std::vector<char>> &engineSchematic) {
            map.loadEngineSchematic(engineSchematic);
        }

        void dumpEngineParts() const {
            map.dumpEngineParts();
        }

    private:
        EngineSchematic map;

        void loadEngine(const std::vector<std::vector<char>> &engineSchematic) {
            map.loadEngineSchematic(engineSchematic);
        }
};

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    std::vector<std::vector<char>> engineSchematic;

    while (!input.eof()) {
        std::string inputRow;
        std::getline(input, inputRow);
        std::vector<char> vec(inputRow.begin(), inputRow.end());
        engineSchematic.push_back(vec);
    }

    Engine engine(engineSchematic);
    engine.dumpEngineParts();

    return EXIT_SUCCESS;
}