#include "agent_logic.h"
#include "console_ui.h"
#include "history.h"
#include "map_io.h"
#include "state.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

int main() {
    MapSelection mapSelection = ChooseMapSource("maps");
    if (mapSelection.useRandomMap) {
        GenerateRandomMap(mapSelection.randomParameters);
        std::cout << "Generated random map: "
                  << mapSelection.randomParameters.width << "x" << mapSelection.randomParameters.height
                  << ", obstacle probability " << mapSelection.randomParameters.obstacleProbabilityPercent
                  << "%\n";
    } else {
        std::cout << mapSelection.filename << '\n';
        LoadMapFromFile(mapSelection.filename);
    }

    MakeObservations();

    while (!IsAgentAtDoor()) {
        DrawMap();
        MoveAgent();
        MakeObservations();
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
    }

    std::fstream logFile("agent_log.txt");
    if (logFile.is_open()) {
        for (const auto& entry : History) {
            logFile << PrintHistoryEntry(entry).str();
        }
        logFile.close();
    } else {
        std::cerr << "Unable to open log file for writing.\n";
    }

    DrawMap();
    return 0;
}
