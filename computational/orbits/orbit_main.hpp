//
// Created by scott on 11/27/25.
//

#ifndef MODERN_TUTOR_ORBIT_MAIN_HPP
#define MODERN_TUTOR_ORBIT_MAIN_HPP
#include <cstddef>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include "Trail.hpp"

#include "Vect2.hpp"
#include "SimulationBody.hpp"



// struct Trail {
//     sf::VertexArray _trail;
//     Vect2 last_point;
//     double min_delta;
//     std::size_t max_points;
//     sf::Color color;
//
//     void addTrailPoint(const Vect2 &position, sf::Color color = sf::Color::White) {};
// };

class OrbitMain {
};

double calculate_distance(SimulationBody &a, SimulationBody &b);
Vect2 calculate_forces_on_body(SimulationBody &a, SimulationBody &b);

#endif //MODERN_TUTOR_ORBIT_MAIN_HPP