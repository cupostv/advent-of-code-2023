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
};

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    ModuleList modules;

    std::string delimiter = " -> ";

    int32_t i = 0;
    std::string rxInput;
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
            if (helper::find(m->outputs, "rx")) {
                rxInput = left;
            }
            modules.list[left] = m;
        }
    }

    modules.resetConjuctions();
    auto counts = modules.getEmptyCounts(rxInput);

    int64_t count = 0;

    do {
        auto initialInputs = modules.inputModules;
        count++;
        while(!initialInputs.empty()) {
            auto &[input, output, value] = initialInputs.front();
            initialInputs.pop();

            if (!modules.list.contains(output)) continue;

            auto [pass, res] = modules.list[output]->process(input, value);
            if (!pass) continue;

            if (res && counts.contains(output) && counts[output] == 0)
                counts[output] = count;
            for (auto &out : modules.list[output]->outputs) {
                initialInputs.push({output, out, res});
            }
        }

        if (std::ranges::count(counts | std::views::values, 0) == 0) break;
    } while(!modules.allOff());

    int64_t min = 1;
    for (auto c : counts | std::views::values)
        min = std::lcm(min, c);

    std::cout << min << std::endl;

    return EXIT_SUCCESS;
}
