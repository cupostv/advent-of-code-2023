#include "../helpers/includes.hpp"

#define INPUT "input.txt"

struct EngineSchematic {
    static const int NOT_NUMBER = -1;

    std::unordered_map<char, std::vector<int>> engineMap;
    std::unordered_map<int, std::vector<int>> gears;

    std::pair<std::vector<int>, char> getAdjacentSymbol(const std::vector<std::string> &engineSchematic, int row, int col) const {
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

        if (tl != '.' && !helper::string::isAsciiDigit(tl)) {
            return std::make_pair(std::vector{row - 1, col - 1}, tl);
        }
        if (tm != '.' && !helper::string::isAsciiDigit(tm)) {
            return std::make_pair(std::vector{row - 1, col}, tm);
        }
        if (tr != '.' && !helper::string::isAsciiDigit(tr)) {
            return std::make_pair(std::vector{row - 1, col + 1}, tr);
        }

        if (ml != '.' && !helper::string::isAsciiDigit(ml)) {
            return std::make_pair(std::vector{row, col -1 }, ml);
        }
        if (mr != '.' && !helper::string::isAsciiDigit(mr)) {
            return std::make_pair(std::vector{row, col + 1}, mr);
        }

        if (bl != '.' && !helper::string::isAsciiDigit(bl)) {
            return std::make_pair(std::vector{row + 1, col - 1}, bl);
        }
        if (bm != '.' && !helper::string::isAsciiDigit(bm)) {
            return std::make_pair(std::vector{row + 1, col}, bm);
        }
        if (br != '.' && !helper::string::isAsciiDigit(br)) {
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

    void loadEngineSchematic(const std::vector<std::string> &engineSchematic) {
        std::string currentPart = "";
        char currentSymbol = '.';
        int currentId = 0;

        for (int32_t row = 0; row < engineSchematic.size(); row++) {
            for (int32_t col = 0; col < engineSchematic[row].size(); col++) {
                if (helper::string::isAsciiDigit(engineSchematic[row][col])) {
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

    int64_t getGearRatio() const {
        auto ratioView = gears | std::views::values | std::views::transform([](const auto &parts) {
            return parts.size() == 2 ? parts[0] * parts[1] : 0;
        });
        return std::accumulate(ratioView.begin(), ratioView.end(), (int64_t)0);
    }

    int32_t getPartsSum() const {
        auto partsSumView = engineMap | std::views::values | std::views::transform([](const auto &parts) {
            return std::accumulate(parts.begin(), parts.end(), 0);
        });
        return std::accumulate(partsSumView.begin(), partsSumView.end(), 0);
    }

};

class Engine {
    public:
        Engine(const std::vector<std::string> &engineSchematic) {
            map.loadEngineSchematic(engineSchematic);
        }

        int64_t getGearRatio() const {
            return map.getGearRatio();
        }

    private:
        EngineSchematic map;

        void loadEngine(const std::vector<std::string> &engineSchematic) {
            map.loadEngineSchematic(engineSchematic);
        }
};

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    std::vector<std::string> engineSchematic;

    while (!input.eof()) {
        std::string inputRow;
        std::getline(input, inputRow);
        engineSchematic.push_back(inputRow);
    }

    Engine engine(engineSchematic);

    std::cout << engine.getGearRatio() << std::endl;

    return EXIT_SUCCESS;
}