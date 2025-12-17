#include "ui/MenuController.hpp"
#include <iostream>

int main() {
    try {
        amazons::MenuController controller;
        controller.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown fatal error" << std::endl;
        return 1;
    }
}
