//
// Created by scott on 12/22/25.
//

#pragma once
#include <algorithm>
#include "Vect2.hpp"
#include "SimulationState.hpp"
#include "SimulationBody.hpp"

class TrajectoryLogger {
public:
    struct Config {
        bool include_header = true;
    };


    explicit TrajectoryLogger(Config cfg={}) : cfg_(std::move(cfg)) {}

    bool open(const std::string& filename) {
        close();

        file_ = std::fopen(path.c_str(), "wb");
        if (!file_) return false;

        line_count_ = 0;
        buffer_.clear();
        buffer_.reserve(1 << 20);
        next_sample_t_ = 0.0;

        // format t, id, name, mass, px, py, vx, vy, fx, fy
        if (cfg_.include_header) {
            buffer_ += "t, id, name, mass, px, py, vx, vy, fx, fy\n";
        }

        flush_if_needed(true);

        return true;
    }

    void close() {
        if (file_) {
            flush_if_needed(true);
            std::fclose(file_);
            file_ = nullptr;
        }
        buffer_.clear();
    }

    bool is_open() const { return file_ != nullptr; }

    // format t, id, name, mass, px, py, vx, vy, fx, fy
    void update_log(SimulationState & sim, std::vector<std::string> & bodiesToLog) {

        if (!file_) return;

        // only record when we reach next sample time.
        if (sim.t + 1e-12 < next_sample_t_) return;

        next_sample_t_ = sim.t + dt_sample;

        auto f = [&](double v) {
            return fmt::format("{:.{}g}", v, cfg_.precision);
        };


        if (!bodiesToLog.empty()) {
            for (size_t i=0;i<bodiesToLog.size();++i) {
                SimulationBody * body = get_body_by_name(sim.bodies, bodiesToLog[i]);
                if (body != nullptr) {
                    double total_energy = body->kinetic_energy+body->potential_energy;
                    SimulationBody * nearest_star = get_body_by_id(sim.bodies, body->nearest_star);
                    std::string nearest_star_name = nearest_star != nullptr ? nearest_star->name : "";

                    std::string body_str = fmt::format("{:.4f},{:d},{},{:7f},{:7f},{:7f},{:7f},{:7f},{:7f},{:7f}",
                    sim.t, body->id, body->name, body->mass, body->position.x, body->position.y,
                    body->velocity.x, body->velocity.y,
                    body->force.x, body->force.y);

                    std::cout << body_str << std::endl;

                }
            }
        }
    }

private:
    Config cfg_;
    std::FILE* file_ = nullptr;

    std::string buffer_;
    size_t line_count_ = 0;
    double next_sample_t_ = 0.0;


};
#endif //MODERN_TUTOR_TRAJECTORYLOGGER_HPP