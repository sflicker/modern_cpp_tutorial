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
double x_max = 1200;
double y_max = 1200;
int id_seq = 0;
int N = 40;
double step_size = 5.0;;
double radius = 10;
double T = 0.5;
double kB = 1.0;
std::uniform_real_distribution<double> uniform01(0.0, 1.0);
std::mt19937 gen(std::random_device{}());
double cutoff = 100.0;
double max_displacement = 0.0;

struct Circle {
    int id;
    double x;
    double y;
    double x_ref;
    double y_ref;
    double radius;
    sf::Color color;

    Circle(double x_, double y_, double radius_, sf::Color color_) : id(id_seq++), x(x_), y(y_), x_ref(x_), y_ref(y_), radius(radius_), color(color_) {}

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

std::tuple<double, double> apply_minimum_image_convection(double _dx, double _dy) {
    double x_range = x_max - x_min;
    double y_range = y_max - y_min;

    double dx = _dx - x_range * std::round(_dx/x_range);
    double dy = _dx - y_range * std::round(_dy/y_range);

    return {dx, dy};
}

std::tuple<double, double> correct_boundary_conditions(double x, double  y) {
    double x_range = x_max - x_min;
    double y_range = y_max - y_min;

    x = x_min + std::fmod((x - x_min + x_range), x_range);
    y = y_min + std::fmod((y - y_min + y_range), y_range);

    return {x, y};
    // if (new_x > x_max) {
    //     new_x -= x_range;
    // }
    // if (new_x < x_min) {
    //     new_x += x_range;
    // }
    // if (new_y > y_max) {
    //     new_y -= y_range;
    // }
    // if (new_y < y_min) {
    //     new_y += y_range;
    // }
}

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
        //std::cout << "Rendering shape at " << circle.x << ", " << circle.y << std::endl;
        display.drawCircle(circle.x, circle.y, circle.radius, circle.color);
    }

    display.display();
}

double getRandom(std::mt19937 & generator, std::uniform_real_distribution<double> distribution) {
    return distribution(generator);
}

Circle & getRandomCircle(std::mt19937 & generator, std::uniform_real_distribution<double> distribution, Scene * scene) {
    int num_elements = scene->shapes.size();
    double random = getRandom(generator, distribution);
    int particle_num = static_cast<int>((random + 1.0)*num_elements / 2.0);

    if (particle_num >= num_elements) {
        particle_num = num_elements - 1;
    }
    if (particle_num < 0) {
        particle_num = 0;
    }
    return scene->shapes[particle_num];
}

double lj_potential(double r, double epsilion = 1.0, double sigma = 17.8) {
    double sr6 = std::pow(sigma/r, 6);
    return 4.0 * epsilion * (sr6 * sr6 - sr6);
}

double calculate_pair_wise_with_particle(const Circle & shape,
    double x, double y, Scene * scene, std::vector<std::vector<int>> & neighbor_list) {
    double energy = 0.0;
    int count = 0;
    for (int other_id : neighbor_list[shape.id]) {
        if (other_id == shape.id) continue;
        double _dx = x - scene->shapes[other_id].x;
        double _dy = y - scene->shapes[other_id].y;
        auto [dx, dy] = correct_boundary_conditions(_dx, _dy);
        double r2 = dx*dx + dy*dy;
        if (r2 == 0) continue;  // avoid divide by zero

        double r = std::sqrt(r2);
        if (r < cutoff) {
            count++;
            energy += lj_potential(r);
        }
    }
    std::cout << "Particles within the cutoff: " << count << std::endl;
    return energy;
}

double distance_moved(Circle & shape) {
    double _dx = shape.x - shape.x_ref;
    double _dy = shape.y - shape.y_ref;
    auto [dx, dy] = apply_minimum_image_convection(_dx, _dy);
//    correct_boundary_conditions(dx, dy);
    double r = std::sqrt(dx*dx + dy*dy);
    return r;
}

void move_shape(std::mt19937 & generator,
    std::uniform_real_distribution<double> distribution,
    Scene * scene,
    std::vector<std::vector<int>> & neighbor_list) {
//    Node * current = scene->shapes.get();

    Circle & shape = getRandomCircle(generator, distribution, scene);

    double initial_energy = calculate_pair_wise_with_particle(shape, shape.x, shape.y, scene, neighbor_list);
    double x = shape.x, y = shape.y;

    double _dx = step_size* getRandom(generator, distribution);
    double _dy = step_size * getRandom(generator, distribution);

    double _new_x = x + _dx;
    double _new_y = y + _dy;
//    double _dx = x - shape.x;
//    double _dy = y - shape.y;

    auto [new_x, new_y] = correct_boundary_conditions(_new_x, _new_y);

    // double new_x = x + dx;
    // double new_y = y + dy;

    double after_energy = calculate_pair_wise_with_particle(shape, new_x, new_y, scene, neighbor_list);

    double delta_e = after_energy - initial_energy;
    // check energies, only do move if allowed
    if (delta_e <= 0.0) {
        std::cout << "Moving shape (energy reduction) " << std::endl;
        shape.x = new_x;
        shape.y = new_y;
        max_displacement = std::max(max_displacement, distance_moved(shape));
    } else {
        double p_accept = std::exp(-delta_e / (kB * T));
        double r = uniform01(gen);
        if (r < p_accept) {
            std::cout << "Moving shape (energy increase but below probability" << std::endl;
            shape.x = new_x;
            shape.y = new_y;
            max_displacement = std::max(max_displacement, distance_moved(shape));
        }
        else {
            std::cout << "Did not move shape" << std::endl;
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

void update_scene(std::mt19937 & generator,
    std::uniform_real_distribution<double> distribution,
    Scene * scene,
    std::vector<std::vector<int>> & neighbor_list) {
    move_shape(generator, distribution, scene, neighbor_list);
    color_overlaps(scene);
}

void populate_scene(int INC_X, int INC_Y, float x, float y, float radius, std::unique_ptr<Scene> &scene) {
    scene = std::make_unique<Scene>();
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
}

void build_neighbor_list(Scene * scene, std::vector<std::vector<int>> &neighbor_list, double cutoff, double skin) {
    double cutoff_skin = cutoff + skin;
    double cutoff2 = cutoff_skin * cutoff_skin;
    size_t N = scene->shapes.size();
    for (size_t i = 0; i < N; ++i) {
        neighbor_list[i].clear();
        for (size_t j = 0; j < N; ++j) {
            if (i == j) continue;
            double _dx = scene->shapes[i].x - scene->shapes[j].x;
            double _dy = scene->shapes[i].y - scene->shapes[j].y;
            auto [dx, dy] = correct_boundary_conditions(_dx, _dy);
            double r2 = dx * dx + dy * dy;
            if (r2 < cutoff2) {
                neighbor_list[i].push_back(j);
            }
        }
    }
}

int main() {
    GraphicsDisplay g(x_max, y_max, "Demo");
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution dist(-1.0, 1.0);

    int INC_X = 30;
    int INC_Y = 30;

    double x = 20, y = 20;

    std::cout << "Making shapes" << std::endl;
    std::unique_ptr<Scene> scene;
    populate_scene(INC_X, INC_Y, x, y, radius, scene);

    std::vector<std::vector<int>> neighbor_list(scene->shapes.size());
    build_neighbor_list(scene.get(), neighbor_list, cutoff, cutoff/2);
    // tail->next = std::move(head);
    // tail->next->prev = tail;


    while (g.isOpen()) {
        update_display(g, scene.get());

        g.pollEvents();
//        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        update_scene(gen, dist, scene.get(), neighbor_list);
//        move_shapes(gen, dist, scene.get());

        if (max_displacement > cutoff/2) {
            std::cout << "max_displacement: " << max_displacement << ". reset neighbor list and reference positions" << std::endl;
            build_neighbor_list(scene.get(), neighbor_list, cutoff, cutoff/2);
            size_t N = scene->shapes.size();
            for (size_t i = 0; i < N; ++i) {
                scene->shapes[i].x_ref = scene->shapes[i].x;
                scene->shapes[i].y_ref = scene->shapes[i].y;
            }
            max_displacement = 0.0;
        }

        std::cout << "max_displacement is " << max_displacement << std::endl;
    }
}
