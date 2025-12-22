cmake_minimum_required(VERSION 3.31)
project(modern_tutor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_subdirectory(tutorials)
add_subdirectory(graphics_display)
add_subdirectory(computational)
add_subdirectory(games)

