#pragma once

#include <vector>

namespace helper {

// TODO: Change this to class Point
using Position = std::pair<int64_t, int64_t>;

struct GridPolygon {
    std::vector<Position> points;

    int64_t shoelace() const {
        int64_t area = 0;

        // Calculate value of shoelace formula
        int64_t j = points.size() - 1;
        for (int64_t i = 0; i < points.size(); i++)
        {
            area += (points[j].first + points[i].first) * (points[j].second - points[i].second);
            j = i;
        }

        return std::abs(area / 2);
    }

    int64_t perimeter() const {
        int64_t perimeter = 0;
        auto prev = points[0];

        for (int64_t i = 1; i < points.size(); i++) {
            perimeter += std::abs(prev.first - points[i].first) + std::abs(prev.second - points[i].second);
            prev = points[i];
        }
        return perimeter;
    }

    int64_t area() const {
        return shoelace() + perimeter() / 2 + 1;
    }

    int64_t areaInside() const {
        return shoelace() - perimeter() / 2 + 1;
    }
};

}
