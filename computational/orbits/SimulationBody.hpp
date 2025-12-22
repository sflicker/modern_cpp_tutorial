//
// Created by scott on 12/14/25.
//

#ifndef MODERN_TUTOR_SIMULATIONBODY_HPP
#define MODERN_TUTOR_SIMULATIONBODY_HPP

#include <cstdint>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/String.hpp>
#include "Vect2.hpp"
#include "Trail.hpp"

constexpr double INF = std::numeric_limits<double>::max();
using BodyID = uint32_t;
static constexpr BodyID INVALID_BODY_ID = BodyID(-1);

BodyID allocateId();

struct SimulationBody {
    BodyID id = INVALID_BODY_ID;
    std::string name;
    double mass;
    Vect2 position;
    Vect2 velocity;
    Vect2 force;
    double kinetic_energy;
    double potential_energy;
    sf::Color color;
    float drawRadius;
    bool showLabel;
    bool isStar = false;
    BodyID nearest_star = INVALID_BODY_ID;
    double nearest_star_dist = INF;
    Trail positions;


    //    Body(double mass, Vect2 &position, Vect2 &velocity, Vect2 &force, sf::Color & color, float radius);

    SimulationBody()
  : id(allocateId())
  , name("")
  , mass(0.0)
  , position{0,0}
    , velocity{0,0}
    , force{0,0}
    , kinetic_energy(0.0)
    , potential_energy(0.0)
    , color(sf::Color::White)
    , drawRadius(3.0f)
    , showLabel(false)  {};

    SimulationBody(BodyID id, std::string name, double mass, Vect2 position, Vect2 velocity, Vect2 force,
    double kinetic_energy, double potential_energy, sf::Color color, float radius, bool showLabel,
    bool isStar);
};

std::ostream& operator<<(std::ostream & os, const SimulationBody &b);

SimulationBody * get_body_by_id(std::vector<SimulationBody> & bodies, BodyID id);
SimulationBody * get_body_by_name(std::vector<SimulationBody> & bodies, std::string name);

#endif //MODERN_TUTOR_SIMULATIONBODY_HPP