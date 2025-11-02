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
float x_min = 0;
float y_min = 0;
float x_max = 640;
float y_max = 480;

struct Circle {
    float x, y;
    float radius;
    sf::Color color;
};

struct Node {
    std::unique_ptr<Circle> shape;
    std::unique_ptr<Node> next;
    Node * prev = nullptr;        // raw pointer for circular link back
};

struct Scene {
    std::unique_ptr<Node> shapes;
};

bool overlaps(const Node &a, const Node &b) {
    float dx = a.shape->x - b.shape->x;
    float dy = a.shape->y - b.shape->y;
    float distanceSquared = dx*dx + dy*dy;
    float radiusSum = a.shape->radius + b.shape->radius;
    return distanceSquared < radiusSum*radiusSum;
}

void update_display(GraphicsDisplay & display, Scene * scene) {
    display.clear();

    Node * current = scene->shapes.get();

    while (current != nullptr) {
        std::cout << "Rendering shape at " << current->shape->x << ", " << current->shape->y << std::endl;
        display.drawCircle(current->shape->x, current->shape->y, current->shape->radius, current->shape->color);
        current = current->next.get();
    }

    display.display();
}

float getRandom(std::mt19937 & generator, std::uniform_real_distribution<float> distribution) {
    return distribution(generator);
}

void move_shapes(std::mt19937 & generator, std::uniform_real_distribution<float> distribution, Scene * scene) {
    Node * current = scene->shapes.get();

    while (current != nullptr) {
        float new_x = current->shape->x + getRandom(generator, distribution);
        if (new_x > x_min && new_x < x_max) {
            current->shape->x = new_x;
        }

        float new_y = current->shape->y + getRandom(generator, distribution);
        if (new_y > y_min && new_y < y_max) {
            current->shape->y = new_y;
        }

        current = current->next.get();
    }

}

float combine_radius(Node * lhs, Node * rhs) {
    return sqrt(lhs->shape->radius*lhs->shape->radius + rhs->shape->radius*rhs->shape->radius);
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
        for (Node * current = scene->shapes.get(); current != nullptr; current = current->next.get()) {
            current->shape->color = default_color;
        }
        Node * lhs = scene->shapes.get();
        while (lhs != nullptr) {
            Node * rhs = lhs->next.get();
            while (rhs != nullptr) {
                if (overlaps(*lhs, *rhs)) {
                    lhs->shape->color = overlap_color;
                    rhs->shape->color = overlap_color;
                }
                rhs = rhs->next.get();
            }
            lhs = lhs->next.get();
        }
}

void update_scene(std::mt19937 & generator, std::uniform_real_distribution<float> distribution, Scene * scene) {
    move_shapes(generator, distribution, scene);
    color_overlaps(scene);
}

int main() {
    GraphicsDisplay g(x_max, y_max, "Demo");
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution dist(-1.0f, 1.0f);

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

    Node * tail = nullptr;
    scene->shapes.get();

//    x += INC_X;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::unique_ptr<Node> new_node = std::make_unique<Node>();
            new_node->shape = std::make_unique<Circle>();
            new_node->shape->color = default_color;
            new_node->shape->x = x;
            new_node->shape->y = y;
            new_node->shape->radius = radius;
            if (scene->shapes.get() == nullptr) {
                scene->shapes = std::move(new_node);
                tail = scene->shapes.get();
            } else {
                tail->next = std::move(new_node);
                tail = tail->next.get();
            }

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
        move_shapes(gen, dist, scene.get());
    }
}