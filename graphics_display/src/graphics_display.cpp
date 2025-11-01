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
