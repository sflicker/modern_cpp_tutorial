#include <iostream>
#include <memory>

struct MyObject {
    MyObject() { std::cout << "Created\n"; }
    ~MyObject() { std::cout << "Destroyed\n"; }
};

int main() {
    std::unique_ptr<MyObject> ptr = std::make_unique<MyObject>();
}

