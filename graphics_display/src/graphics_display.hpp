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
        void display();
        bool isOpen();
        bool pollEvents();

        void setView(sf::View & view);

    private:
        sf::RenderWindow mWindow;

};

#endif //MODERN_TUTOR_GRAPHICS_DISPLAY_HPP