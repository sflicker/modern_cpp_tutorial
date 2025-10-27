#include <iostream>
#include <memory>
//
// Created by scott on 10/26/25.
//
struct Node {
    std::shared_ptr<Node> child;
    Node() { std::cout << "Created\n"; }
    ~Node() { std::cout << "Destroyed\n"; }
};

int main() {
    auto a = std::make_shared<Node>();
    auto b = std::make_shared<Node>();
    a->child = b;
    b->child = a;
    // destroyed never gets called because both nodes have a ref to the other so
    // ref counts never go to zero
}