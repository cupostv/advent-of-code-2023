#include "../helpers/includes.hpp"

#define INPUT "input.txt"

struct Range {
    int64_t start;
    int64_t length;

    int64_t end() const {
        return start + length - 1;
    }

    static Range create(int64_t start, int64_t end) {
        return Range{start, end - start + 1};
    }

    Range getIntersection(const Range &range) const {
        int64_t a = start;
        int64_t b = end();
        int64_t c = range.start;
        int64_t d = range.end();

        if (b - c >= 0 && d - a >= 0) {
            return Range::create(std::max(a, c), std::min(b, d));
        }

        return {-1, -1};
    }

    std::vector<Range> split(const Range &intersection) const {
        std::vector<Range> ranges;

        if (intersection.start > start)
            ranges.push_back({start, intersection.start - start});

        if (intersection.end() < end())
            ranges.push_back({intersection.start + intersection.length, end() - intersection.end()});

        return ranges;
    }
};

struct Map {
    int64_t source;
    int64_t destination;
    int64_t length;

    Range getSrcRange() {
        return {source, length};
    }

    Range getDstRange() {
        return {destination, length};
    }
};

std::vector<Map> seedToSoil;
std::vector<Map> soilToFertilizer;
std::vector<Map> fertilizerToWater;
std::vector<Map> waterToLight;
std::vector<Map> lightToTemperature;
std::vector<Map> temperatureToHumidity;
std::vector<Map> humidityToLocation;

static inline int64_t mapValueToValue(std::vector<Map> mapValueToValue, int64_t src) {
    int64_t value = -1;
        for (auto map : mapValueToValue) {
            if (src >= map.source && src < map.source + map.length) {
                // Found source
                value = map.destination + (src - map.source);
                break;
            }
        }
        if (value == -1) value = src;
        return value;
}

struct MapResult {
    std::vector<Range> sameValue;
    std::vector<Range> mapValue;
};

std::vector<Range> mapRangeToRange(std::vector<Map> mapRangeToRange, Range srcRange) {
    std::vector<Range> result;

    std::vector<Range> toCheckInNextMap = {srcRange};

    for (auto map : mapRangeToRange) {
        // Find everything that falls in range
        auto mapRange = map.getSrcRange();
        auto mapDstRange = map.getDstRange();
        std::vector<Range> leftover;
        for (auto toCheck : toCheckInNextMap) {
            auto intersection = toCheck.getIntersection(mapRange);
            if (intersection.start != -1) {
                // remove intersection from srcRange
                auto splits = toCheck.split(intersection);
                for (auto split : splits)
                    leftover.push_back(split);
                // Map intersection to destination
                auto offset = intersection.start - mapRange.start;
                result.push_back({mapDstRange.start + offset, intersection.length});
            } // no intersection
            else {
                leftover.push_back(toCheck);
            }
        }

        toCheckInNextMap = leftover;
    }
    for (auto left : toCheckInNextMap)
        result.push_back(left);

    return result;
}

std::vector<Range> seedRanges;

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    while (!input.eof()) {

        std::string inputRow;
        std::getline(input, inputRow);
        if (inputRow.empty()) continue;
        // seeds: 79 14 55 13
        std::string dummy;
        std::stringstream inputStream(inputRow);
        std::getline(inputStream, dummy, ':');
        while (!inputStream.eof()) {
            Range s;
            inputStream >> s.start;
            inputStream >> s.length;
            seedRanges.push_back(s);
        }

        // new line
        std::getline(input, inputRow);
        // seed-to-soil map:
        std::getline(input, inputRow);

        while (inputRow.find("soil-to-fertilizer") == std::string::npos) {
            Map map;
            std::getline(input, inputRow);
            if (inputRow.empty() || inputRow.find("soil-to-fertilizer") != std::string::npos) continue;
            std::stringstream inputStream(inputRow);
            inputStream >> map.destination >> map.source >> map.length;
            seedToSoil.push_back(map);
        }
        while (inputRow.find("fertilizer-to-water") == std::string::npos) {
            Map map;
            std::getline(input, inputRow);
            if (inputRow.empty() || inputRow.find("fertilizer-to-water") != std::string::npos) continue;
            std::stringstream inputStream(inputRow);
            inputStream >> map.destination >> map.source >> map.length;
            soilToFertilizer.push_back(map);
        }
        while (inputRow.find("water-to-light") == std::string::npos) {
            Map map;
            std::getline(input, inputRow);
            if (inputRow.empty() || inputRow.find("water-to-light") != std::string::npos) continue;
            std::stringstream inputStream(inputRow);
            inputStream >> map.destination >> map.source >> map.length;
            fertilizerToWater.push_back(map);
        }
        while (inputRow.find("light-to-temperature") == std::string::npos) {
            Map map;
            std::getline(input, inputRow);
            if (inputRow.empty() || inputRow.find("light-to-temperature") != std::string::npos) continue;
            std::stringstream inputStream(inputRow);
            inputStream >> map.destination >> map.source >> map.length;
            waterToLight.push_back(map);
        }
        while (inputRow.find("temperature-to-humidity") == std::string::npos) {
            Map map;
            std::getline(input, inputRow);
            if (inputRow.empty() || inputRow.find("temperature-to-humidity") != std::string::npos) continue;
            std::stringstream inputStream(inputRow);
            inputStream >> map.destination >> map.source >> map.length;
            lightToTemperature.push_back(map);
        }
        while (inputRow.find("humidity-to-location") == std::string::npos) {
            Map map;
            std::getline(input, inputRow);
            if (inputRow.empty() || inputRow.find("humidity-to-location") != std::string::npos) continue;
            std::stringstream inputStream(inputRow);
            inputStream >> map.destination >> map.source >> map.length;
            temperatureToHumidity.push_back(map);
        }

        while (!input.eof()) {
            Map map;
            std::getline(input, inputRow);
            if (inputRow.empty()) continue;
            std::stringstream inputStream(inputRow);
            inputStream >> map.destination >> map.source >> map.length;
            humidityToLocation.push_back(map);
        }
    }

    // All maps and seeds loaded
    int64_t min = std::numeric_limits<int64_t>::max();
    std::vector<Range> soilMap;
    std::vector<Range> fertilizerMap;
    std::vector<Range> waterMap;
    std::vector<Range> lightMap;
    std::vector<Range> temperatureMap;
    std::vector<Range> humidityMap;
    std::vector<Range> locationMap;

    for (auto range : seedRanges) {
        auto temp = mapRangeToRange(seedToSoil, range);
        soilMap.insert(soilMap.end(), temp.begin(), temp.end());
    }

    for (auto range : soilMap) {
        auto temp = mapRangeToRange(soilToFertilizer, range);
        fertilizerMap.insert(fertilizerMap.end(), temp.begin(), temp.end());
    }

    for (auto range : fertilizerMap) {
        auto temp = mapRangeToRange(fertilizerToWater, range);
        waterMap.insert(waterMap.end(), temp.begin(), temp.end());
    }

    for (auto range : waterMap) {
        auto temp = mapRangeToRange(waterToLight, range);
        lightMap.insert(lightMap.end(), temp.begin(), temp.end());
    }

    for (auto range : lightMap) {
        auto temp = mapRangeToRange(lightToTemperature, range);
        temperatureMap.insert(temperatureMap.end(), temp.begin(), temp.end());
    }

    for (auto range : temperatureMap) {
        auto temp = mapRangeToRange(temperatureToHumidity, range);
        humidityMap.insert(humidityMap.end(), temp.begin(), temp.end());
    }
    for (auto range : humidityMap) {
        auto temp = mapRangeToRange(humidityToLocation, range);
        locationMap.insert(locationMap.end(), temp.begin(), temp.end());
    }

    for (auto loc : locationMap) {
        if (loc.start < min) {
            min = loc.start;
        }
    }

    std::cout << min << std::endl;

    return EXIT_SUCCESS;
}