#include "agent_logic.h"
#include "history.h"
#include "map_io.h"
#include "state.h"
#include "sdl_ui.h"

#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
    try {
    std::string mapDirectory = "maps";
    if (!std::filesystem::exists(mapDirectory) && argc > 0)
        mapDirectory = (std::filesystem::path(argv[0]).parent_path() / "maps").string();
    MapSelection mapSelection = ChooseMapSourceSDL(mapDirectory);
    if (mapSelection.useRandomMap) {
        GenerateRandomMap(mapSelection.randomParameters);
    } else {
        LoadMapFromFile(mapSelection.filename);
    }

    MakeObservations();
    RunAgentSDL();

    std::fstream logFile("agent_log.txt");
    if (logFile.is_open()) {
        for (const auto& entry : History) {
            logFile << PrintHistoryEntry(entry).str();
        }
        logFile.close();
    } else {
        std::cerr << "Unable to open log file for writing.\n";
    }
    return 0;
    } catch (const std::exception& e) {
        std::cerr << "Application error: " << e.what() << '\n';
        return 1;
    }
}
