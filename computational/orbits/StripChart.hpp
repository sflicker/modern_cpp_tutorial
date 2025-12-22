//
// Created by scott on 12/21/25.
//

#ifndef MODERN_TUTOR_STRIPCHART_HPP
#define MODERN_TUTOR_STRIPCHART_HPP
#include <cstddef>
#include <deque>
#include <SFML/Graphics/VertexArray.hpp>

struct StripChart {
    std::deque<float> y;
    size_t maxN = 600;
    float yMin = -180, yMax = 180;

    void push(float v) {
        y.push_back(v);
        if (y.size() > maxN) y.pop_front();
    }

    void draw(sf::RenderTarget& rt, sf::FloatRect rect) {
        if (y.size() < 2) return;

        sf::VertexArray va(sf::LineStrip, y.size());
        for (size_t i=0;i<y.size();++i) {
            float x = rect.left + rect.width * (float(i) / float(maxN-1));
            float t = (y[i] -yMin) / (yMax - yMin);
            float yy = rect.top + rect.height * (1.0f - t);
            va[i].position = { x, yy};
        }
        rt.draw(va);
    }
};

#endif //MODERN_TUTOR_STRIPCHART_HPP