//
// Created by scott on 12/14/25.
//
#include "SimulationBody.hpp"
#include "Vect2.hpp"

SimulationBody::SimulationBody(std::string name, double mass, Vect2 position, Vect2 velocity, Vect2 force, sf::Color color, float radius) :
    name(name), mass(mass), position(position), velocity(velocity), force(force), color(color), radius(radius) {};

std::ostream& operator<<(std::ostream & os, const SimulationBody &b) {
    os << "mass: " << b.mass << ", position: " << b.position << ", velocity: " << b.velocity << ", force: " << b.force
        << ", KE: " << b.kinetic_energy << ", PE: " << b.potential_energy;
    return os;
}