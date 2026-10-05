#include "map_io.h"

#include "state.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>

namespace {
int ReadIntInRange(const std::string& prompt, int minValue, int maxValue) {
    int value = 0;

    while (true) {
        std::cout << prompt;
        std::cin >> value;

        if (!std::cin.fail() && value >= minValue && value <= maxValue) {
            return value;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid value. Expected range " << minValue << "-" << maxValue << ".\n";
    }
}

bool ReadYesNo(const std::string& prompt) {
    char answer = '\0';

    while (true) {
        std::cout << prompt;
        std::cin >> answer;

        if (answer == 'y' || answer == 'Y') {
            return true;
        }

        if (answer == 'n' || answer == 'N') {
            return false;
        }

        std::cout << "Please answer y/n.\n";
    }
}

std::mt19937& GetRandomEngine() {
    static std::mt19937 engine(std::random_device{}());
    return engine;
}

int DrawInt(int minValue, int maxValue) {
    std::uniform_int_distribution<int> distribution(minValue, maxValue);
    return distribution(GetRandomEngine());
}

int DrawObstacleProbability() {
    static const int probabilities[] = {10, 50, 70, 90};
    std::uniform_int_distribution<int> distribution(0, 3);
    return probabilities[distribution(GetRandomEngine())];
}

RandomMapParameters ChooseRandomMapParameters() {
    RandomMapParameters parameters{};
    bool randomizeParameters = ReadYesNo("Randomize map parameters? (y/n): ");

    if (randomizeParameters) {
        parameters.height = DrawInt(3, 7);
        parameters.width = DrawInt(7, 20);
        parameters.agentPosition = {0, DrawInt(0, parameters.height - 1)};
        parameters.doorPosition = {parameters.width - 1, DrawInt(0, parameters.height - 1)};
        parameters.obstacleProbabilityPercent = DrawObstacleProbability();
        return parameters;
    }

    parameters.height = ReadIntInRange("Map height (3-7): ", 3, 7);
    parameters.width = ReadIntInRange("Map width (7-20): ", 7, 20);
    parameters.agentPosition = {
        0,
        ReadIntInRange("Agent start row y (0-" + std::to_string(parameters.height - 1) + "): ", 0, parameters.height - 1)
    };
    parameters.doorPosition = {
        parameters.width - 1,
        ReadIntInRange("Door row y (0-" + std::to_string(parameters.height - 1) + "): ", 0, parameters.height - 1)
    };
    parameters.obstacleProbabilityPercent = ReadIntInRange("Obstacle probability in percent (10-90): ", 10, 90);
    return parameters;
}
}

std::vector<std::string> GetMapFiles(const std::string& directory) {
    std::vector<std::string> files;

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path().string());
        }
    }

    std::sort(files.begin(), files.end());
    return files;
}

MapSelection ChooseMapSource(const std::string& directory) {
    std::vector<std::string> files = GetMapFiles(directory);
    int randomMapMenuNumber = static_cast<int>(files.size()) < 9 ? 9 : static_cast<int>(files.size()) + 1;

    std::cout << "Available maps:\n";
    for (int i = 0; i < static_cast<int>(files.size()); i++) {
        std::cout << i + 1 << ". " << std::filesystem::path(files[i]).filename().string() << '\n';
    }
    std::cout << randomMapMenuNumber << ". Random map\n";

    int choice = 0;
    while (true) {
        std::cout << "Choose map number: ";
        std::cin >> choice;

        if (!std::cin.fail() &&
            ((choice >= 1 && choice <= static_cast<int>(files.size())) || choice == randomMapMenuNumber)) {
            break;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid choice.\n";
    }

    if (choice == randomMapMenuNumber) {
        return {true, "", ChooseRandomMapParameters()};
    }

    if (files.empty()) {
        throw std::runtime_error("No map files found in directory: " + directory);
    }

    return {false, files[choice - 1], {}};
}

void LoadMapFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open map file: " + filename);
    }

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        lines.push_back(line);
    }

    if (lines.empty()) {
        throw std::runtime_error("Map file is empty");
    }

    int detectedHeight = static_cast<int>(lines.size());
    int detectedWidth = static_cast<int>(lines[0].size());

    if (detectedWidth == 0) {
        throw std::runtime_error("Map width cannot be zero");
    }

    for (int y = 0; y < detectedHeight; y++) {
        if (static_cast<int>(lines[y].size()) != detectedWidth) {
            throw std::runtime_error(
                "Inconsistent row width at row " + std::to_string(y) +
                ": expected " + std::to_string(detectedWidth) +
                ", got " + std::to_string(lines[y].size())
            );
        }
    }

    ResizeMapState(detectedHeight, detectedWidth);

    bool agentFound = false;
    bool doorFound = false;

    for (int y = 0; y < Height; y++) {
        for (int x = 0; x < Width; x++) {
            char c = lines[y][x];

            if (c == 'X') {
                Obstacles[y][x] = true;
            } else if (c == 'A') {
                if (agentFound) {
                    throw std::runtime_error("More than one agent start position");
                }
                AgentPosition = {x, y};
                agentFound = true;
            } else if (c == 'D') {
                if (doorFound) {
                    throw std::runtime_error("More than one door position");
                }
                DoorPosition = {x, y};
                doorFound = true;
            } else if (c != '.') {
                throw std::runtime_error(
                    "Invalid character in map file at x=" + std::to_string(x) +
                    ", y=" + std::to_string(y)
                );
            }
        }
    }

    if (!agentFound) {
        throw std::runtime_error("Agent start position not found");
    }

    if (!doorFound) {
        throw std::runtime_error("Door position not found");
    }
}

void GenerateRandomMap(const RandomMapParameters& parameters) {
    if (parameters.height < 3 || parameters.height > 7) {
        throw std::runtime_error("Random map height must be in range 3-7");
    }

    if (parameters.width < 7 || parameters.width > 20) {
        throw std::runtime_error("Random map width must be in range 7-20");
    }

    if (parameters.agentPosition.x != 0) {
        throw std::runtime_error("Agent start position must be on the left border (x = 0)");
    }

    if (parameters.doorPosition.x != parameters.width - 1) {
        throw std::runtime_error("Door position must be on the right border (x = width - 1)");
    }

    if (parameters.agentPosition.y < 0 || parameters.agentPosition.y >= parameters.height ||
        parameters.doorPosition.y < 0 || parameters.doorPosition.y >= parameters.height) {
        throw std::runtime_error("Agent or door y position out of range");
    }

    if (parameters.obstacleProbabilityPercent < 0 || parameters.obstacleProbabilityPercent > 100) {
        throw std::runtime_error("Obstacle probability must be in range 0-100");
    }

    ResizeMapState(parameters.height, parameters.width);
    AgentPosition = parameters.agentPosition;
    DoorPosition = parameters.doorPosition;

    std::bernoulli_distribution obstacleDistribution(
        static_cast<double>(parameters.obstacleProbabilityPercent) / 100.0
    );

    for (int y = 0; y < Height; y++) {
        for (int x = 0; x < Width; x++) {
            bool reservedForAgent = (x == AgentPosition.x && y == AgentPosition.y);
            bool reservedForDoor = (x == DoorPosition.x && y == DoorPosition.y);

            if (!reservedForAgent && !reservedForDoor) {
                Obstacles[y][x] = obstacleDistribution(GetRandomEngine());
            }
        }
    }
}
