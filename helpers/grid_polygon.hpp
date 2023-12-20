#pragma once

#include <vector>
#include "point_helper.hpp"

namespace helper {

struct GridPolygon {
    std::vector<helper::Point> points;

    int64_t shoelace() const {
        int64_t area = 0;

        // Calculate value of shoelace formula
        int64_t j = points.size() - 1;
        for (int64_t i = 0; i < points.size(); i++)
        {
            area += (points[j].x + points[i].x) * (points[j].y - points[i].y);
            j = i;
        }

        return std::abs(area / 2);
    }

    int64_t perimeter() const {
        int64_t perimeter = 0;
        auto prev = points[0];

        for (int64_t i = 1; i < points.size(); i++) {
            perimeter += prev.manhattanDistance(points[i]);
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
