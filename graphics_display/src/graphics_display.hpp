//
// Created by scott on 11/1/25.
//

#ifndef MODERN_TUTOR_GRAPHICS_DISPLAY_HPP
#define MODERN_TUTOR_GRAPHICS_DISPLAY_HPP

#include <SFML/Graphics.hpp>

class GraphicsDisplay {
    public:
        GraphicsDisplay(int width = 800, int height = 600, const std::string & title = "Display");

        void clear();
        void drawCircle(float x, float y, float radius, sf::Color color);
        void drawRect(float x, float y, float width, float height, sf::Color color);
        void drawLine(float x1, float y1, float x2, float y2, float y, sf::Color color);
        void drawLines(const sf::VertexArray & lines);
        void drawText(float x, float y, const std::string text);
        void drawCrosshairAtWorld(float x, float y);
        void display();
        bool isOpen();
        bool pollEvents();
        bool pollEvent(sf::Event & event);
        void close();
        sf::Vector2i mapCoordsToPixel(float x, float y);

        void setView(sf::View & view);

    private:
        sf::RenderWindow mWindow;
        sf::Font mFont;
};

#endif //MODERN_TUTOR_GRAPHICS_DISPLAY_HPP