//
// Created by scott on 12/14/25.
//

#pragma once
#include <SFML/Graphics/Color.hpp>

namespace Colors {

    // Stars
    inline const sf::Color Star{255, 230, 160};

    // Inner planets
    inline const sf::Color Mercury{140, 140, 140};
    inline const sf::Color Venus  {220, 200, 140};
    inline const sf::Color Earth  { 80, 130, 200};
    inline const sf::Color Mars   {190,  90,  60};

    // Gas / ice giants
    inline const sf::Color Jupiter {210, 170, 120};
    inline const sf::Color Saturn  {220, 200, 150};
    inline const sf::Color Uranus  {140, 210, 210};
    inline const sf::Color Neptune { 70,  90, 180};

    // Small bodies
    inline const sf::Color Asteroid{120, 120, 120};
    inline const sf::Color Comet   {220, 240, 255};
}