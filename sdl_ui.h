#pragma once

#include "map_io.h"

#include <string>

MapSelection ChooseMapSourceSDL(const std::string& directory);
void RunAgentSDL();
