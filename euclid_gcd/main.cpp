#include <iostream>
//
// Created by scott on 10/25/25.
//
int gcd(int u, int v) {
    int temp;
    while (u > 0) {
        if (u < v) {
            temp = u;
            u = v;
            v = temp;
        }
        u = u - v;
    }
    return v;
}

int main() {
    int x, y;
    std::cout << "Enter First Number: ";
    std::cin >> x;
    std::cout << "Enter Second Number: ";
    std::cin >> y;

//    while (std::cin >> x && std::cin >> y) {
        if (x > 0 && y > 0) {
            std::cout << x << ' ' << y << " => " << gcd(x, y) << std::endl;
        }
//    }
}