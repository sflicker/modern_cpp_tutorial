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
        void display();
        bool isOpen();
        bool pollEvents();

    private:
        sf::RenderWindow mWindow;

};

#endif //MODERN_TUTOR_GRAPHICS_DISPLAY_HPP