#pragma once

#include <string>
#include <vector>

struct Position {
    int x;
    int y;
};

struct HistoryEntry {
    std::string kind;
    std::vector<std::string> premises;
    std::string conclusion;
};
