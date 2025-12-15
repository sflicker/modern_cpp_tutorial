//
// Created by scott on 12/14/25.
//

#pragma once

#include <nlohmann/json.hpp>
#include "Vect2.hpp"
#include "SimulationBody.hpp"
#include "SimulationState.hpp"

using json = nlohmann::json;

inline void to_json(json& j, const Vect2& v) {
    j = json::array({v.x, v.y});
}

inline void from_json(const json& j, Vect2& v) {
    if (!j.is_array() || j.size() != 2) throw new std::runtime_error("Vect2 must by [x,y]");
    v.x = j.at(0).get<double>();
    v.y = j.at(1).get<double>();
}


inline void to_json(json& j, const SimulationBody& b) {
    j = json{
                {"name", b.name},
                {"mass", b.mass},
                {"position", b.position},
                {"velocity", b.velocity},
                { "force", b.force},
                { "KE", b.kinetic_energy},
                { "PE", b.potential_energy},
                {"drawRadius", b.drawRadius},
                {"color", json::array({b.color.r, b.color.g, b.color.b, b.color.a})}};
}

inline void from_json(const json& j, SimulationBody& b) {
    b.name = j.value("name", "");
    b.mass = j.at("mass").get<double>();
    b.position = j.at("position").get<Vect2>();
    b.velocity = j.at("velocity").get<Vect2>();
    b.drawRadius = j.value("drawRadius", 3.0f);

    auto c = j.value("color", json::array({255,255,255,255}));
    if (!c.is_array() || c.size() != 4) throw std::runtime_error("color must be [r,g,b,a]");
    b.color.r = c.at(0).get<std::uint8_t>();
    b.color.g = c.at(1).get<std::uint8_t>();
    b.color.b = c.at(2).get<std::uint8_t>();
    b.color.a = c.at(3).get<std::uint8_t>();
}


inline void saveSimulation(const SimulationState & sim, const std::string& path) {
    json j;
    j["version"] = 1;
    j["globals"] = {{ "G", sim.G},
        {"t", sim.t},
        {"dt", sim.dt}};

    j["activeSimulation"] = sim.activeSimulation;
    j["boundary"] = sim.boundary;
    j["bodies"] = sim.bodies;

    std::ofstream out(path);
    if (!out) throw std::runtime_error("Failed to open for writing:" + path);
    out << j.dump(2) << std::endl;
}

inline SimulationState loadSimulation(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("Failed to open for reading:" + path);

    json j;
    in >> j;

    const int version = j.value("version", 0);
    if (version != 1) throw std::runtime_error("Unsupported save version: " + std::to_string(version));

    SimulationState sim;
    const auto& g = j.at("globals");
    sim.G = g.value("G", sim.G);
    sim.t = g.value("t", 0.0);
    sim.dt = g.value("dt", sim.dt);

    sim.activeSimulation = j.at("activeSimulation");
    sim.boundary = j.at("boundary");

    sim.bodies = j.at("bodies").get<std::vector<SimulationBody>>();
    return sim;
}
