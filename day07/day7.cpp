#include "../helpers/includes.hpp"

#define INPUT "input.txt"

std::unordered_map<char, int32_t> cardStrength = {
    {'A', 13},
    {'K', 12},
    {'Q', 11},
    {'T', 10},
    {'9', 9},
    {'8', 8},
    {'7', 7},
    {'6', 6},
    {'5', 5},
    {'4', 4},
    {'3', 3},
    {'2', 2},
    {'J', 1},
};

struct Card {
    char symbol;

    int32_t operator<=>(const Card& r) const {
        return cardStrength[r.symbol] - cardStrength[symbol];
    }

    // bool compareStrength(Card card) const {
    //     return cardStrength[card.symbol] - cardStrength[symbol];
    // }
};

enum Poker {
    ONE_PAIR = 1,
    TWO_PAIRS = 2,
    THREE_OF_A_KIND = 3,
    FULL_HOUSE = 4,
    FOUR_OF_A_KIND = 5,
    FIVE_OF_A_KIND = 6,
};

struct Hand {
    std::array<Card, 5> cards;
    int32_t bet;

    int32_t getStrength() const {
        std::unordered_map<char, int32_t> map;
        int32_t jokerCount = 0;
        for (int32_t i = 0; i < 5; i++) {
            if (cards[i].symbol == 'J') jokerCount++;
            else map[cards[i].symbol]++;
        }

        if (jokerCount == 5 || jokerCount == 4) {
            return FIVE_OF_A_KIND;
        }

        // bool hasThree = false;
        // bool hasTwo = false;
        int32_t threeCount = 0;
        int32_t twoCount = 0;
        // we have every card without jokers
        // int32_t jokerLeftover = jokerCount;
        for (auto [symbol, count] : map) {
            if (count == 5 || count + jokerCount == 5) {
                return FIVE_OF_A_KIND;
            }
            if (count == 4 || count + jokerCount == 4) {
                return FOUR_OF_A_KIND;
            }
            if (count == 3) {
                threeCount++;
            }
            if (count == 2) {
                twoCount++;
            }
        }
        // Full house
        if (twoCount == 2 && jokerCount == 1 ||
            twoCount == 1 && threeCount == 1 ||
            threeCount == 1 && jokerCount == 2) {
            return FULL_HOUSE;
        }

        // Three of a kind
        if (threeCount == 1 || twoCount == 1 && jokerCount == 1 || jokerCount == 2) {
            return THREE_OF_A_KIND;
        }

        //two pairs
        if (twoCount == 2 ||
            twoCount == 1 && jokerCount == 2 ||
            twoCount == 1 && jokerCount == 1) {
            return TWO_PAIRS;
        }

        if (twoCount == 1 || jokerCount == 1) {
            return ONE_PAIR;
        }

        return 0;
    }

    int32_t operator<=>(const Hand& r) const {
        auto left = getStrength();
        auto right = r.getStrength();

        auto diff = left - right;

        if (diff == 0) {
            for (int32_t i = 0; i < 5; i++) {
                // auto res = cards[i].compareStrength(r.cards[i]);
                // std::cout << res;
                // if return res > 0 ? -1 : 1;
                auto res = r.cards[i] <=> cards[i];
                if (res != 0) {
                    return res;
                }
            }
        }
        return diff;
    };


//     Five of a kind, where all five cards have the same label: AAAAA
// Four of a kind, where four cards have the same label and one card has a different label: AA8AA
// Full house, where three cards have the same label, and the remaining two cards share a different label: 23332
// Three of a kind, where three cards have the same label, and the remaining two cards are each different from any other card in the hand: TTT98
// Two pair, where two cards share one label, two other cards share a second label, and the remaining card has a third label: 23432
// One pair, where two cards share one label, and the other three cards have a different label from the pair and each other: A23A4
// High card, where all cards
};

struct Game {
    std::vector<Hand> hands;

    void sort() {
        std::sort(hands.begin(), hands.end(), std::greater<Hand>());
    }

    int64_t calculateTotal() {
        sort();
        int64_t sum = 0;
        for (int32_t i = 0; i < hands.size(); i++) {
            sum += hands[i].bet * (hands.size() - i);
        }
        return sum;
    }

    void dump() {
        for (auto hand : hands) {
            std::cout << "Hand: ";
            for (auto card : hand.cards) {
                std::cout << card.symbol << " ";
            }
            std::cout << "Bet: " << hand.bet << std::endl;
        }
    }
};

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    Game game;

    while (!input.eof()) {

        std::string inputRow;
        std::getline(input, inputRow);
        if (inputRow.empty()) continue;

        std::stringstream inputStream(inputRow);

        Hand hand;
        for (int32_t i = 0; i < 5; i++) {
            Card card;
            inputStream >> card.symbol;
            hand.cards[i] = card;
        }

        inputStream >> hand.bet;

        game.hands.push_back(hand);
    }

    // Hand h{{'A', 'Q', 'Q', 'K', 'J'}, 10};
    // std::cout << h.getStrength();

    std::cout << game.calculateTotal() << std::endl;

    return EXIT_SUCCESS;
}