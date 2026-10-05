#pragma once

#include "types.h"

#include <sstream>
#include <string>
#include <vector>

extern std::vector<HistoryEntry> History;

void AddObservation(const std::string& observation);
void AddInference(const std::vector<std::string>& premises, const std::string& conclusion);
void AddAction(const std::string& action);
std::stringstream PrintHistoryEntry(const HistoryEntry& entry);
