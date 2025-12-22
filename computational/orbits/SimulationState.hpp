//
// Created by scott on 12/14/25.
//

#ifndef MODERN_TUTOR_SIMULATIONSTATE_HPP
#define MODERN_TUTOR_SIMULATIONSTATE_HPP

#include "SimulationBody.hpp"

struct SimulationState {
    std::vector<SimulationBody> bodies;
    double dt = 0.01;
    double t = 0.0;
    bool activeSimulation;
    double G = 1;
    bool showTrails = true;
    double boundary = 35000;
};

#endif //MODERN_TUTOR_SIMULATIONSTATE_HPP