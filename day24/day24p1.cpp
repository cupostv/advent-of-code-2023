#include "../helpers/includes.hpp"

// y1 = m1*x1 + c1
// y2 = m2*x2 + c2
// c1 = y1 - m1 * x1
// c2 = y2 - m2 * x2

// First equation
// m1x + c1 = m2x + c2
// m1x - m2x = c2 - c1
// x = (c2 - c1) / (m1 - m2)
// x = (y2 - m2x2 - y1 + m1x1) / (m1 - m2)
// Second equation
// y = m1 * (c2 - c1) / (m1 - m2) + c1
// y = m1 * newX + y1 - m1 * x1
// m == slope

#define INPUT "input.txt"

struct Hailstone {
    int64_t positionX;
    int64_t positionY;
    int64_t positionZ;

    int64_t velocityX;
    int64_t velocityY;
    int64_t velocityZ;

    float getSlope() const {
        return (float)velocityY / (float)velocityX;
    }

    bool isFuture(float x, float y) const {
        if (velocityX < 0 && positionX < x) return false;
        if (velocityX > 0 && positionX > x) return false;
        if (velocityY < 0 && positionY < y) return false;
        if (velocityY > 0 && positionY > y) return false;

        return true;
    }


    bool intersects(const Hailstone &other, int64_t min, int64_t max) const {
        auto slope = getSlope();
        auto otherSlope = other.getSlope();

        // Lines are parallel
        if (slope == otherSlope) return false;

        // Find intersection
        // x = (y2 - m2x2 - y1 + m1x1) / (m1 - m2)
        auto intersectX = (other.positionY - otherSlope * other.positionX - positionY + slope * positionX) / (slope - otherSlope);
        // y = m1 * newX + y1 - m1 * x1
        auto intersectY = slope * intersectX + positionY - slope * positionX;

        if (intersectX < min || intersectX > max) return false;
        if (intersectY < min || intersectY > max) return false;

        return isFuture(intersectX, intersectY) && other.isFuture(intersectX, intersectY);
    }

    friend std::istream& operator >> (std::istream& is, Hailstone& p) {
        is >> p.positionX;
        is.ignore(3, ' ');
        is >> p.positionY;
        is.ignore(3, ' ');
        is >> p.positionZ;
        is.ignore(3, '@');
        is >> p.velocityX;
        is.ignore(3, ' ');
        is >> p.velocityY;
        is.ignore(3, ' ');
        is >> p.velocityZ;
        is.ignore(2);
        return is;
    }
};

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    std::vector<Hailstone> hailstones;

    while (!input.eof()) {

        std::string inputRow;

        std::getline(input, inputRow);
        if (inputRow.empty()) break;

        std::stringstream inputStream(inputRow);

        Hailstone h;
        inputStream >> h;

        hailstones.push_back(h);

    }

    int64_t intersections = 0;
    for (int32_t i = 0; i < hailstones.size(); i++) {
        for (int32_t j = i + 1; j < hailstones.size(); j++) {
            // 7, 27 for test input
            // 200000000000000, 400000000000000 for real input
            if (hailstones[i].intersects(hailstones[j], 200000000000000, 400000000000000))
                intersections++;
        }
    }

    std::cout << intersections << std::endl;

    return EXIT_SUCCESS;
}
