project (GraphicsDemo VERSION 1.0 LANGUAGES CXX)

add_library(graphics_display STATIC src/graphics_display.cpp)

# Include the directory that contains graphics_display.hpp
target_include_directories(graphics_display PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Link SFML libraries
target_link_libraries(graphics_display PUBLIC sfml-graphics sfml-window sfml-system)
