//
// Created by scott on 11/1/25.
//

#include "../../graphics_display/src/graphics_display.hpp"
#include <thread>
#include <chrono>

int main() {
    GraphicsDisplay g(640, 480, "Demo");

    while (g.isOpen()) {
        g.pollEvents();
        g.clear();
        g.drawCircle(100, 100, 50, sf::Color::Red);
        g.drawRect(200, 200, 100, 50, sf::Color::Blue);
        g.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}