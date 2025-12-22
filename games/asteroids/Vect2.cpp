//
// Created by scott on 11/29/25.
//

#include <iostream>
#include "Vect2.hpp"

std::ostream& operator<<(std::ostream& os, const Vect2& v) {
    os << "[" << v.x << ", " << v.y << "]";
    return os;
}