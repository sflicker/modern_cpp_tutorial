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

struct Body {
    double mass;
    Vect2 position;
    Vect2 velocity;
    Vect2 force;
    double kinetic_energy;
    double potential_energy;
    sf::Color color;
    float radius;
    Trail positions;
//    Body(double mass, Vect2 &position, Vect2 &velocity, Vect2 &force, sf::Color & color, float radius);

    Body(double mass, Vect2 vect2, Vect2 vect3, Vect2 vect4, sf::Color color, float radius);;
};

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

double calculate_distance(Body &a, Body &b);
Vect2 calculate_forces_on_body(Body &a, Body &b);

#endif //MODERN_TUTOR_ORBIT_MAIN_HPP