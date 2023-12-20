#pragma once

namespace helper {

struct Point {

    int64_t x;
    int64_t y;

    auto operator<=>(const Point&) const = default;

    Point() = default;
    Point(int64_t x, int64_t y) : x(x), y(y) {}
    Point(int64_t val) : x(val), y(val) {}

    int64_t manhattanDistance(const Point &other) const {
        // Manhattan distance
        return std::abs(x - other.x) + std::abs(y - other.y);
    }

    friend std::ostream& operator << (std::ostream& os, const Point& p) {
        os << "(" << p.x << "," << p.y << ")";
        return os;
    }
    friend Point operator+(const Point& p1, const Point& p2) {
        return Point(p1.x + p2.x, p1.y + p2.y);
    }
    friend Point operator-(const Point& p1, const Point& p2) {
        return Point(p1.x - p2.x, p1.y - p2.y);
    }
    friend Point operator*(const Point& p1, const Point& p2) {
        return Point(p1.x * p2.x, p1.y * p2.y);
    }
    Point& operator+=(const Point& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    Point& operator-=(const Point& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    Point& operator*=(const Point& rhs) {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }

    static const Point RIGHT;
    static const Point LEFT;
    static const Point UP;
    static const Point DOWN;
};

const Point Point::RIGHT = Point(0, 1);
const Point Point::LEFT = Point(0, -1);
const Point Point::UP = Point(-1, 0);
const Point Point::DOWN = Point(1, 0);

}