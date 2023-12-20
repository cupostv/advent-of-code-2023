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

    void simplify() {
        // if destination is same for every rule, just leave rule
        auto dest = rules[0].dest;
        bool canSimplify = true;
        for (auto rule : rules) {
            if (rule.dest != dest) {
                canSimplify = false;
            }
        }
        if (canSimplify) {
            Rule newRule;
            newRule.dest = dest;
            rules = {newRule};
        }
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

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    while (!input.eof()) {

        std::string inputRow;
        // std::getline(input, inputRow);

        // if (inputRow.empty()) continue;

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
