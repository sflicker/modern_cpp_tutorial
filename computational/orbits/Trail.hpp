//
// Created by scott on 11/29/25.
//

#ifndef MODERN_TUTOR_TRAIL_HPP
#define MODERN_TUTOR_TRAIL_HPP

#include "Vect2.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>

class Trail {
    sf::VertexArray _trail;
    Vect2 last_point;
    double min_delta = 1;
    std::size_t max_points = 500;
    sf::Color color;

public:
    void addTrailPoint(const Vect2 &position, sf::Color color = sf::Color::White);
    int getVertexCount() const;
    const sf::VertexArray &getTrail() const;
};

#endif //MODERN_TUTOR_TRAIL_HPP