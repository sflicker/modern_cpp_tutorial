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
int YMAX = 600;

Body::Body(double mass, Vect2 position, Vect2 velocity, Vect2 force) : mass(mass), position(position), velocity(velocity), force(force) {};

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
        }
    }
}

void update_display(GraphicsDisplay & display,
    Body& body1,
    Body& body2,
    Trail & planet1_positions,
    Trail & planet2_positions) {
    if (display.isOpen()) {
        display.pollEvents();

        display.clear();

        display.drawCircle(XMAX/2-star_radius, YMAX/2-star_radius, star_radius, star_color);
        Vect2 p1, p2;
        if (planet1_positions.getVertexCount() > 2) {
            display.drawLines(planet1_positions.getTrail());
        }
        display.drawCircle(
            body1.position.x + XMAX/2 - planet_radius,
            body1.position.y + YMAX/2 - planet_radius,
            planet_radius,
            planet_color);

        if (planet2_positions.getVertexCount() > 2) {
            display.drawLines(planet2_positions.getTrail());
        }

        display.drawCircle(
            body2.position.x + XMAX/2 - planet_radius,
            body2.position.y + YMAX/2 - planet_radius,
            planet_radius,
            planet_color);

        display.display();
    }
}

int main() {
    std::cout << "Starting Orbits!\n";

    GraphicsDisplay graphics_disp(XMAX, YMAX, "Orbits");

    std::add_const<Body> bodies;


    double R = 100.0;
    double M = 100.0;
    double m = 1.0;
    double dt = 0.1;
    double t = 0.0;
    int nSteps = 100000;
    int launchStep = 1000;



    Body star(M, Vect2(0,0), Vect2(0,0), Vect2(0,0));
    Body planet1(m, Vect2(R, 0), Vect2(0, -sqrt(G*M/R) * 1.01) , Vect2(0,0));
    Body planet2(m, Vect2(R*2, 0), Vect2(0, -sqrt(G*M/(R*2))), Vect2(0,0));

    Trail planet1_positions;
    Trail planet2_positions;

    planet1_positions.addTrailPoint( Vect2(planet1.position.x + XMAX/2, planet1.position.y + YMAX/2), planet_color);
    planet2_positions.addTrailPoint(Vect2(planet2.position.x + XMAX/2, planet2.position.y + YMAX/2), planet_color);

    std::cout << planet1 << ", " << planet2 << std::endl;
    update_display(graphics_disp, planet1, planet2, planet1_positions, planet2_positions);
    for (int n = 0; n < nSteps; n++) {

        if (n == launchStep) {

        }

        // planet1
        double r2 = planet1.position.x*planet1.position.x + planet1.position.y*planet1.position.y;
        double r = sqrt(r2);
        double inv_r3 = 1.0 / (r2 * r);
        planet1.force.x = -G*M * inv_r3 * planet1.position.x;
        planet1.force.y = -G*M * inv_r3 * planet1.position.y;

        planet1.velocity.x += planet1.force.x * dt;
        planet1.velocity.y += planet1.force.y * dt;

        planet1.position.x += planet1.velocity.x * dt;
        planet1.position.y += planet1.velocity.y * dt;
        planet1_positions.addTrailPoint(Vect2(planet1.position.x + XMAX/2, planet1.position.y + YMAX/2), planet_color);

        // planet2
        r2 = planet2.position.x*planet2.position.x + planet2.position.y*planet2.position.y;
        r = sqrt(r2);
        inv_r3 = 1.0 / (r2 * r);
        planet2.force.x = -G*M * inv_r3 * planet2.position.x;
        planet2.force.y = -G*M * inv_r3 * planet2.position.y;

        planet2.velocity.x += planet2.force.x * dt;
        planet2.velocity.y += planet2.force.y * dt;

        planet2.position.x += planet2.velocity.x * dt;
        planet2.position.y += planet2.velocity.y * dt;

        planet2_positions.addTrailPoint(Vect2(planet2.position.x + XMAX/2, planet2.position.y + YMAX/2), planet_color);

        t += dt;

        if (n % 100 == 0) {
            std::cout << "time: " << t <<  ", planet: " << planet1 << std::endl;
        }

        update_display(graphics_disp, planet1, planet2, planet1_positions, planet2_positions);

//        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }


}
