#include "../helpers/includes.hpp"

#define INPUT "input.txt"

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

        for (bool in : inputs | std::views::values) {
            if (!in) {
                return {true, true};
            }
        }
        return {true, false};
    }

    bool getState() const {
        for (bool in : inputs | std::views::values) {
            if (in) {
                return true;
            }
        }
        return false;
    }
};

struct ModuleList {
    std::unordered_map<std::string, std::shared_ptr<Module>> list;
    std::queue<std::tuple<std::string, std::string, bool>> inputModules;


    void resetConjuctions() {
        for (auto &[in, module] : list)
            for (auto &out : module->outputs)
                if (list.contains(out))
                    if (auto conj = dynamic_pointer_cast<Conjuction>(list[out]))
                        conj->reset(in);
    }

    bool allOff() const {
        for (auto module : list | std::views::values)
            if (module->getState()) return false;

        return true;
    }

    auto getEmptyCounts(const std::string& input) const {
        std::unordered_map<std::string, int64_t> counts;
        for (const auto &[in, module] : list) {
            if (helper::find(module->outputs, input)) {
                counts[in] = 0;
            }
        }
        return counts;
    }

    std::tuple<int64_t, int64_t> broadcast() {
        int64_t lowCount = 0;
        int64_t highCount = 0;
        std::queue<std::tuple<std::string, std::string, bool>> initialModules = inputModules;
        while(!initialModules.empty()) {
            auto &[input, output, value] = initialModules.front();
            initialModules.pop();
            if (value) highCount++; else lowCount++;

            if (!list.contains(output)) continue;

            auto [pass, res] = list[output]->process(input, value);
            if (pass) {
                for (auto &out : list[output]->outputs) {
                    initialModules.push({output, out, res});
                }
            }
        }
        return {lowCount, highCount};
    }
};

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    ModuleList modules;

    std::string delimiter = " -> ";

    while (!input.eof()) {

        std::string inputRow;

        std::getline(input, inputRow);
        if (inputRow.empty()) break;

        std::string left = inputRow.substr(0, inputRow.find(delimiter));
        std::string right = inputRow.substr(inputRow.find(delimiter) + delimiter.size(), inputRow.size());
        helper::string::removeSpaces(right);
        std::stringstream ss(right);
        std::string out;
        if (left == "broadcaster") {
            while (!ss.eof()) {
                std::getline(ss, out, ',');
                modules.inputModules.push({left, out, false});
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
            modules.list[left] = m;
        }
    }

    modules.resetConjuctions();

    int64_t lowCount = 0;
    int64_t highCount = 0;
    int64_t count = 0;

    do {
        auto initialInputs = modules.inputModules;
        auto [low, high] = modules.broadcast();
        lowCount += low + 1; // button press
        highCount += high;
        count++;
    } while(count != 1000);

    std::cout << lowCount * highCount << std::endl;

    return EXIT_SUCCESS;
}
