#include "../helpers/includes.hpp"

#define GAMES "games.txt"

struct HandPull {
    static const int32_t MAX_R = 12;
    static const int32_t MAX_G = 13;
    static const int32_t MAX_B = 14;

    int r = 0;
    int g = 0;
    int b = 0;

    void dump() const {
        std::cout << "r=" << r << " ";
        std::cout << "g=" << g << " ";
        std::cout << "b=" << b << " ";
    }

    int getPower() const {
        return r * g * b;
    }
};

struct Game {
    int id;
    std::vector<HandPull> pulls;

    void dump() const {
        std::cout << "#" << id << ": ";
        for (const auto &pull : pulls) {
            pull.dump();
            std::cout << ": ";
        }
        std::cout << std::endl;
    }

    bool isValid() const {
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

    int getPower() const {
        return getMax().getPower();
    }
};

class Games {
public:
    void add(Game &&game) {
        games.push_back(game);
    }

    int totalPower() const {
        int power = 0;
        for (const auto &game : games) {
            power += game.getPower();
        }
        return power;
    }

    int totalValid() const {
        int valid = 0;
        for (const auto &game : games) {
            valid += game.isValid() ? game.id : 0;
        }
        return valid;
    }

    void dump() const {
        for (const auto &game : games) {
            game.dump();
        };
        std::cout << "Sum valid: " << totalValid() << std::endl;
        std::cout << "Power: " << totalPower() << std::endl;
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

    games.dump();

    return EXIT_SUCCESS;
}