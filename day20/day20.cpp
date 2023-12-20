#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <map>
#include <cmath>
#include <set>
#include <numeric>
#include <ranges>
#include <algorithm>
#include <list>
#include <memory>
#include <climits>
#include <cassert>

#define INPUT "input.txt"

void removeSpaces(std::string &str) {
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
}

struct Module {
    std::vector<std::string> outputs;
    virtual std::pair<bool, bool> process(const std::string &inputName, bool inputValue) = 0;

    virtual bool getState() const = 0;
};

struct FlipFlop : Module {
    bool state = false;

    std::pair<bool, bool> process(const std::string &inputName, bool inputValue) {
        if (inputValue) {
            // nothing happens;
            return {false, state};
        }
        state = !state;
        return {true, state};
    }

    bool getState() const {
        return state;
    }
};

struct Conjuction : Module {
    std::unordered_map<std::string, bool> inputs;

    void reset(const std::string &inputName) {
        inputs[inputName] = false;
    }

    std::pair<bool, bool> process(const std::string &inputName, bool inputValue) {
        inputs[inputName] = inputValue;

        bool out = false;
        for (bool in : inputs | std::views::values) {
            if (!in) {
                out = true;
                break;
            }
        }
        return {true, out};
    }

    bool getState() const {
        bool out = false;
        for (bool in : inputs | std::views::values) {
            if (in) {
                out = true;
                break;
            }
        }
        return out;
    }
};

struct ModuleList {
    std::unordered_map<std::string, std::shared_ptr<Module>> list;

    void resetConjuctions() {
        for (auto &[in, module] : list) {
            for (auto &out : module->outputs) {
                if (list.contains(out))
                    if (auto conj = dynamic_pointer_cast<Conjuction>(list[out]))
                        conj->reset(in);
            }
        }
    }

    bool allOff() const {
        for (auto module : list | std::views::values) {
            if (module->getState()) return false;
        }
        return true;
    }

    auto getEmptyCounts(const std::string& input) const {
        std::unordered_map<std::string, int64_t> counts;
        for (auto &[in, module] : list) {
            auto outs = module->outputs;
            if (std::ranges::find(outs, input) != outs.end()) {
                counts[in] = 0;
            }
        }
        return counts;
    }
};

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    ModuleList modules;
    std::queue<std::tuple<std::string, std::string, bool>> inputModules;

    std::string delimiter = " -> ";

    int32_t i = 0;
    std::string rxInput;
    while (!input.eof()) {

        std::string inputRow;

        std::getline(input, inputRow);
        if (inputRow.empty()) break;

        std::string left = inputRow.substr(0, inputRow.find(delimiter));
        std::string right = inputRow.substr(inputRow.find(delimiter) + delimiter.size(), inputRow.size());
        removeSpaces(right);
        std::stringstream ss(right);
        std::string out;
        if (left == "broadcaster") {
            while (!ss.eof()) {
                std::getline(ss, out, ',');
                inputModules.push({left, out, false});
            }
        } else {
            char symbol = left[0];
            left = left.substr(1, left.size());
            std::shared_ptr<Module> m;
            if (symbol == '%') m = std::make_shared<FlipFlop>();
            if (symbol == '&') m = std::make_shared<Conjuction>();

            while (!ss.eof()) {
                std::getline(ss, out, ',');
                m->outputs.push_back(out);
            }
            if (std::ranges::find(m->outputs, "rx") != m->outputs.end()) {
                rxInput = left;
            }
            modules.list[left] = m;
        }
    }

    modules.resetConjuctions();
    auto counts = modules.getEmptyCounts(rxInput);

    int64_t lowCount = 0;
    int64_t highCount = 0;
    int64_t count = 0;

    do {
        auto initialInputs = inputModules;
        lowCount++; // button press
        count++;
        while(!initialInputs.empty()) {
            auto &[input, output, value] = initialInputs.front();
            initialInputs.pop();
            if (value) highCount++; else lowCount++;

            if (!modules.list.contains(output)) continue;

            auto [pass, res] = modules.list[output]->process(input, value);
            if (pass) {
                if (res && counts.contains(output) && counts[output] == 0)
                    counts[output] = count;
                for (auto &out : modules.list[output]->outputs) {
                    initialInputs.push({output, out, res});
                }
            }
        }

        if (count == 1000) {
            std::cout << "Part 1: " << lowCount * highCount << std::endl;
        }

        if (std::ranges::count(counts | std::views::values, 0) == 0) break;
    } while(!modules.allOff());

    int64_t min = 1;
    for (auto c : counts | std::views::values)
        min = std::lcm(min, c);

    std::cout << "Part 2: " << min << std::endl;

    return EXIT_SUCCESS;
}
