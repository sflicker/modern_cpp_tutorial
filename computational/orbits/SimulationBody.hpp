//
// Created by scott on 12/14/25.
//

#ifndef MODERN_TUTOR_SIMULATIONBODY_HPP
#define MODERN_TUTOR_SIMULATIONBODY_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/String.hpp>
#include "Vect2.hpp"
#include "Trail.hpp"

struct SimulationBody {
    std::string name;
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

    SimulationBody(std::string name, double mass, Vect2 vect2, Vect2 vect3, Vect2 vect4, sf::Color color, float radius);;
};

std::ostream& operator<<(std::ostream & os, const SimulationBody &b);

#endif //MODERN_TUTOR_SIMULATIONBODY_HPP