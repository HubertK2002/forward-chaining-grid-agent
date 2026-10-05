# Projekt 1 — SDL

Graficzna wersja symulacji agenta. Logika w `agent_logic.cpp` pozostała oddzielona od interfejsu; SDL obsługuje wybór/generowanie mapy, wizualizację i sterowanie krokami.

## Zależności

- CMake 3.20 lub nowszy
- Ninja
- kompilator C++17
- SDL2 oraz SDL2_ttf
- czcionka DejaVu Sans, Liberation Sans albo Arial

Ubuntu / Debian:

```sh
sudo apt install build-essential cmake ninja-build pkg-config libsdl2-dev libsdl2-ttf-dev fonts-dejavu-core
cmake --preset ninja-debug
cmake --build --preset ninja-debug
./build/project1
```

Windows z vcpkg (triplet x64-windows):

```powershell
vcpkg install sdl2 sdl2-ttf --triplet x64-windows
cmake --preset ninja-debug -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build --preset ninja-debug
```

Wybór mapy: strzałki i Enter; `G` otwiera ustawienia losowej mapy. W losowej mapie Tab wybiera parametr, strzałki go zmieniają, a Enter rozpoczyna symulację. Podczas symulacji `Space` wykonuje krok, Enter uruchamia/zatrzymuje automatykę, `M` wraca do wyboru mapy, a Esc zamyka okno.
