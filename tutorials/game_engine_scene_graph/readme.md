🧩 Exercise: Game Engine Scene Graph

You’re building a simplified scene graph for a game engine. Each GameObject:

Has a unique name

Can have one parent

Can have many children

Lives in a Scene, which holds the top-level objects

✅ Goals:

Use std::shared_ptr<GameObject> to share ownership between scene and children.

Use std::weak_ptr<GameObject> for the parent pointer to avoid cyclic references.

Use std::unique_ptr<Scene> in main() to manage scene lifetime.

🔧 Your Tasks

Implement a class GameObject:

Fields:

std::string name

std::weak_ptr<GameObject> parent

std::vector<std::shared_ptr<GameObject>> children

Methods:

void add_child(std::shared_ptr<GameObject> child)

void print_hierarchy(int depth = 0) const

Implement a class Scene:

Holds a std::vector<std::shared_ptr<GameObject>> roots

Has a method to add_root(std::shared_ptr<GameObject>)

Has a method to print_scene()

In main():

Create a std::unique_ptr<Scene>

Create some GameObjects

Build a hierarchy (e.g., "Player" → "Weapon", "Player" → "Shield")

Print the scene hierarchy

🧱 Starter Skeleton
#include <iostream>
#include <memory>
#include <vector>
#include <string>

class GameObject {
public:
std::string name;
std::weak_ptr<GameObject> parent;
std::vector<std::shared_ptr<GameObject>> children;

    explicit GameObject(std::string name_) : name(std::move(name_)) {
        std::cout << "Created GameObject: " << name << "\n";
    }

    ~GameObject() {
        std::cout << "Destroyed GameObject: " << name << "\n";
    }

    void add_child(std::shared_ptr<GameObject> child) {
        child->parent = shared_from_this();  // 🔧 you’ll need enable_shared_from_this
        children.push_back(std::move(child));
    }

    void print_hierarchy(int depth = 0) const {
        std::cout << std::string(depth * 2, ' ') << "- " << name << "\n";
        for (const auto& child : children) {
            child->print_hierarchy(depth + 1);
        }
    }
};

class Scene {
public:
std::vector<std::shared_ptr<GameObject>> roots;

    void add_root(std::shared_ptr<GameObject> obj) {
        roots.push_back(std::move(obj));
    }

    void print_scene() const {
        std::cout << "Scene Hierarchy:\n";
        for (const auto& obj : roots) {
            obj->print_hierarchy();
        }
    }
};

int main() {
auto scene = std::make_unique<Scene>();

    auto player = std::make_shared<GameObject>("Player");
    auto sword = std::make_shared<GameObject>("Sword");
    auto shield = std::make_shared<GameObject>("Shield");

    player->add_child(sword);
    player->add_child(shield);

    scene->add_root(player);
    scene->print_scene();
}