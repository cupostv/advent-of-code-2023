#include "../helpers/includes.hpp"

#define INPUT "input.txt"

bool isAsciiDigit(char digit) {
    return digit >= '0' && digit <= '9';
}

struct ScratchCard {
    int id;
    std::vector<int> winningNumbers;
    std::vector<int> myNumbers;

    int getPoints() const {
        int points = 0;
        for (auto num : myNumbers) {
            if (std::find(winningNumbers.begin(), winningNumbers.end(), num) != winningNumbers.end()) {
                if (points == 0) {
                    points = 1;
                } else {
                    points *= 2;
                }
            }
        }
        return points;
    }

    int winCount() const {
        int count = 0;
        for (auto num : myNumbers) {
            if (std::find(winningNumbers.begin(), winningNumbers.end(), num) != winningNumbers.end()) {
                count++;
            }
        }
        return count;
    }

    void dump() {
        std::cout << "ID: " << id << std::endl;
        for (auto number : winningNumbers) {
            std::cout << number << " ";
        }
    }
};

struct CardDeck {
    std::vector<ScratchCard> cards;

    int getPoints() {
        int points = 0;
        for (const auto &card : cards) {
            points += card.getPoints();
        }
        return points;
    }

    int getTotalCards() {
        std::vector<int> counts(cards.size(), 1);
        for (const auto &card : cards) {
            int count = card.winCount();
            for (int32_t i = 0; i < count; i++) {
                counts[card.id + i] += 1 * counts[card.id - 1];
            }
        }
        return std::accumulate(counts.begin(), counts.end(), 0);
    }
};



std::unordered_map<int, int> cards;
std::vector<int> totalCards;

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