#include "../helpers/includes.hpp"

#define GAMES "games.txt"

struct HandPull {
    static const int32_t MAX_R = 12;
    static const int32_t MAX_G = 13;
    static const int32_t MAX_B = 14;

    int32_t r = 0;
    int32_t g = 0;
    int32_t b = 0;

    int32_t getPower() const {
        return r * g * b;
    }
};

struct Game {
    int32_t id;
    std::vector<HandPull> pulls;

    bool isValid() const {
        if (pulls.empty()) return false;
        for (const auto &pull : pulls) {
            if (pull.r > HandPull::MAX_R) return false;
            if (pull.g > HandPull::MAX_G) return false;
            if (pull.b > HandPull::MAX_B) return false;
        }
        return true;
    }

    HandPull getMax() const {
        HandPull maxPull;
        for (const auto &pull : pulls) {
            if (pull.r > maxPull.r) maxPull.r = pull.r;
            if (pull.g > maxPull.g) maxPull.g = pull.g;
            if (pull.b > maxPull.b) maxPull.b = pull.b;
        }
        return maxPull;
    }

    int32_t getPower() const {
        return getMax().getPower();
    }
};

class Games {
public:
    void add(Game &&game) {
        games.push_back(game);
    }

    int32_t totalPower() const {
        auto powerView = games | std::views::transform([](const auto &g) {
            return g.getPower();
        });

        return std::accumulate(powerView.begin(), powerView.end(), 0);
    }

    int32_t totalValid() const {
        auto validView = games | std::views::transform([](const auto &g) {
            return g.isValid() ? g.id : 0;
        });
        return std::accumulate(validView.begin(), validView.end(), 0);
    }

private:
    std::vector<Game> games;
};

int32_t main() {

    std::ifstream gamesFile;
    gamesFile.open(GAMES);

    if (!gamesFile) return EXIT_FAILURE;

    std::string gameEntry;

    Games games;

    while (!gamesFile.eof()) {
        Game game;
        std::string gameEntry;
        std::string dummy;
        std::getline(gamesFile, gameEntry);
        std::stringstream gameStream(gameEntry);
        std::getline(gameStream, dummy, ' ');
        gameStream >> game.id;
        std::getline(gameStream, dummy, ':');

        while (!gameStream.eof()) {
            std::string onePull;
            std::getline(gameStream, onePull, ';');

            std::stringstream pullStream(onePull);

            HandPull pull;
            while(!pullStream.eof()) {
                int number;
                std::string color;
                pullStream >> number;
                std::getline(pullStream, dummy, ' ');
                std::getline(pullStream, color, ',');

                if (color == "red") {
                    pull.r = number;
                } else if (color == "green") {
                    pull.g = number;
                } else if (color == "blue") {
                    pull.b = number;
                }
            }

            game.pulls.push_back(pull);
        }

        games.add(std::move(game));
    }

    std::cout << "Total valid: " << games.totalValid() << std::endl;
    std::cout << "Total power: " << games.totalPower() << std::endl;

    return EXIT_SUCCESS;
}