#include "../helpers/includes.hpp"

#define INPUT "input.txt"

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

private:
    std::vector<Game> games;
};

int32_t main() {

    std::ifstream gamesFile;
    gamesFile.open(INPUT);

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

    std::cout << games.totalPower() << std::endl;

    return EXIT_SUCCESS;
}