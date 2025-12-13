//
// Created by scott on 11/27/25.
//

#include "orbit_main.hpp"

#include <cmath>
#include <iostream>
#include <bits/this_thread_sleep.h>
#include <SFML/Graphics/Color.hpp>
#include <fmt/core.h>

#include "../../graphics_display/src/graphics_display.hpp"

#include "Trail.hpp"

sf::Color planet_color = sf::Color::Blue;
sf::Color star_color = sf::Color::Yellow;

int star_radius = 20;
int planet_radius = 6;

Vect2 center;

double boundary = 25000;

double G = 1;

int XMAX = 800;
int YMAX = 800;

Body::Body(double mass, Vect2 position, Vect2 velocity, Vect2 force, sf::Color color, float radius) :
    mass(mass), position(position), velocity(velocity), force(force), color(color), radius(radius) {};

std::ostream& operator<<(std::ostream& os, const Vect2& v) {
    os << "[" << v.x << ", " << v.y << "]";
    return os;
}

std::ostream& operator<<(std::ostream & os, const Body &b) {
    os << "mass: " << b.mass << ", position: " << b.position << ", velocity: " << b.velocity << ", force: " << b.force
        << ", KE: " << b.kinetic_energy << ", PE: " << b.potential_energy;
    return os;
}

void computeForces(std::vector<Body> &bodies) {

    double softening = 2.0;
    double soft2 = softening * softening;

    // clear old forces
    for (auto &body : bodies) {
        body.force = Vect2(0.f, 0.f);
        body.potential_energy = 0.0;
    }

    double totalPotential = 0.0;

    // pairwise forces
    const std::size_t N = bodies.size();
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = i + 1; j < N; ++j) {
            Vect2 d = bodies[j].position - bodies[i].position;
            double r2 = d.x * d.x + d.y * d.y + soft2;
            double r = sqrt(r2);
            // if (r < 1 || r > 10000) {
            //     std::cout << "r: " << r << ", i: " << i << ", j: " << j << std::endl;
            // }
            double inv_r = 1.0 / r;
            double inv_r3 = inv_r / r2;

            // double soft = 1.0;
            // double inv_r3 = 1.0 / std::pow(r2+soft * soft, 1.5);

            double fmag = G * bodies[i].mass * bodies[j].mass * inv_r3;
            Vect2 F = fmag * d;

            bodies[i].force += F;
            bodies[j].force -= F;

            double Uij = -G * bodies[i].mass * bodies[j].mass / (sqrt(d.x*d.x + d.y*d.y));
            totalPotential += Uij;

            bodies[i].potential_energy += 0.5 * Uij;
            bodies[j].potential_energy += 0.5 * Uij;
        }
    }
}

void integrate(std::vector<Body> &bodies, double dt) {

    center = Vect2((bodies[0].position.x + bodies[1].position.x)/2, (bodies[0].position.y + bodies[1].position.y)/2);

    computeForces(bodies);

    // first half step
    for (auto &body : bodies) {
        Vect2 accel = body.force * (1.0 / body.mass);

        body.velocity += 0.5 * accel * dt;
        body.position += body.velocity * dt;
        body.positions.addTrailPoint(Vect2(body.position.x, body.position.y));
    }

    computeForces(bodies);

    // second half step
    for (auto &body : bodies) {
        Vect2 accel = body.force * (1.0 / body.mass);

        body.velocity += 0.5 * accel * dt;
        double velocity_mag = body.velocity.x * body.velocity.x + body.velocity.y * body.velocity.y;
        body.kinetic_energy = 0.5 * body.mass * velocity_mag*velocity_mag;
    }
}

void update_display(GraphicsDisplay & display, std::vector<Body> &bodies, double time, bool activeSimulation) {
    // if (display.isOpen()) {
    //     display.pollEvents();

        display.clear();

        std::string time_str = fmt::format("Time: {:.3f}", time);
        display.drawText(0, 0, time_str);

        std::string center_str = fmt::format("Center: [{:.3f}, {:.3f}]", center.x , center.y);
        display.drawText(0, 30, center_str);

        display.drawCrosshairAtWorld(center.x, center.y);

        Body & body = bodies[15];
        double newDistance = center.dist(body.position);
        std::string body_str = fmt::format("Body - pos: [{:.3f}, {:.3f}], velo: [{:3f}, {:3f}], KE: {:.3f}, PE: {:.3f}, dist: {:.3f}",
            body.position.x, body.position.y, body.velocity.x, body.velocity.y, body.kinetic_energy, body.potential_energy, newDistance);
        display.drawText(0, 60, body_str);

        if (!activeSimulation) {
            std::string paused_str = "Paused";
            display.drawText(0, 0, paused_str);
        }

        // std::string steps_per_sec_str = fmt::format("StepsPerSecond: {:d}", simStepCountPrev);
        // display.drawText(0, 30, steps_per_sec_str);


        for (const auto &body : bodies) {
            display.drawCircle(body.position.x - body.radius, body.position.y - body.radius, body.radius, body.color);
            if (body.positions.getVertexCount() > 2) {
                 display.drawLines(body.positions.getTrail());
            }
        }

        display.display();
//    }
}

void setupSimulationBodies(std::vector<Body> &bodies) {

    double R = 100.0;
    double M = 10000.0;
    double m0 = 1.0;

    double r = 40.0 * R;

    center = Vect2(0.0, 0.0);

    Body star(M, Vect2(0,0), Vect2(0, 0 /*-sqrt(G*M/(r))*/ ), Vect2(0,0), star_color, 40);
//    star.positions.setMaxPoints(2500);

    r = 0.4*R; double m = 0.55 * m0;
    Body planet1(m, Vect2(r, 0), Vect2(0, -sqrt(G*M/r)) , Vect2(0,0), sf::Color::Magenta, 10);

    r = 0.7*R; m = 0.815 * m0;
    Body planet2(m, Vect2(-r, 0), Vect2(0, sqrt(G*M/(r))), Vect2(0,0), sf::Color::Magenta, 10);

    r = R; m = m0;
    Body planet3(m, Vect2(r, 0), Vect2(0, -sqrt(G*M/(r))), Vect2(0,0), planet_color, 10);

    r = 1.6 * R; m = .01 *m0;
    Body planet4(m, Vect2(-r, 0), Vect2(0, sqrt(G*M/(r))), Vect2(0,0), sf::Color::Red, 10);

    r = 2.8 * R; m = .01 * m0;
    Body asteroid1(m, Vect2(r, 0), Vect2(0, -sqrt(G*M/(r))), Vect2(0,0), sf::Color::Magenta, 6);
    Body asteroid2(m, Vect2(0, r), Vect2(sqrt(G*M/(r)), 0), Vect2(0,0), sf::Color::Magenta, 6);
    Body asteroid3(m, Vect2(-r, 0), Vect2(0, sqrt(G*M/(r))), Vect2(0,0), sf::Color::Magenta, 6);
    Body asteroid4(m, Vect2(0, -r), Vect2(-sqrt(G*M/(r)), 0), Vect2(0,0), sf::Color::Magenta, 6);

    r = 5.2 * R; m = 100 * m0;
    Body planet5(m, Vect2(-r, 0), Vect2(0, 0.95*sqrt(G*M/(r))), Vect2(0,0), sf::Color::Cyan, 25);
//    planet5.positions.setMaxPoints(1000);

    r = 9.0 * R; m = 50 * m0;
    Body planet6(m, Vect2(r, 0), Vect2(0, -sqrt(G*M/(r))), Vect2(0,0), sf::Color::Cyan, 20);
//    planet6.positions.setMaxPoints(1000);

    r = 15.0 * R; m = 10 * m0;
    Body planet7(m, Vect2(-r, 0), Vect2(0, sqrt(G*M/(r))), Vect2(0,0), sf::Color::Cyan, 20);
//    planet7.positions.setMaxPoints(1000);

    r = 35.0 * R; m = 0.01 * m0;
    Body comet1(m, Vect2(-r, 0), Vect2(0, 0.15*sqrt(G*M/(r))), Vect2(0,0), sf::Color::White, 10);
    comet1.positions.setMaxPoints(1000);

    r = 25.0 * R; m = 0.01 * m0;
    Body comet2(m, Vect2(r, 0), Vect2(0, -0.2*sqrt(G*M/(r))), Vect2(0,0), sf::Color::White, 10);
    comet2.positions.setMaxPoints(1000);
    Body comet3(m, Vect2(0, r), Vect2(-0.1*sqrt(G*M/(r)), 0), Vect2(0,0), sf::Color::White, 10);
    comet3.positions.setMaxPoints(1000);
    Body comet4(m, Vect2(0, -r), Vect2(0.05*sqrt(G*M/(r)), 0), Vect2(0,0), sf::Color::White, 10);
    comet4.positions.setMaxPoints(1000);

    r = 40.0 * R; m = M;
    Body star2(M, Vect2(-r, 0), Vect2(0, 0.8*sqrt(G*M/(r))), Vect2(0,0), sf::Color::Yellow, 40);
//    star2.positions.setMaxPoints(2500);

    double r2 = r + 100;
    Body planet10(m0, Vect2(-r2, 0), Vect2(0, sqrt(G*M/(100))), Vect2(0, 0), sf::Color::Magenta, 10);


    bodies.push_back(star);
    bodies.push_back(star2);

    bodies.push_back(planet1);
    bodies.push_back(planet2);
    bodies.push_back(planet3);
    bodies.push_back(planet4);
    bodies.push_back(asteroid1);
    bodies.push_back(asteroid2);
    bodies.push_back(asteroid3);
    bodies.push_back(asteroid4);
    bodies.push_back(planet5);
    bodies.push_back(planet6);
    bodies.push_back(planet7);
    bodies.push_back(comet1);
    bodies.push_back(comet2);
    bodies.push_back(comet3);
    bodies.push_back(comet4);
    bodies.push_back(planet10);
    //

    star.positions.addTrailPoint(Vect2(star.position.x, star.position.y));
    planet1.positions.addTrailPoint(Vect2(planet1.position.x, planet1.position.y), planet_color);
    planet2.positions.addTrailPoint(Vect2(planet2.position.x, planet2.position.y), planet_color);
    planet3.positions.addTrailPoint(Vect2(planet3.position.x, planet3.position.y), planet_color);

    std::cout << planet1 << ", " << planet2 << std::endl;
}

void bounceBodiesOnTheEdge(std::vector<Body> & bodies, double dt) {
    return;  // skip for now
    for (int i = 2; i < bodies.size(); i++) {
        double distanceToCenter = center.dist(bodies[i].position);
        if (distanceToCenter > boundary) {
            Body & body = bodies[i];
            std::cout << "Simulation Object " << i << " Crossed the outer edge with distance "
                    << distanceToCenter << ". Bouncing it back" << std::endl;;

            // reverse velocities
            bodies[i].velocity.x = -bodies[i].velocity.x;
            bodies[i].velocity.y = -bodies[i].velocity.y;

            // jump 100 time steps with reverse velocities to avoid bunching again
            bodies[i].position.x += 100*dt*bodies[i].velocity.x;
            bodies[i].position.y += 100*dt*bodies[i].velocity.y;

            // scale velocities
            bodies[i].velocity.x *= 0.1;
            bodies[i].velocity.y *= 0.1;

            //bodies[i].positions.clear();

            // reset forces on body
            bodies[i].force.x = 0;
            bodies[i].force.y = 0;

            double newDistance = center.dist(bodies[i].position);
            std::cout << "New distance = " << newDistance << std::endl;
        }
    }
}


int main() {
    std::cout << "Starting Orbits!\n";

    GraphicsDisplay graphics_disp(XMAX, YMAX, "Orbits");

    std::vector<Body> bodies;

    double dt = 0.01;
    double t = 0.0;
   // int nSteps = 1000000;
    int launchStep = 1000;
    bool activeSimulation = true;

    std::size_t simStepCount = 0;
    sf::Clock simStepTimer;
    std::size_t simStepCountPrev = 0;

    sf::View view;

    view.setCenter(0.f, 0.f);
    view.setSize(5000.f, 5000.f);

    graphics_disp.setView(view);

    setupSimulationBodies(bodies);

    update_display(graphics_disp, bodies, t, activeSimulation);

    int trackedIndex = 0;


    while (true) {

        if (graphics_disp.isOpen()) {
            graphics_disp.pollEvents();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                exit(0);
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
            else if (trackedIndex > 0 && trackedIndex <= bodies.size()) {
                view.setCenter(bodies[trackedIndex - 1].position.x, bodies[trackedIndex - 1].position.y);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                view.move(-panSpeed, 0.f);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) view.move(panSpeed, 0.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) view.move(0.f, -panSpeed);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) view.move(0.f, panSpeed);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                !activeSimulation;       // pause key
            }

            graphics_disp.setView(view);

            // if (n == launchStep) {
            //
            // }

            if (activeSimulation) {
                //computeForces(bodies);
                bounceBodiesOnTheEdge(bodies, dt);
                integrate(bodies, dt);
                simStepCount++;
                t += dt;
            }

            // if (n % 100 == 0) {
            //     std::cout << "time: " << t <<  ", planet: " << bodies[1] << std::endl;
            // }

            // redraw every 1/60 of a second (16.666667 ms) based on current simulation coordinates.
            if (simStepTimer.getElapsedTime().asMilliseconds() >= 16.66667f) {
                update_display(graphics_disp, bodies, t, activeSimulation); //planet1, planet2, planet1_positions, planet2_positions);
                simStepCount = 0;
                simStepTimer.restart();
            }
        }
//        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }


}
