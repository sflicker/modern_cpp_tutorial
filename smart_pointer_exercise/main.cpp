//
// Created by scott on 10/25/25.
//

/** Smart Pointer Exercise (from chatgpt)
 * You're building a basic image processing system. Each image is represented by a class Image that holds a name and a brightness value.

 * Your task is to:

 * Create a function that prints image info (read-only).
 * Create a function that adjusts brightness (modifies the image).
 * Create a function that loads an image and returns it as a unique_ptr.

 * Use all of this in main() — passing by const&, by &, and returning unique_ptr.

*/

#include <iostream>
#include <memory>
#include <string>

class Image {
    public:
        std::string name;
        int brightness;

        Image(std::string name_) : name(std::move(name_)) {}
};

std::unique_ptr<Image> load_image(std::string name, int brightness) {
    auto ptr = std::make_unique<Image>(name);
    ptr->brightness = brightness;
    return ptr;
}

void print_image(const Image& img) {
    std::cout << "Printing Image - " << img.name << " with brightness - " << img.brightness << std::endl;
}

void adjust_brightness(Image& img, int new_brightness) {
    std::cout << "Adjusting image brightness of " << img.name
              << " from " << img.brightness
              << " to " << new_brightness << std::endl;

    img.brightness = new_brightness;
}

int main() {
    auto image = load_image("test.png", 1);
    print_image(*image);
    adjust_brightness(*image, 10);
    print_image(*image);
}