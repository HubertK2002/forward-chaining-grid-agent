#pragma once

#include "map_io.h"

#include <string>

MapSelection ChooseMapSourceSDL(const std::string& directory);
// Returns true when the user asks to return to map selection.
bool RunAgentSDL();
