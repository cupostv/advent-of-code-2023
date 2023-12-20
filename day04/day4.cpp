#include "../helpers/includes.hpp"

#define INPUT "input.txt"

struct ScratchCard {
    int32_t id;
    std::vector<int32_t> winningNumbers;
    std::vector<int32_t> myNumbers;

    int32_t getPoints() const {
        int32_t points = 0;
        for (auto num : myNumbers) {
            if (helper::find(winningNumbers, num)) {
                if (points == 0) {
                    points = 1;
                } else {
                    points *= 2;
                }
            }
        }
        return points;
    }

    int32_t winCount() const {
        int32_t count = 0;
        for (auto num : myNumbers) {
            if (helper::find(winningNumbers, num)) {
                count++;
            }
        }
        return count;
    }
};

struct CardDeck {
    std::vector<ScratchCard> cards;

    int32_t getPoints() {
        int32_t points = 0;
        for (const auto &card : cards) {
            points += card.getPoints();
        }
        return points;
    }

    int32_t getTotalCards() {
        std::vector<int32_t> counts(cards.size(), 1);
        for (const auto &card : cards) {
            int32_t count = card.winCount();
            for (int32_t i = 0; i < count; i++) {
                counts[card.id + i] += 1 * counts[card.id - 1];
            }
        }
        return std::accumulate(counts.begin(), counts.end(), 0);
    }
};



std::unordered_map<int32_t, int32_t> cards;
std::vector<int32_t> totalCards;

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    CardDeck deck;

    if (!input) return EXIT_FAILURE;

    int sum = 0;

    while (!input.eof()) {
        ScratchCard card;
        // std::vector<int> myNumbers;

        std::string inputRow;
        std::getline(input, inputRow);
        if (inputRow.empty()) continue;
        std::stringstream inputStream(inputRow);
        std::string dummy;
        std::getline(inputStream, dummy, ' '); // Game
        inputStream >> card.id;
        std::getline(inputStream, dummy, ':');
        std::string winningString;
        std::getline(inputStream, winningString, '|');
        std::stringstream winningStream(winningString);
        while(!winningStream.eof()) {
            int winningNumber;
            winningStream >> winningNumber;
            card.winningNumbers.push_back(winningNumber);
        }
        std::string mynumbersString;
        std::getline(inputStream, mynumbersString);
        std::stringstream myNumbersStream(mynumbersString);
        while(!myNumbersStream.eof()) {
            int myNumber;
            myNumbersStream >> myNumber;
            card.myNumbers.push_back(myNumber);
        }

        deck.cards.push_back(card);
    }

    std::cout << deck.getTotalCards() << std::endl;

    int count = 0;
    for (auto [id, cnt] : cards) {
        count += cnt;
    }

    std::cout << count << std::endl;

    return EXIT_SUCCESS;
}