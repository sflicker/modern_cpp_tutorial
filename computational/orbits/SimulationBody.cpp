//
// Created by scott on 12/14/25.
//
#include "SimulationBody.hpp"
#include "Vect2.hpp"

SimulationBody::SimulationBody(BodyID id, std::string name, double mass, Vect2 position, Vect2 velocity, Vect2 force,
    double kinetic_energy, double potential_energy, sf::Color color, float radius, bool showLabel, bool isStar) :
    id(id), name(name), mass(mass), position(position), velocity(velocity), force(force),
    kinetic_energy(kinetic_energy), potential_energy(potential_energy),
    color(color), drawRadius(radius), showLabel(showLabel), isStar(isStar) {};

std::ostream& operator<<(std::ostream & os, const SimulationBody &b) {
    os << "mass: " << b.mass << ", position: " << b.position << ", velocity: " << b.velocity << ", force: " << b.force
        << ", KE: " << b.kinetic_energy << ", PE: " << b.potential_energy;
    return os;
}

BodyID nextBodyId = 1;

BodyID allocateId() {
    return nextBodyId++;
}

SimulationBody * get_body_by_id(std::vector<SimulationBody> & bodies, BodyID id) {
    for (auto &body : bodies) {
        if (body.id == id) {
            return &body;
        }
    }
    return nullptr;
}

SimulationBody * get_body_by_name(std::vector<SimulationBody> & bodies, std::string name) {
    for (auto &body : bodies) {
        if (body.name == name) {
            return &body;
        }
    }
    return nullptr;
}