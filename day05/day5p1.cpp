#include "../helpers/includes.hpp"

#define INPUT "input.txt"

struct Map {
    int64_t source;
    int64_t destination;
    int64_t length;
};

std::vector<Map> seedToSoil;
std::vector<Map> soilToFertilizer;
std::vector<Map> fertilizerToWater;
std::vector<Map> waterToLight;
std::vector<Map> lightToTemperature;
std::vector<Map> temperatureToHumidity;
std::vector<Map> humidityToLocation;

struct Seed {
    int64_t id;

    int64_t getSoil() {
        int64_t soil = -1;
        for (auto map : seedToSoil) {
            if (id >= map.source && id < map.source + map.length) {
                // Found source
                soil = map.destination + (id - map.source);
                break;
            }
        }
        if (soil == -1) soil = id;
        return soil;
    }

    int64_t getFertilizer() {
        int64_t soil = getSoil();
        int64_t fertilizer = -1;
        for (auto map : soilToFertilizer) {
            if (soil >= map.source && soil < map.source + map.length) {
                // Found source
                fertilizer = map.destination + (soil - map.source);
                break;
            }
        }
        if (fertilizer == -1) fertilizer = soil;
        return fertilizer;
    }

    int64_t getWater() {
        int64_t fertilizer = getFertilizer();
        int64_t water = -1;
        for (auto map : fertilizerToWater) {
            if (fertilizer >= map.source && fertilizer < map.source + map.length) {
                // Found source
                water = map.destination + (fertilizer - map.source);
                break;
            }
        }
        if (water == -1) water = fertilizer;
        return water;
    }

    int64_t getLight() {
        int64_t water = getWater();
        int64_t light = -1;
        for (auto map : waterToLight) {
            if (water >= map.source && water < map.source + map.length) {
                // Found source
                light = map.destination + (water - map.source);
                break;
            }
        }
        if (light == -1) light = water;
        return light;
    }

    int64_t getTemperature() {
        int64_t light = getLight();
        int64_t temperature = -1;
        for (auto map : lightToTemperature) {
            if (light >= map.source && light < map.source + map.length) {
                // Found source
                temperature = map.destination + (light - map.source);
                break;
            }
        }
        if (temperature == -1) temperature = light;
        return temperature;
    }

    int64_t getHumidity() {
        int64_t temperature = getTemperature();
        int64_t humidity = -1;
        for (auto map : temperatureToHumidity) {
            if (temperature >= map.source && temperature < map.source + map.length) {
                // Found source
                humidity = map.destination + (temperature - map.source);
                break;
            }
        }
        if (humidity == -1) humidity = temperature;
        return humidity;
    }

    int64_t getLocation() {
        int64_t humidity = getHumidity();
        int64_t location = -1;
        for (auto map : humidityToLocation) {
            if (humidity >= map.source && humidity < map.source + map.length) {
                // Found source
                location = map.destination + (humidity - map.source);
                break;
            }
        }

        if (location == -1) location = humidity;
        return location;
    }
};

std::vector<Seed> seeds;

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    int sum = 0;

    while (!input.eof()) {

        std::string inputRow;
        std::getline(input, inputRow);
        if (inputRow.empty()) continue;
        // seeds: 79 14 55 13
        std::string dummy;
        std::stringstream inputStream(inputRow);
        std::getline(inputStream, dummy, ':');
        while (!inputStream.eof()) {
            Seed s;
            inputStream >> s.id;
            seeds.push_back(s);
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
    int64_t min = seeds[0].getLocation();
    for (auto seed : seeds) {
        if (seed.getLocation() < min) {
            min = seed.getLocation();
        }
    }

    std::cout << min << std::endl;


    return EXIT_SUCCESS;
}