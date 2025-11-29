//
// Created by scott on 11/27/25.
//

#ifndef MODERN_TUTOR_ORBIT_MAIN_HPP
#define MODERN_TUTOR_ORBIT_MAIN_HPP

struct Vect2 {
    double x;
    double y;
    Vect2(double x, double y);
    Vect2();
};

struct Body {
    double mass;
    Vect2 position;
    Vect2 velocity;
    Vect2 force;
    Body(double mass, Vect2 &position, Vect2 &velocity, Vect2 &force);

    Body(double x, Vect2 vect2, Vect2 vect3, Vect2 vect4);
};

class OrbitMain {
};

double calculate_distance(Body &a, Body &b);
Vect2 calculate_forces_on_body(Body &a, Body &b);

#endif //MODERN_TUTOR_ORBIT_MAIN_HPP