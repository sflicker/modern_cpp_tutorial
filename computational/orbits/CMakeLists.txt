add_executable(orbit_main orbit_main.cpp Trail.cpp Vect2.cpp)
target_link_libraries(orbit_main PRIVATE graphics_display)

find_package(fmt REQUIRED)
target_link_libraries(orbit_main PRIVATE fmt::fmt)