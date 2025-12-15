//
// Created by scott on 11/27/25.
//

#include "orbit_main.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <bits/this_thread_sleep.h>
#include <SFML/Graphics/Color.hpp>
#include <fmt/core.h>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include "Vect2.hpp"
#include "../../graphics_display/src/graphics_display.hpp"
#include "SimulationBody.hpp"
#include "SimulationState.hpp"
#include "Trail.hpp"
#include "SimulationBodyJson.hpp"
#include "Colors.hpp"

// sf::Color planet_color = sf::Color::Blue;
// sf::Color STAR_COLOR = sf::Color(255, 230, 160);     // bright yellow white
// sf::Color MERCURY_COLOR = sf::Color(140, 140, 140);   // dark grey


int star_radius = 20;
int planet_radius = 6;

Vect2 center;

int XMAX = 800;
int YMAX = 800;




void computeForces(SimulationState & sim) {

    double softening = 2.0;
    double soft2 = softening * softening;

    // clear old forces
    for (auto &body : sim.bodies) {
        body.force = Vect2(0.f, 0.f);
        body.potential_energy = 0.0;
    }

    double totalPotential = 0.0;

    // pairwise forces
    const std::size_t N = sim.bodies.size();
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = i + 1; j < N; ++j) {
            Vect2 d = sim.bodies[j].position - sim.bodies[i].position;
            double r2 = d.x * d.x + d.y * d.y + soft2;
            double r = sqrt(r2);
            // if (r < 1 || r > 10000) {
            //     std::cout << "r: " << r << ", i: " << i << ", j: " << j << std::endl;
            // }
            double inv_r = 1.0 / r;
            double inv_r3 = inv_r / r2;

            // double soft = 1.0;
            // double inv_r3 = 1.0 / std::pow(r2+soft * soft, 1.5);

            double fmag = sim.G * sim.bodies[i].mass * sim.bodies[j].mass * inv_r3;
            Vect2 F = fmag * d;

            sim.bodies[i].force += F;
            sim.bodies[j].force -= F;

            double Uij = -sim.G * sim.bodies[i].mass * sim.bodies[j].mass / (sqrt(d.x*d.x + d.y*d.y));
            totalPotential += Uij;

            sim.bodies[i].potential_energy += 0.5 * Uij;
            sim.bodies[j].potential_energy += 0.5 * Uij;
        }
    }
}

void integrate(SimulationState & sim) {

    center = Vect2((sim.bodies[0].position.x + sim.bodies[1].position.x)/2, (sim.bodies[0].position.y + sim.bodies[1].position.y)/2);

    computeForces(sim);

    // first half step
    for (auto &body : sim.bodies) {
        Vect2 accel = body.force * (1.0 / body.mass);

        body.velocity += 0.5 * accel * sim.dt;
        body.position += body.velocity * sim.dt;
        body.positions.addTrailPoint(Vect2(body.position.x, body.position.y));
    }

    computeForces(sim);

    // second half step
    for (auto &body : sim.bodies) {
        Vect2 accel = body.force * (1.0 / body.mass);

        body.velocity += 0.5 * accel * sim.dt;
        body.kinetic_energy = 0.5 * body.mass * (body.velocity.x * body.velocity.x + body.velocity.y * body.velocity.y);
    }
}

void update_display(GraphicsDisplay & display, SimulationState & sim) {
    // if (display.isOpen()) {
    //     display.pollEvents();

        display.clear();

        std::string time_str = fmt::format("Time: {:.3f}", sim.t);
        display.drawText(0, 0, time_str);

        std::string center_str = fmt::format("Center: [{:.3f}, {:.3f}]", center.x , center.y);
        display.drawText(0, 30, center_str);

        double starSeparation = sim.bodies[0].position.dist(sim.bodies[1].position);

        std::string starSepStr = fmt::format("Star separation: {:.3f}", starSeparation);
        display.drawText(0, 60, starSepStr);

        display.drawCrosshairAtWorld(center.x, center.y);

        // Body & body = bodies[15];
        // double newDistance = center.dist(body.position);
        // std::string body_str = fmt::format("Body - pos: [{:.3f}, {:.3f}], velo: [{:3f}, {:3f}], KE: {:.3f}, PE: {:.3f}, dist: {:.3f}",
        //     body.position.x, body.position.y, body.velocity.x, body.velocity.y, body.kinetic_energy, body.potential_energy, newDistance);
        // display.drawText(0, 60, body_str);

        if (!sim.activeSimulation) {
            std::string paused_str = "Paused";
            display.drawText(XMAX-100, 0, paused_str);
        }

        // std::string steps_per_sec_str = fmt::format("StepsPerSecond: {:d}", simStepCountPrev);
        // display.drawText(0, 30, steps_per_sec_str);


        for (const auto &body : sim.bodies) {
            display.drawCircle(body.position.x - body.drawRadius, body.position.y - body.drawRadius, body.drawRadius, body.color);
            if (body.positions.getVertexCount() > 2) {
                 display.drawLines(body.positions.getTrail());
            }
        }

        display.display();
//    }
}


void update_info(GraphicsDisplay & infoWindow, SimulationState & sim) {

    infoWindow.clear();
    int count = 0;
    for (auto &body : sim.bodies) {
        std::string body_str = fmt::format("Body - {}, KE: {:.3f}, PE: {:.3f}, E: {:3f}",
            body.name, body.kinetic_energy, body.potential_energy, (body.kinetic_energy+body.potential_energy));
        infoWindow.drawText(0, count*30, body_str);
        count++;
    }
    infoWindow.display();
}

void setupSimulationBodies(SimulationState & sim) {

    double R = 100.0;
    double M = 10000.0;
    double m0 = 1.0;

    double r = 40.0 * R;

    center = Vect2(0.0, 0.0);

    SimulationBody star("Sun", M, Vect2(0,0), Vect2(0, 0 /*-sqrt(G*M/(r))*/ ), Vect2(0,0), 0, 0, Colors::Star, 40);
//    star.positions.setMaxPoints(2500);

    r = 0.4*R; double m = 0.55 * m0;
    SimulationBody planet1("Mercury", m, Vect2(r, 0), Vect2(0, -sqrt(sim.G*M/r)) , Vect2(0,0), 0, 0, Colors::Mercury, 10);

    r = 0.7*R; m = 0.815 * m0;
    SimulationBody planet2("Venus", m, Vect2(-r, 0), Vect2(0, sqrt(sim.G*M/(r))), Vect2(0,0), 0, 0, Colors::Venus, 10);

    r = R; m = m0;
    SimulationBody planet3("Earth", m, Vect2(r, 0), Vect2(0, -sqrt(sim.G*M/(r))), Vect2(0,0), 0, 0, Colors::Earth, 10);

    r = 1.5 * R; m = .01 *m0;
    SimulationBody planet4("Mars", m, Vect2(-r, 0), Vect2(0, sqrt(sim.G*M/(r))), Vect2(0,0), 0, 0, Colors::Mars, 10);

    r = 2.8 * R; m = .01 * m0;
    SimulationBody asteroid1("Asteroid1", m, Vect2(r, 0), Vect2(0, -sqrt(sim.G*M/(r))), Vect2(0,0), 0, 0, Colors::Asteroid, 10);
    SimulationBody asteroid2("Asteroid2", m, Vect2(0, r), Vect2(sqrt(sim.G*M/(r)), 0), Vect2(0,0), 0, 0, Colors::Asteroid, 10);
    SimulationBody asteroid3("Asteroid3", m, Vect2(-r, 0), Vect2(0, sqrt(sim.G*M/(r))), Vect2(0,0), 0, 0, Colors::Asteroid, 10);
    SimulationBody asteroid4("Asteroid4", m, Vect2(0, -r), Vect2(-sqrt(sim.G*M/(r)), 0), Vect2(0,0), 0, 0, Colors::Asteroid, 10);

    r = 5.2 * R; m = 100 * m0;
    SimulationBody planet5("Jupiter", m, Vect2(-r, 0), Vect2(0, sqrt(sim.G*M/(r))), Vect2(0,0), 0, 0, Colors::Jupiter, 25);
//    planet5.positions.setMaxPoints(1000);

    r = 9.0 * R; m = 50 * m0;
    SimulationBody planet6("Saturn", m, Vect2(r, 0), Vect2(0, -sqrt(sim.G*M/(r))), Vect2(0,0), 0, 0, Colors::Saturn, 20);
//    planet6.positions.setMaxPoints(1000);

    r = 19.0 * R; m = 10 * m0;
    SimulationBody planet7("Uranus", m, Vect2(-r, 0), Vect2(0, sqrt(sim.G*M/(r))), Vect2(0,0), 0, 0, Colors::Uranus, 20);
//    planet7.positions.setMaxPoints(1000);

    r = 30.0 * R; m = 10 * m0;
    SimulationBody planet8("Neptune", m, Vect2(r, 0), Vect2(0, -sqrt(sim.G*M/(r))), Vect2(0,0), 0, 0, Colors::Neptune, 20);


    r = 40.0 * R; m = 0.01 * m0;
    SimulationBody comet1("Comet1", m, Vect2(-r, 0), Vect2(0, 0.15*sqrt(sim.G*M/(r))), Vect2(0,0), 0, 0, Colors::Comet, 10);
    comet1.positions.setMaxPoints(1000);

    r = 35.0 * R; m = 0.01 * m0;
    SimulationBody comet2("Comet2", m, Vect2(r, 0), Vect2(0, -0.2*sqrt(sim.G*M/(r))), Vect2(0,0), 0, 0, Colors::Comet, 10);
    comet2.positions.setMaxPoints(1000);
    SimulationBody comet3("Comet3", m, Vect2(0, r), Vect2(-0.1*sqrt(sim.G*M/(r)), 0), Vect2(0,0), 0, 0, Colors::Comet, 10);
    comet3.positions.setMaxPoints(1000);
    SimulationBody comet4("Comet4", m, Vect2(0, -r), Vect2(0.05*sqrt(sim.G*M/(r)), 0), Vect2(0,0), 0, 0, Colors::Comet, 10);
    comet4.positions.setMaxPoints(1000);

    r = 100.0 * R; m = M;
    SimulationBody star2("Sirius", M, Vect2(-r, 0), Vect2(0, 0.8*sqrt(sim.G*M/(r))), Vect2(0,0), 0, 0, Colors::Star, 40);
//    star2.positions.setMaxPoints(2500);

    double r2 = r + 100;
    m = 0.3*m0;
    SimulationBody planet10("PlanetX", m0, Vect2(-r2, 0), Vect2(0, sqrt(sim.G*M/(100))), Vect2(0, 0), 0, 0, Colors::Mercury, 10);

    r2 = r + 200;
    m = m0;
    SimulationBody planet11("PlanetY", m0, Vect2(-r2, 0), Vect2(0, sqrt(sim.G*M/(200))), Vect2(0, 0), 0, 0, Colors::Earth, 10);

    m = 10*m0;
    r2 = r + 400;
    SimulationBody planet12("PlanetZ", m0, Vect2(-r2, 0), Vect2(0, sqrt(sim.G*M/(400))), Vect2(0, 0), 0, 0, Colors::Jupiter, 20);

    r2 = r + 1200;
    m = 0.01*m0;
    SimulationBody comet5("Comet5", m, Vect2(-r2, 0), Vect2(0, sqrt(0.2*sim.G*M/(1200))), Vect2(0, 0), 0, 0, Colors::Comet, 10);

    sim.bodies.push_back(star);
    sim.bodies.push_back(star2);

    sim.bodies.push_back(planet1);
    sim.bodies.push_back(planet2);
    sim.bodies.push_back(planet3);
    sim.bodies.push_back(planet4);
    sim.bodies.push_back(asteroid1);
    sim.bodies.push_back(asteroid2);
    sim.bodies.push_back(asteroid3);
    sim.bodies.push_back(asteroid4);
    sim.bodies.push_back(planet5);
    sim.bodies.push_back(planet6);
    sim.bodies.push_back(planet7);
    sim.bodies.push_back(planet8);
    sim.bodies.push_back(comet1);
    sim.bodies.push_back(comet2);
    sim.bodies.push_back(comet3);
    sim.bodies.push_back(comet4);
    sim.bodies.push_back(planet10);
    sim.bodies.push_back(planet11);
    sim.bodies.push_back(planet12);
    sim.bodies.push_back(comet5);
    //

    // star.positions.addTrailPoint(Vect2(star.position.x, star.position.y));
    // planet1.positions.addTrailPoint(Vect2(planet1.position.x, planet1.position.y), planet_color);
    // planet2.positions.addTrailPoint(Vect2(planet2.position.x, planet2.position.y), planet_color);
    // planet3.positions.addTrailPoint(Vect2(planet3.position.x, planet3.position.y), planet_color);

    std::cout << planet1 << ", " << planet2 << std::endl;
}

void bounceBodiesOnTheEdge(SimulationState & sim) {
    for (int i = sim.bodies.size() - 1; i >= 0; --i) {
        double distanceToCenter = center.dist(sim.bodies[i].position);
        if (distanceToCenter > sim.boundary) {
            std::cout << "Removing body " << sim.bodies[i].name << " from simulation" << std::endl;
            sim.bodies.erase(sim.bodies.begin() + i);
            // Body & body = bodies[i];
            // std::cout << "Simulation Object " << i << " Crossed the outer edge with distance "
            //         << distanceToCenter << ". Bouncing it back" << std::endl;;
            //
            // // reverse velocities
            // bodies[i].velocity.x = -bodies[i].velocity.x;
            // bodies[i].velocity.y = -bodies[i].velocity.y;
            //
            // // jump 100 time steps with reverse velocities to avoid bunching again
            // bodies[i].position.x += 100*dt*bodies[i].velocity.x;
            // bodies[i].position.y += 100*dt*bodies[i].velocity.y;
            //
            // // scale velocities
            // bodies[i].velocity.x *= 0.1;
            // bodies[i].velocity.y *= 0.1;
            //
            // //bodies[i].positions.clear();
            //
            // // reset forces on body
            // bodies[i].force.x = 0;
            // bodies[i].force.y = 0;
            //
            // double newDistance = center.dist(bodies[i].position);
            // std::cout << "New distance = " << newDistance << std::endl;
        }
    }
}


int main() {
    std::cout << "Starting Orbits!\n";

    GraphicsDisplay graphics_disp(XMAX, YMAX, "Orbits");
    GraphicsDisplay infoWindow(600, 800, "Simulation Objects");

    SimulationState sim;
    sim.t = 0;
    sim.dt = 0.01;


   // int nSteps = 1000000;
    int launchStep = 1000;
    sim.activeSimulation = true;

    std::size_t simStepCount = 0;
    sf::Clock simStepTimer;
    std::size_t simStepCountPrev = 0;

    sf::View view;

    view.setCenter(0.f, 0.f);
    view.setSize(5000.f, 5000.f);

    graphics_disp.setView(view);

    setupSimulationBodies(sim);

    update_display(graphics_disp, sim);

    int trackedIndex = 0;


    while (infoWindow.isOpen() && graphics_disp.isOpen()) {

        sf::Event event;

        while (infoWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                infoWindow.close();
            }
        }

        while (graphics_disp.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                graphics_disp.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
                sim.activeSimulation = !(sim.activeSimulation);       // pause key
            }

        }

        if (graphics_disp.isOpen() && graphics_disp.pollEvents()) {

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                exit(0);
            }


            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                saveSimulation(sim, "savesim.json");
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
                sim = loadSimulation("savesim.json");
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) {
                view.zoom(0.999f);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) {
                view.zoom(1.001f);
            }

            float panSpeed = 10.0f;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0)) trackedIndex = 0;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) trackedIndex = 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) trackedIndex = 2;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) trackedIndex = 3;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) trackedIndex = 4;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9)) trackedIndex = 9;

            if (trackedIndex == 0) {
                view.setCenter(0.f, 0.f);
            }
            else if (trackedIndex == 9) {
                view.setCenter(center.x, center.y);
            }
            else if (trackedIndex > 0 && trackedIndex <= sim.bodies.size()) {
                view.setCenter(sim.bodies[trackedIndex - 1].position.x, sim.bodies[trackedIndex - 1].position.y);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                view.move(-panSpeed, 0.f);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) view.move(panSpeed, 0.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) view.move(0.f, -panSpeed);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) view.move(0.f, panSpeed);



            graphics_disp.setView(view);

            // if (n == launchStep) {
            //
            // }

            if (sim.activeSimulation) {
                //computeForces(bodies);
                bounceBodiesOnTheEdge(sim);
                integrate(sim);
                simStepCount++;
                sim.t += sim.dt;
            }

            // if (n % 100 == 0) {
            //     std::cout << "time: " << t <<  ", planet: " << bodies[1] << std::endl;
            // }

            // redraw every 1/60 of a second (16.666667 ms) based on current simulation coordinates.
            if (simStepTimer.getElapsedTime().asMilliseconds() >= 16.66667f) {
                update_display(graphics_disp, sim); //planet1, planet2, planet1_positions, planet2_positions);
                update_info(infoWindow, sim);
                simStepCount = 0;
                simStepTimer.restart();
            }
        }
//        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }


}
