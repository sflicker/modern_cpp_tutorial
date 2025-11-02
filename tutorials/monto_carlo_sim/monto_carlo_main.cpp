//
// Created by scott on 11/1/25.
//

#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <thread>
#include <vector>
#include <SFML/Graphics/Color.hpp>

#include "../../graphics_display/src/graphics_display.hpp"

sf::Color overlap_color = sf::Color::Red;
sf::Color default_color = sf::Color::Blue;
double x_min = 0;
double y_min = 0;
double x_max = 640;
double y_max = 480;
int id_seq = 0;

struct Circle {
    int id;
    double x;
    double y;
    double radius;
    sf::Color color;

    Circle(double x_, double y_, double radius_, sf::Color color_) : id(++id_seq), x(x_), y(y_), radius(radius_), color(color_) {}

    void setColor(sf::Color c) { color = c; }
};

// struct Node {
//     std::unique_ptr<Shape> shape;
//     std::unique_ptr<Node> next;
//     Node * prev = nullptr;        // raw pointer for circular link back
// };

struct Scene {
    std::vector<Circle> shapes;
};

bool overlaps(const Circle &a, const Circle &b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    double distanceSquared = dx*dx + dy*dy;
    double radiusSum = a.radius + b.radius;
    return distanceSquared < radiusSum*radiusSum;
}

void update_display(GraphicsDisplay & display, Scene * scene) {
    display.clear();

    //Node * current = scene->shapes.get();

    for (const auto& circle : scene->shapes) {
        std::cout << "Rendering shape at " << circle.x << ", " << circle.y << std::endl;
        display.drawCircle(circle.x, circle.y, circle.radius, circle.color);
    }

    display.display();
}

double getRandom(std::mt19937 & generator, std::uniform_real_distribution<double> distribution) {
    return distribution(generator);
}

void move_shapes(std::mt19937 & generator, std::uniform_real_distribution<double> distribution, Scene * scene) {
//    Node * current = scene->shapes.get();

    for (auto& shape : scene->shapes) {
        float new_x = shape.x + getRandom(generator, distribution);
        if (new_x > x_min && new_x < x_max) {
            shape.x = new_x;
        }

        float new_y = shape.y + getRandom(generator, distribution);
        if (new_y > y_min && new_y < y_max) {
            shape.y = new_y;
        }

    }

}

float combine_radius(const Circle * lhs, const Circle * rhs) {
    return sqrt(lhs->radius * lhs->radius + rhs->radius * rhs->radius);
}

// void remove_from_scene(Scene * scene, Node * node) {
//     if (scene->shapes.get() == node) {
//         Node * new_head = node->next.get();
//         scene->shapes.reset();
//         scene->shapes = new_head;
//     }
// }

// void remove_smallest(Scene * scene, Node * lhs, Node * rhs) {
//     Node * smallest = rhs;
//     Node * largest = lhs;
//     if (smallest->shape->radius > largest->shape->radius) {
//         Node * temp = rhs;
//         rhs = lhs;
//         lhs = temp;
//     }
//     largest->shape->radius = combine_radius(largest, smallest);
//     remove_from_scene(scene, smallest);
// }

void color_overlaps(Scene * scene) {
        // reset colors
        for (auto & circle : scene->shapes) {
            circle.color = default_color;
        }

        // change color if overlap
        for (auto &lhs : scene->shapes) {
            for (auto &rhs : scene->shapes) {
                if (lhs.id != rhs.id && overlaps(lhs, rhs)) {
                    lhs.color = overlap_color;
                    rhs.color = overlap_color;
                }
            }
        }
        // Node * lhs = scene->shapes.get();
        // while (lhs != nullptr) {
        //     Node * rhs = lhs->next.get();
        //     while (rhs != nullptr) {
        //         if (overlaps(*lhs, *rhs)) {
        //             lhs->shape->color = overlap_color;
        //             rhs->shape->color = overlap_color;
        //         }
        //         rhs = rhs->next.get();
        //     }
        //     lhs = lhs->next.get();
        // }
}

void update_scene(std::mt19937 & generator, std::uniform_real_distribution<double> distribution, Scene * scene) {
    move_shapes(generator, distribution, scene);
    color_overlaps(scene);
}

int main() {
    GraphicsDisplay g(x_max, y_max, "Demo");
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution dist(-1.0, 1.0);

    int N = 10;
    int INC_X = 40;
    int INC_Y = 40;

    float x = 20, y = 20, radius = 10;

    std::cout << "Making shapes" << std::endl;
    std::unique_ptr<Scene> scene = std::make_unique<Scene>();
    //scene->shapes = std::make_unique<Node>();
    // scene->shapes->shape = std::make_unique<Shape>();
    // scene->shapes->shape->color = default_color;
    // scene->shapes->shape->x = x;
    // scene->shapes->shape->y = y;
    // scene->shapes->shape->radius = radius;

    // Node * tail = nullptr;
    // scene->shapes.get();

//    x += INC_X;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            Circle circle(x,y,radius, default_color);
            //new_node->shape = std::make_unique<Circle>();
            // cir->color = default_color;
            // cir->x = x;
            // cir->y = y;
            // cir->radius = radius;
            scene->shapes.push_back(circle);
            //scene->shapes.push_back((std::move(cir));
            // if (scene->shapes.get() == nullptr) {
            //     scene->shapes = std::move(new_node);
            //     tail = scene->shapes.get();
            // } else {
            //     tail->next = std::move(new_node);
            //     tail = tail->next.get();
            // }

            x += INC_X;
        }
        y += INC_Y;
        x = 20;
    }

    // tail->next = std::move(head);
    // tail->next->prev = tail;


    while (g.isOpen()) {
        update_display(g, scene.get());

        g.pollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        update_scene(gen, dist, scene.get());
//        move_shapes(gen, dist, scene.get());
    }
}