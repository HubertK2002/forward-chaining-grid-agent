#include "console_ui.h"

#include "history.h"
#include "state.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <windows.h>

void ClearScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count = 0;
    DWORD cellCount = 0;
    COORD homeCoords = {0, 0};

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return;
    }

    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    FillConsoleOutputCharacter(hConsole, ' ', cellCount, homeCoords, &count);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count);
    SetConsoleCursorPosition(hConsole, homeCoords);
}

void DrawMap() {
    ClearScreen();

    for (int y = 0; y < Height; y++) {
        std::cout << std::setw(2) << " |";
        for (int x = 0; x < Width; x++) {
            if (IsAgentAtPosition(x, y)) {
                std::cout << std::setw(2) << "A" << " |";
            } else if (Obstacles[y][x]) {
                if (ObstaclesObserved[y][x]) {
                    std::cout << std::setw(2) << "O" << " |";
                } else {
                    std::cout << std::setw(2) << "X" << " |";
                }
            } else if (DoorPosition.x == x && DoorPosition.y == y) {
                std::cout << std::setw(2) << "D" << " |";
            } else {
                std::cout << std::setw(2) << " " << " |";
            }
        }
        std::cout << '\n';
    }

    if (!History.empty()) {
        int start = std::max(0, static_cast<int>(History.size()) - 14);
        for (int i = start; i < static_cast<int>(History.size()); i++) {
            std::cout << PrintHistoryEntry(History[i]).str();
        }
    }
}
