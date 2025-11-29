//
// Created by scott on 11/1/25.
//

#include "graphics_display.hpp"

GraphicsDisplay::GraphicsDisplay(int width, int height, const std::string &title) : mWindow(sf::VideoMode(width, height), title) {}

void GraphicsDisplay::clear() { mWindow.clear(); }

void GraphicsDisplay::drawCircle(float x, float y, float radius, sf::Color color) {
    sf::CircleShape circle(radius);
    circle.setFillColor(color);
    circle.setPosition(x, y);
    mWindow.draw(circle);
}

void GraphicsDisplay::drawRect(float x, float y, float w, float h, sf::Color color) {
    sf::RectangleShape rect({w, h});
    rect.setFillColor(color);
    rect.setPosition(x, y);
    mWindow.draw(rect);
}

void GraphicsDisplay::drawLine(float x1, float y1, float x2, float y2, float width, sf::Color color) {
    sf::Vector2f p1(x1, y1);
    sf::Vector2f p2(x2, y2);
    sf::Vertex line[] = {{p1, color}, {p2, color}};
    mWindow.draw(line, width, sf::Lines);
}

void GraphicsDisplay::drawLines(sf::VertexArray & lines) {
    mWindow.draw(lines);
}

void GraphicsDisplay::display() { mWindow.display(); }

bool GraphicsDisplay::isOpen() { return mWindow.isOpen(); }

bool GraphicsDisplay::pollEvents() {
    sf::Event e;
    while (mWindow.pollEvent(e)) {
        if (e.type == sf::Event::Closed)
            mWindow.close();
    }
    return mWindow.isOpen();
}
