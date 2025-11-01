#include <iostream>
#include <memory>

struct MyObject {
    MyObject() { std::cout << "Created\n"; }
    ~MyObject() { std::cout << "Destroyed\n"; }
};

std::unique_ptr<MyObject> make_unique() {
    std::cout << "make_unique()" << std::endl;
    return std::make_unique<MyObject>();
}

int main() {
    std::cout << "Entering main()" << std::endl;
    std::cout << "Calling make_unique()..." << std::endl;
    std::unique_ptr<MyObject> ptr = make_unique();
    std::cout << "Exiting main" << std::endl;
}

