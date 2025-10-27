//
// Created by scott on 10/26/25.
//
#include <iostream>
#include <memory>

struct Node {
    std::string name;
    std::weak_ptr<Node> parent;
    std::shared_ptr<Node> child;

    Node(std::string n) : name(std::move(n)) {
        std::cout << "Node " << name << " created\n";
    }
    ~Node() {
        std::cout << "Node " << name << " destroyed\n";
    }
};

int main() {
    auto parent = std::make_shared<Node>(std::string("parent"));
    auto child = std::make_shared<Node>(std::string("child"));

    parent->child = child;
    child->parent = parent;
}