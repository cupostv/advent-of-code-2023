#include "../helpers/includes.hpp"

#define INPUT "input.txt"

struct Part {
    std::unordered_map<char, int64_t> ratings;

    int64_t totalRating() {
        int64_t total = 0;
        for (auto rating : ratings | std::views::values)
            total += rating;

        return total;
    }
};

enum class Action {
    SKIP,
    GOTO,
    REJECT,
    ACCEPT,
};

struct Command {
    Action action;
    std::string operand; // only for goto
};

struct Rule {
    char lhs = '.';
    char op = '|'; // goto
    int64_t rhs = 0;
    std::string dest;

    Command process(const Part &part) {
        switch(op) {
            case '>':
                if (part.ratings.at(lhs) > rhs) {
                    if (dest == "R") return {Action::REJECT};
                    if (dest == "A") return {Action::ACCEPT};
                    return {Action::GOTO, dest};
                }
                break;
            case '<':
                if (part.ratings.at(lhs) < rhs) {
                    if (dest == "R") return {Action::REJECT};
                    if (dest == "A") return {Action::ACCEPT};
                    return {Action::GOTO, dest};
                }
                break;
            case '|':
                if (dest == "R") return {Action::REJECT};
                if (dest == "A") return {Action::ACCEPT};
                return {Action::GOTO, dest};
                break;
        };
        return {Action::SKIP};
    }

    friend std::istream& operator >> (std::istream &in, Rule &rule) {
        std::string temp;
        std::getline(in, temp);
        std::stringstream ss(temp);

        if (temp.size() > 1 && temp[1] == '>' || temp[1] == '<') {
            ss >> rule.lhs >> rule.op >> rule.rhs >> rule.dest;
            rule.dest.erase(rule.dest.begin());
        } else {
            ss >> rule.dest;
        }
        return in;
    }
};

struct Workflow {
    std::vector<Rule> rules;

    Command doWork(const Part &part) {
        int32_t ruleNo = 0;
        Command c = rules[ruleNo++].process(part);

        while(c.action == Action::SKIP) {
            assert(ruleNo < rules.size());
            c = rules[ruleNo++].process(part);
        }
        return c;
    }

    bool optimizationOneDestination() {
        auto dest = rules[0].dest;
        bool canSimplify = true;
        for (auto rule : rules) {
            if (rule.dest != dest) {
                canSimplify = false;
            }
        }
        if (canSimplify && rules.size() > 1) {
            Rule newRule;
            newRule.dest = dest;
            rules = {newRule};
            return true;
        }
        return false;
    }

    bool optimize() {
        // if destination is same for every rule, just leave rule
        return optimizationOneDestination();
    }

    friend std::istream& operator >> (std::istream &in, Workflow &work) {
        std::string temp;
        while(!in.eof()) {
            Rule r;
            std::getline(in, temp, ',');
            std::stringstream ss(temp);
            ss >> r;
            work.rules.push_back(r);
        }

        return in;
    }
};

std::unordered_map<std::string, Workflow> workflows;
std::vector<Part> parts;

void optimize() {
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto &w : workflows | std::views::values) {
            changed |= w.optimize();
        }

        // TODO: If workflow has only goto, remove it and change it everywhere only to goto
    }
}

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    while (!input.eof()) {

        std::string inputRow;

        // Parse workflows
        while(true) {
            std::getline(input, inputRow);
            if (inputRow.empty()) break;
            std::stringstream inputStream(inputRow);

            std::string workflowName;
            std::getline(inputStream, workflowName, '{');

            std::string tmp;
            std::getline(inputStream, tmp, '}');

            std::stringstream workStream(tmp);
            workStream >> workflows[workflowName];
        }

        while(!input.eof()) {
            std::getline(input, inputRow);
            if (inputRow.empty()) break;
            std::stringstream inputStream(inputRow);

            std::string tmp;
            std::getline(inputStream, tmp, '{');
            std::getline(inputStream, tmp, '}');

            std::stringstream partsStream(tmp);
            Part part;
            while(!partsStream.eof()) {
                std::string partString;
                std::getline(partsStream, partString, ',');
                if (partString.empty()) continue;
                std::stringstream partStream(partString);
                char symbol;
                char op;
                int64_t rating;
                partStream >> symbol >> op >> rating;
                part.ratings[symbol] = rating;
            }
            parts.push_back(part);
        }

    }

    optimize();

    int64_t sum = 0;
    for (auto &part : parts) {
        auto work = workflows["in"];
        auto command = work.doWork(part);
        // Do work returns goto or accept/reject
        while (command.action == Action::GOTO) {
            work = workflows[command.operand];
            command = work.doWork(part);
        }
        if (command.action == Action::ACCEPT) {
            sum += part.totalRating();
        }
    }

    std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}
