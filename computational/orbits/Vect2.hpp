//
// Created by scott on 11/29/25.
//

#ifndef MODERN_TUTOR_VECT2_HPP
#define MODERN_TUTOR_VECT2_HPP

#include <cmath>

struct Vect2 {
    double x = 0.f;
    double y = 0.f;

    Vect2() = default;
    Vect2(double x, double y) : x(x), y(y) {};

    // unary operator
    Vect2 operator-() const {
        return Vect2(-x, -y);
    }

    // binary operators
    Vect2 operator-(const Vect2 & rhs) const {
        return Vect2(x - rhs.x, y - rhs.y);
    }

    Vect2 operator+(const Vect2 & rhs) const {
        return Vect2(x + rhs.x, y + rhs.y);
    }

    Vect2 operator*(double scalar) const {
        return Vect2(x * scalar, y * scalar);
    }

    friend Vect2 operator*(double s, const Vect2& v) {
        return Vect2(v.x * s, v.y * s);
    }

    Vect2 operator/(double scalar) const {
        return Vect2(x / scalar, y / scalar);
    }

    Vect2 & operator+=(const Vect2 & vect2) {
        x += vect2.x;
        y += vect2.y;
        return *this;
    }

    Vect2 & operator-=(const Vect2 & vect2) {
        x -= vect2.x;
        y -= vect2.y;
        return *this;
    }

    Vect2 & operator*=(double scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vect2 & operator/=(double scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    double dist(const Vect2 & v) const {
        double dx = x - v.x;
        double dy = y - v.y;
        return sqrt(dx*dx + dy*dy);
    }
};

#endif //MODERN_TUTOR_VECT2_HPP