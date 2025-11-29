//
// Created by scott on 11/27/25.
//

#include "orbit_main.hpp"

#include <cmath>
#include <iostream>
#include <bits/this_thread_sleep.h>
#include <SFML/Graphics/Color.hpp>

#include "../../graphics_display/src/graphics_display.hpp"

sf::Color planet_color = sf::Color::Blue;
sf::Color star_color = sf::Color::Yellow;

int star_radius = 20;
int planet_radius = 6;

double G = 1;

int XMAX = 800;
int YMAX = 600;

Vect2::Vect2(double x, double y) : x(x), y(y) {}
Vect2::Vect2() : x(0), y(0) {}

Body::Body(double mass, Vect2 position, Vect2 velocity, Vect2 force) : mass(mass), position(position), velocity(velocity), force(force) {};

std::ostream& operator<<(std::ostream& os, const Vect2& v) {
    os << "[" << v.x << ", " << v.y << "]";
    return os;
}

std::ostream& operator<<(std::ostream & os, const Body &b) {
    os << "mass: " << b.mass << ", position: " << b.position << ", velocity: " << b.velocity << ", force: " << b.force;
    return os;
}

void update_display(GraphicsDisplay & display,
    Body& body1,
    Body& body2,
    sf::VertexArray & planet1_positions,
    sf::VertexArray & planet2_positions) {
    if (display.isOpen()) {
        display.pollEvents();

        display.clear();

        display.drawCircle(XMAX/2-star_radius, YMAX/2-star_radius, star_radius, star_color);
        Vect2 p1, p2;
        if (planet1_positions.getVertexCount() > 2) {
//            sf::VertexArray lines(sf::Lines);
//             for (int i = 0; i < planet1_positions.size(); i++) {
//                 p1 = planet1_positions[i];
//                 lines.append(sf::Vertex(sf::Vertex(sf::Vector2f(p1.x + XMAX/2, p1.y + YMAX/2), planet_color)));
// //                display.drawLine(p1.x + XMAX/2, p1.y + YMAX/2, p2.x + XMAX/2, p2.y + YMAX/2, 4, planet_color);
// //                p1 = p2;
//             }
            display.drawLines(planet1_positions);
        }
        display.drawCircle(
            body1.position.x + XMAX/2 - planet_radius,
            body1.position.y + YMAX/2 - planet_radius,
            planet_radius,
            planet_color);

        if (planet2_positions.getVertexCount() > 2) {
            // p1 = planet2_positions[0];
            // for (int i = 1; i < planet2_positions.size(); i++) {
            //     p2 = planet2_positions[i];
            //     display.drawLine(p1.x + XMAX/2, p1.y + YMAX/2, p2.x + XMAX/2, p2.y + YMAX/2, 4, planet_color);
            //     p1 = p2;
            // }
            display.drawLines(planet2_positions);
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

    double R = 100.0;
    double M = 100.0;
    double m = 1.0;
    double dt = 0.1;
    double t = 0.0;
    double nSteps = 100000.0;

    Body star(M, Vect2(0,0), Vect2(0,0), Vect2(0,0));
    Body planet1(m, Vect2(R, 0), Vect2(0, -sqrt(G*M/R) * 1.1) , Vect2(0,0));
    Body planet2(m, Vect2(R*2, 0), Vect2(0, -sqrt(G*M/(R*2)) * 0.9), Vect2(0,0));

    sf::VertexArray planet1_positions;
    sf::VertexArray planet2_positions;

    planet1_positions.append(sf::Vertex(sf::Vertex(sf::Vector2f(planet1.position.x + XMAX/2, planet1.position.y + YMAX/2), planet_color)));
    planet2_positions.append(sf::Vertex(sf::Vertex(sf::Vector2f(planet2.position.x + XMAX/2, planet2.position.y + YMAX/2), planet_color)));

    std::cout << planet1 << ", " << planet2 << std::endl;
    update_display(graphics_disp, planet1, planet2, planet1_positions, planet2_positions);
    for (int n = 0; n < nSteps; n++) {
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

        if (n % 10 == 0) {
            planet1_positions.append(sf::Vertex(sf::Vertex(sf::Vector2f(planet1.position.x + XMAX/2, planet1.position.y + YMAX/2), planet_color)));
        }

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

        if (n % 10 == 0) {
            planet2_positions.append(sf::Vertex(sf::Vertex(sf::Vector2f(planet2.position.x + XMAX/2, planet2.position.y + YMAX/2), planet_color)));
        }

        t += dt;

        if (n % 100 == 0) {
            std::cout << "time: " << t <<  ", planet: " << planet1 << std::endl;
        }


        update_display(graphics_disp, planet1, planet2, planet1_positions, planet2_positions);

//        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }


}
