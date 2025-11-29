//
// Created by scott on 11/29/25.
//

#include "Trail.hpp"

#include <cstddef>

void Trail::addTrailPoint(const Vect2 & pos, sf::Color color) {

    double dr = last_point.dist(pos);

    if (dr > min_delta) {
        if (_trail.getVertexCount() >= max_points) {
            const std::size_t n = _trail.getVertexCount();

            // shift
            for (std::size_t i = 1; i < n; ++i) {
                _trail[i - 1] = _trail[i];
            }

            _trail.resize(n - 1);
        }
        _trail.append(sf::Vector2f(pos.x, pos.y));
        last_point = pos;
    }
}

int Trail::getVertexCount() { return _trail.getVertexCount(); }

sf::VertexArray & Trail::getTrail() { return _trail; }