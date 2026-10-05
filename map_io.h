#pragma once

#include "types.h"

#include <string>
#include <vector>

struct RandomMapParameters {
    int width;
    int height;
    Position agentPosition;
    Position doorPosition;
    int obstacleProbabilityPercent;
};

struct MapSelection {
    bool useRandomMap;
    std::string filename;
    RandomMapParameters randomParameters;
};

std::vector<std::string> GetMapFiles(const std::string& directory);
MapSelection ChooseMapSource(const std::string& directory);
void LoadMapFromFile(const std::string& filename);
void GenerateRandomMap(const RandomMapParameters& parameters);
