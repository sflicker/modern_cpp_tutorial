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
    float drawRadius;
    Trail positions;
    //    Body(double mass, Vect2 &position, Vect2 &velocity, Vect2 &force, sf::Color & color, float radius);

    SimulationBody()
  : name("")
  , mass(0.0)
  , position{0,0}
    , velocity{0,0}
    , force{0,0}
    , kinetic_energy(0.0)
    , potential_energy(0.0)
    , color(sf::Color::White)
    , drawRadius(3.0f)
    , positions() {};

    SimulationBody(std::string name, double mass, Vect2 position, Vect2 velocity, Vect2 force,
    double kinetic_energy, double potential_energy, sf::Color color, float radius);
};

std::ostream& operator<<(std::ostream & os, const SimulationBody &b);

#endif //MODERN_TUTOR_SIMULATIONBODY_HPP