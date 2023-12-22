#include "../helpers/includes.hpp"

#define INPUT "input.txt"

const std::string cardStrength("J23456789TQKA");

struct Card {
    char symbol;

    int32_t operator<=>(const Card& r) const {
        return cardStrength.find(r.symbol) - cardStrength.find(symbol);
    }
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

        auto notJokerCard = [](Card c) { return c.symbol != 'J'; };
        auto cardsWOJoker = cards | std::views::filter(notJokerCard);
        int32_t jokerCount = cards.size() - std::ranges::distance(cardsWOJoker);

        for (auto card : cardsWOJoker) {
            map[card.symbol]++;
        }

        map['X'] = 0;
        auto [maxSymbol, c] = *std::max_element(map.cbegin(), map.cend(), [](auto el1, auto el2) {
            return el1.second < el2.second;
        });

        map[maxSymbol] += jokerCount;

        std::array<int32_t, 6> counts = {0, 0, 0, 0, 0, 0};
        for (auto count : map | std::views::values) {
            counts[count]++;
        }

        if (counts[5] > 0) return FIVE_OF_A_KIND;
        if (counts[4] > 0) return FOUR_OF_A_KIND;
        if (counts[3] == 1 && counts[2] == 1) return FULL_HOUSE;
        if (counts[3] == 1) return THREE_OF_A_KIND;
        if (counts[2] == 2) return TWO_PAIRS;
        if (counts[2] == 1) return ONE_PAIR;
        return 0;
    }

    int32_t operator<=>(const Hand& r) const {
        auto left = getStrength();
        auto right = r.getStrength();

        auto diff = left - right;

        if (diff == 0) {
            for (int32_t i = 0; i < 5; i++) {
                auto res = r.cards[i] <=> cards[i];
                if (res != 0) {
                    return res;
                }
            }
        }
        return diff;
    };

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