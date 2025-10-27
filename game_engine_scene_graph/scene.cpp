//
// Created by scott on 10/26/25.
//

#include <iostream>
#include <memory>
#include <vector>
#include <string>

class GameObject : public std::enable_shared_from_this<GameObject> {
public:
    int id;
    std::string name;
    std::weak_ptr<GameObject> parent;
    std::vector<std::shared_ptr<GameObject>> children;
    static int created_count;
    static int destroyed_count;
    static int id_seq;

    explicit GameObject(std::string name_) : name(std::move(name_)) {
        std::cout << "Created GameObject: " << name << "\n";
        id = ++id_seq;
        created_count++;
    }

    ~GameObject() {
        std::cout << "Destroyed GameObject: " << name << "\n";
        destroyed_count++;
    }

    std::string obj_info() const;

    void add_child(std::shared_ptr<GameObject> child) {
        child->parent = shared_from_this();  // 🔧 you’ll need enable_shared_from_this
        children.push_back(std::move(child));
    }

    void print_hierarchy(int depth = 0) const {
        std::cout << std::string(depth * 2, ' ') << "- " << obj_info() << "\n";
        for (const auto& child : children) {
            child->print_hierarchy(depth + 1);
        }
    }

    std::string get_path() {
        std::string path;
        if (!parent.expired()) {
            path = parent.lock()->get_path();
        }
        path = (!path.empty()) ? path + "/" + obj_info() : "" + obj_info();
        return path;
    }

    static void print_metrics() {
        std::string metrics;
        metrics = "Total Objects Created " + std::to_string(created_count) + ", Total Objects Destroyed " + std::to_string(destroyed_count) + "\n";
        std::cout << metrics;
    }

    std::shared_ptr<GameObject> findByName(std::string name) {
        for (const auto& child : children) {
            if (child->name == name) {
                return child;
            }
            else if (auto childChild = child->findByName(name)) {
                return childChild;
            }
        }
        return nullptr;
    }

    std::shared_ptr<GameObject> findById(int id) {
        for (const auto& child : children) {
            if (child->id == id) {
                return child;
            }
            else if (auto childChild = child->findById(id)) {
                return childChild;
            }
        }
        return nullptr;
    }
};

std::string GameObject::obj_info() const {
    return name + ":" + std::to_string(id);
}

int GameObject::created_count = 0;
int GameObject::destroyed_count = 0;
int GameObject::id_seq = 0;

class Scene {
public:
    std::vector<std::shared_ptr<GameObject>> roots;

    void add_root(std::shared_ptr<GameObject> obj) {
        roots.push_back(std::move(obj));
    }

    void remove_root(std::shared_ptr<GameObject> victim) {
        roots.erase(
            std::remove_if(roots.begin(), roots.end(),
                [&](const std::shared_ptr<GameObject>& obj) {
                    return obj == victim;
                }),
                roots.end());
    }

    void print_scene() const {
        std::cout << "Scene Hierarchy:\n";
        for (const auto& obj : roots) {
            obj->print_hierarchy();
        }
    }

    std::shared_ptr<GameObject> findByName(std::string name) {
        for (const auto& obj : roots) {
            if (obj->name == name) {
                return obj;
            }
            auto child_obj = obj.get()->findByName(name);
            if (child_obj != nullptr) {
                return child_obj;
            }
        }
        return nullptr;
    }

    std::shared_ptr<GameObject> findById(int id) {
        for (const auto& obj : roots) {
            if (obj->id == id) {
                return obj;
            }
            auto child_obj = obj.get()->findById(id);
            if (child_obj != nullptr) {
                return child_obj;
            }
        }
        return nullptr;
    }

};

void run_scene() {
    auto scene = std::make_unique<Scene>();

    auto player = std::make_shared<GameObject>("Player");
    auto sword = std::make_shared<GameObject>("Sword");
    auto shield = std::make_shared<GameObject>("Shield");

    player->add_child(sword);
    player->add_child(shield);

    scene->add_root(player);
    scene->print_scene();

    std::cout << player->get_path() << "\n";
    std::cout << sword->get_path() << "\n";
    std::cout << shield->get_path() << "\n";

    std::cout << "Searching for Player" << std::endl;
    auto obj =  scene.get()->findByName("Player");
    if (obj != nullptr) {
        std::cout << "Found! " << obj->get_path() << "\n";
    } else {
        std::cout << "No object found!\n";
    }

    std::cout << "Searching for Sword" << std::endl;
    obj = scene.get()->findByName("Sword");
    if (obj != nullptr) {
        std::cout << "Found! " << obj->get_path() << "\n";
    } else {
        std::cout << "No object found!\n";
    }

    std::cout << "Searching for Object 3" << std::endl;
    obj = scene.get()->findById(3);
    if (obj != nullptr) {
        std::cout << "Found! " << obj->get_path() << "\n";
    } else {
        std::cout << "No object found!\n";
    }

    scene.get()->remove_root(obj);
    obj.reset();

    scene->print_scene();
}

int main() {

    run_scene();

    GameObject::print_metrics();

}
