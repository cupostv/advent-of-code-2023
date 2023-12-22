#include "../helpers/includes.hpp"

#define INPUT "input.txt"

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

using Direction = std::pair<int64_t, int64_t>;
using Position = std::pair<int64_t, int64_t>;

static const Direction RIGHT = {0, 1};
static const Direction LEFT = {0, -1};
static const Direction UP = {-1, 0};
static const Direction DOWN = {1, 0};

struct Part {
    std::unordered_map<char, int64_t> ratings;

    int64_t totalRating() {
        int64_t total = 0;
        for (auto rating : ratings | std::views::values)
            total += rating;

        return total;
    }
};

using Range = std::pair<int64_t, int64_t>;

struct RangedPart {
    // range [1, 4000]
    std::unordered_map<char, Range> ratings;

    int64_t size(char c) const {
        return ratings.at(c).second - ratings.at(c).first + 1;
    }

    int64_t combinations() const {
        return size('x') * size('m') * size('a') * size('s');
    }

    std::vector<RangedPart> split(char c, int64_t left) const {
        assert(left > 0 && left < size(c));

        auto [x, y] = ratings.at(c);

        RangedPart leftRange = *this;
        RangedPart rightRange = *this;

        leftRange.ratings[c].second = x + left - 1;
        rightRange.ratings[c].first = x + left;

        return {leftRange, rightRange};
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

    std::vector<std::pair<RangedPart, Command>> process(const RangedPart &part) {
        Command c;
        if (dest == "R") c = {Action::REJECT};
        else if (dest == "A") c = {Action::ACCEPT};
        else c = {Action::GOTO, dest};

        switch(op) {
            case '>':
            {
                auto range = part.ratings.at(lhs);
                auto left = range.first - rhs - 1;
                auto right = range.second - rhs;
                if (left <= 0 && right <= 0) {
                    // whole range skip
                    return {{part, {Action::SKIP}}};
                } else if (left < 0 && right > 0) {
                    auto v = part.split(lhs, std::abs(left));
                    // first abs(first) elements skip second goto
                    return {{v[0], {Action::SKIP}}, {v[1], c}};
                } else if (left >= 0 && right >= 0) {
                    // all goto
                    return {{part, c}};
                }

                break;
            }
            case '<':
            {
                auto range = part.ratings.at(lhs);
                auto left = range.first - rhs;
                auto right = range.second - rhs + 1;

                if (left <= 0 && right <= 0) {
                    // all goto
                    return {{part, c}};
                } else if (left < 0 && right > 0) {
                    auto v = part.split(lhs, std::abs(left));
                    // first abs(first) elements goto second skip
                    return {{v[0], c}, {v[1], {Action::SKIP}}};
                } else if (left >= 0 && right >= 0) {
                    // all skip
                    return {{part, {Action::SKIP}}};
                }

                break;
            }
            case '|':
                return {{part, c}};
                break;
        };
        return {{part, {Action::SKIP}}};
    }
};

struct Workflow {
    std::vector<Rule> rules;

    auto doWork(const RangedPart &part) {
        int32_t ruleNo = 0;
        std::vector<std::pair<RangedPart, Command>> ret;
        std::vector<std::pair<RangedPart, Command>> toProcess = rules[ruleNo++].process(part);

        while(!toProcess.empty()) {
            if (toProcess[0].second.action != Action::SKIP) {
                ret.push_back(toProcess[0]);
                toProcess.erase(toProcess.begin());
            }
            if (toProcess.size() == 2) {
                if (toProcess[1].second.action != Action::SKIP) {
                    ret.push_back(toProcess[1]);
                    toProcess.erase(toProcess.begin() + 1);
                }
            }
            if (!toProcess.empty()) {
                toProcess = rules[ruleNo++].process(toProcess[0].first);
            }
        }

        return ret;
    }

    bool optimizationOneDestination() {
        auto dest = rules[0].dest;
        bool canSimplify = true;
        for (auto rule : rules) {
            if (rule.dest != dest)
                canSimplify = false;
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

            Workflow workflow;
            while(!workStream.eof()) {
                Rule rule;
                std::string ruleString;
                std::getline(workStream, ruleString, ',');
                if (ruleString.empty()) continue;
                std::stringstream ruleStream(ruleString);
                if (ruleString.size() > 1 && ruleString[1] == '>' || ruleString[1] == '<') {
                    ruleStream >> rule.lhs >> rule.op >> rule.rhs >> rule.dest;
                    rule.dest.erase(rule.dest.begin());
                } else {
                    ruleStream >> rule.dest;
                }
                workflow.rules.push_back(rule);
            }

            workflows[workflowName] = workflow;
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

    RangedPart part;
    // Happy ...
    part.ratings['x'] = {1, 4000};
    part.ratings['m'] = {1, 4000};
    part.ratings['a'] = {1, 4000};
    part.ratings['s'] = {1, 4000};

    auto work = workflows["in"];
    auto result = work.doWork(part);

    int64_t combinations = 0;
    while(!result.empty()) {
        auto res = result.back();
        result.pop_back();

        if (res.second.action == Action::GOTO) {
            auto w = workflows[res.second.operand].doWork(res.first);
            result.insert(result.end(), w.begin(), w.end());
        }
        if (res.second.action == Action::ACCEPT) {
            combinations += res.first.combinations();
        }

    }

    std::cout << combinations << std::endl;

    return EXIT_SUCCESS;
}
