#include "history.h"

#include "agent_logic.h"

std::vector<HistoryEntry> History;

namespace {
void AddHistory(const std::string& kind, const std::vector<std::string>& premises, const std::string& conclusion) {
    History.push_back({kind, premises, conclusion});
}
}

void AddObservation(const std::string& observation) {
    AddHistory("OBS", {}, observation);
    InferFromLatestObservation(observation);
}

void AddInference(const std::vector<std::string>& premises, const std::string& conclusion) {
    AddHistory("INF", premises, conclusion);
}

void AddAction(const std::string& action) {
    AddHistory("ACT", {}, action);
}

std::stringstream PrintHistoryEntry(const HistoryEntry& entry) {
    std::stringstream ss;
    ss << "[" << entry.kind << "] ";

    if (!entry.premises.empty()) {
        for (int i = 0; i < static_cast<int>(entry.premises.size()); i++) {
            if (i > 0) {
                ss << " /\\ ";
            }
            ss << entry.premises[i];
        }
        ss << " => ";
    }

    ss << entry.conclusion << '\n';
    return ss;
}
