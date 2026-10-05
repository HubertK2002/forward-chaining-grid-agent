cmake_minimum_required(VERSION 3.20)
project(project1_agent LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL2 CONFIG QUIET)
find_package(SDL2_ttf CONFIG QUIET)
if(NOT TARGET SDL2::SDL2 OR NOT TARGET SDL2_ttf::SDL2_ttf)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(SDL2 QUIET IMPORTED_TARGET sdl2)
        pkg_check_modules(SDL2_TTF QUIET IMPORTED_TARGET SDL2_ttf)
    endif()
    if(TARGET PkgConfig::SDL2 AND TARGET PkgConfig::SDL2_TTF)
        set(SDL2_TARGET PkgConfig::SDL2)
        set(SDL2_TTF_TARGET PkgConfig::SDL2_TTF)
    else()
        message(FATAL_ERROR "SDL2 and SDL2_ttf development packages are required. Install libsdl2-dev and libsdl2-ttf-dev, or provide their CMake packages.")
    endif()
else()
    set(SDL2_TARGET SDL2::SDL2)
    set(SDL2_TTF_TARGET SDL2_ttf::SDL2_ttf)
endif()

add_executable(project1
    main.cpp agent_logic.cpp history.cpp map_io.cpp state.cpp sdl_ui.cpp)
target_link_libraries(project1 PRIVATE ${SDL2_TARGET} ${SDL2_TTF_TARGET})
target_compile_definitions(project1 PRIVATE SDL_MAIN_HANDLED)
if(MSVC)
    target_compile_options(project1 PRIVATE /W4)
else()
    target_compile_options(project1 PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_custom_command(TARGET project1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/maps $<TARGET_FILE_DIR:project1>/maps)
