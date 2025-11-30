//
// Created by scott on 11/27/25.
//

#include "orbit_main.hpp"

#include <cmath>
#include <iostream>
#include <bits/this_thread_sleep.h>
#include <SFML/Graphics/Color.hpp>

#include "../../graphics_display/src/graphics_display.hpp"

#include "Trail.hpp"

sf::Color planet_color = sf::Color::Blue;
sf::Color star_color = sf::Color::Yellow;

int star_radius = 20;
int planet_radius = 6;

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
    os << "mass: " << b.mass << ", position: " << b.position << ", velocity: " << b.velocity << ", force: " << b.force;
    return os;
}

void computeForces(std::vector<Body> &bodies) {
    // clear old forces
    for (auto &body : bodies) {
        body.force = Vect2(0.f, 0.f);
    }

    // pairwise forces
    const std::size_t N = bodies.size();
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = i + 1; j < N; ++j) {
            Vect2 d = bodies[j].position - bodies[i].position;
            double r2 = d.x * d.x + d.y * d.y;
            double r = sqrt(r2);

            double soft = 1.0;
            double inv_r3 = 1.0 / std::pow(r2+soft * soft, 1.5);

            Vect2 F = G * bodies[i].mass * bodies[j].mass * inv_r3 * d;

            bodies[i].force += F;
            bodies[j].force -= F;
        }
    }
}

void integrate(std::vector<Body> &bodies, double dt) {

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
    }
}

void update_display(GraphicsDisplay & display, std::vector<Body> &bodies) {
    // if (display.isOpen()) {
    //     display.pollEvents();

        display.clear();

        for (const auto &body : bodies) {
//            display.drawCircle(body.position.x + XMAX/2-body.radius, body.position.y + YMAX/2-body.radius, body.radius, body.color);
            display.drawCircle(body.position.x - body.radius, body.position.y - body.radius, body.radius, body.color);
//            display.drawCircle(body.position.x, body.position.y, body.radius, body.color);
            if (body.positions.getVertexCount() > 2) {
                display.drawLines(body.positions.getTrail());
            }
        }

        // display.drawCircle(XMAX/2-star_radius, YMAX/2-star_radius, star_radius, star_color);
        // Vect2 p1, p2;
        // if (planet1_positions.getVertexCount() > 2) {
        //     display.drawLines(planet1_positions.getTrail());
        // }
        // display.drawCircle(
        //     body1.position.x + XMAX/2 - planet_radius,
        //     body1.position.y + YMAX/2 - planet_radius,
        //     planet_radius,
        //     planet_color);
        //
        // if (planet2_positions.getVertexCount() > 2) {
        //     display.drawLines(planet2_positions.getTrail());
        // }
        //
        // display.drawCircle(
        //     body2.position.x + XMAX/2 - planet_radius,
        //     body2.position.y + YMAX/2 - planet_radius,
        //     planet_radius,
        //     planet_color);

        display.display();
//    }
}

int main() {
    std::cout << "Starting Orbits!\n";

    GraphicsDisplay graphics_disp(XMAX, YMAX, "Orbits");

    std::vector<Body> bodies;


    double R = 100.0;
    double M = 100.0;
    double m = 1.0;
    double dt = 0.1;
    double t = 0.0;
    int nSteps = 1000000;
    int launchStep = 1000;

    sf::View view;

    view.setCenter(0.f, 0.f);
    view.setSize(1500.f, 1500.f);

    graphics_disp.setView(view);

    Body star(M, Vect2(0,0), Vect2(0,0), Vect2(0,0), star_color, 20);
    Body planet1(m, Vect2(R, 0), Vect2(0, -sqrt(G*M/R) * 1.01) , Vect2(0,0), planet_color, 6);
    Body planet2(3*m, Vect2(-R*1.5, 0), Vect2(0, sqrt(G*M/(R*2))), Vect2(0,0), planet_color, 6);
    Body planet3(5*m, Vect2(R*3, 0), Vect2(0, -sqrt(G*M/(R*5))), Vect2(0,0), planet_color, 6);

    bodies.push_back(star);
    bodies.push_back(planet1);
    bodies.push_back(planet2);
    bodies.push_back(planet3);

    // Trail planet1_positions;
    // Trail planet2_positions;
    //
    // star.positions.addTrailPoint(Vect2(star.position.x + XMAX/2, star.position.y + YMAX/2));
    // planet1.positions.addTrailPoint(Vect2(planet1.position.x + XMAX/2, planet1.position.y + YMAX/2), planet_color);
    // planet2.positions.addTrailPoint(Vect2(planet2.position.x + XMAX/2, planet2.position.y + YMAX/2), planet_color);
    // planet3.positions.addTrailPoint(Vect2(planet3.position.x + XMAX/2, planet3.position.y + YMAX/2), planet_color);

    star.positions.addTrailPoint(Vect2(star.position.x, star.position.y));
    planet1.positions.addTrailPoint(Vect2(planet1.position.x, planet1.position.y), planet_color);
    planet2.positions.addTrailPoint(Vect2(planet2.position.x, planet2.position.y), planet_color);
    planet3.positions.addTrailPoint(Vect2(planet3.position.x, planet3.position.y), planet_color);

    std::cout << planet1 << ", " << planet2 << std::endl;
    update_display(graphics_disp, bodies /*planet1, planet2, planet1_positions, planet2_positions */);

    int trackedIndex = 0;

    for (int n = 0; n < nSteps; n++) {

        if (graphics_disp.isOpen()) {
            graphics_disp.pollEvents();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                exit(0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) {
                view.zoom(0.99f);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) {
                view.zoom(1.01f);
            }

            float panSpeed = 0.1f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) view.move(-panSpeed, 0.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) view.move(panSpeed, 0.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) view.move(0.f, -panSpeed);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) view.move(0.f, panSpeed);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0)) trackedIndex = 0;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) trackedIndex = 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) trackedIndex = 2;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) trackedIndex = 3;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) trackedIndex = 4;

            if (trackedIndex == 0) {
                view.setCenter(0.f, 0.f);
            }
            else if (trackedIndex > 0 && trackedIndex <= bodies.size()) {
                view.setCenter(bodies[trackedIndex - 1].position.x, bodies[trackedIndex - 1].position.y);
            }
            graphics_disp.setView(view);

            if (n == launchStep) {

            }

            //computeForces(bodies);
            integrate(bodies, dt);
            t += dt;

            // planet1
            // double r2 = planet1.position.x*planet1.position.x + planet1.position.y*planet1.position.y;
            // double r = sqrt(r2);
            // double inv_r3 = 1.0 / (r2 * r);
            // planet1.force.x = -G*M * inv_r3 * planet1.position.x;
            // planet1.force.y = -G*M * inv_r3 * planet1.position.y;
            //
            // planet1.velocity.x += planet1.force.x * dt;
            // planet1.velocity.y += planet1.force.y * dt;
            //
            // planet1.position.x += planet1.velocity.x * dt;
            // planet1.position.y += planet1.velocity.y * dt;
            // planet1_positions.addTrailPoint(Vect2(planet1.position.x + XMAX/2, planet1.position.y + YMAX/2), planet_color);
            //
            // // planet2
            // r2 = planet2.position.x*planet2.position.x + planet2.position.y*planet2.position.y;
            // r = sqrt(r2);
            // inv_r3 = 1.0 / (r2 * r);
            // planet2.force.x = -G*M * inv_r3 * planet2.position.x;
            // planet2.force.y = -G*M * inv_r3 * planet2.position.y;
            //
            // planet2.velocity.x += planet2.force.x * dt;
            // planet2.velocity.y += planet2.force.y * dt;
            //
            // planet2.position.x += planet2.velocity.x * dt;
            // planet2.position.y += planet2.velocity.y * dt;
            //
            // planet2_positions.addTrailPoint(Vect2(planet2.position.x + XMAX/2, planet2.position.y + YMAX/2), planet_color);
            //
            // t += dt;

            if (n % 100 == 0) {
                std::cout << "time: " << t <<  ", planet: " << bodies[1] << std::endl;
            }

            update_display(graphics_disp, bodies); //planet1, planet2, planet1_positions, planet2_positions);
        }
//        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }


}
