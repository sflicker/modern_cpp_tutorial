//
// Created by scott on 11/1/25.
//

#include "graphics_display.hpp"

GraphicsDisplay::GraphicsDisplay(int width, int height, const std::string &title) : mWindow(sf::VideoMode(width, height), title) {
    if (!mFont.loadFromFile("/home/scott/FreeSans.ttf")) {
        exit(1);
    }
    mWindow.setFramerateLimit(60);
}

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

void GraphicsDisplay::drawLines(const sf::VertexArray & lines) {
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

void GraphicsDisplay::setView(sf::View & view) {
    mWindow.setView(view);
}

void GraphicsDisplay::drawText(float x, float y, const std::string text) {

    sf::View oldview = mWindow.getView();

    mWindow.setView(mWindow.getDefaultView());

    sf::Text textObj;
    textObj.setFont(mFont);
    textObj.setString(text);
    textObj.setPosition(x, y);
    textObj.setFillColor(sf::Color::White);
    textObj.setCharacterSize(20);
    mWindow.draw(textObj);

    mWindow.setView(oldview);
}

void GraphicsDisplay::drawCrosshairAtWorld(float x, float y) {

    sf::View worldView = mWindow.getView();
    sf::Vector2i px = mWindow.mapCoordsToPixel(sf::Vector2f(x, y), worldView);

    mWindow.setView(mWindow.getDefaultView());

    float length = 14.f;
    float thickness = 2.f;

    sf::Vector2f center2f((float)px.x, (float)px.y);

    sf::RectangleShape horiz({length * 2, thickness});
    horiz.setOrigin(length, thickness/2);
    horiz.setPosition(center2f);
    horiz.setFillColor(sf::Color::Red);

    sf::RectangleShape vert({thickness, length * 2});
    vert.setOrigin(thickness/2, length);
    vert.setPosition(center2f);
    vert.setFillColor(sf::Color::Red);

    mWindow.draw(horiz);
    mWindow.draw(vert);

    mWindow.setView(worldView);

}

bool GraphicsDisplay::pollEvent(sf::Event & event) {
    mWindow.pollEvent(event);
}

void GraphicsDisplay::close() { mWindow.close(); }
