#include "ui/MenuController.hpp"
#include "ui/TextDisplay.hpp"
#ifdef WITH_GRAPHICAL_GUI
#include "ui/GraphicalController.hpp"
#endif
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    try {
        // Check command line arguments for mode preference
        enum class RunMode { GRAPHICAL, TEXT };
        RunMode runMode = RunMode::GRAPHICAL; // Default to graphical
        
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--graphical" || arg == "-g") {
                runMode = RunMode::GRAPHICAL;
            } else if (arg == "--text" || arg == "-t") {
                runMode = RunMode::TEXT;
            } else if (arg == "--help" || arg == "-h") {
                std::cout << "King of the Amazons\n";
                std::cout << "Usage: " << argv[0] << " [options]\n";
                std::cout << "Options:\n";
                std::cout << "  --graphical, -g    Use pure graphical interface (default)\n";
                std::cout << "  --text, -t         Use text interface\n";
                std::cout << "  --help, -h         Show this help message\n";
                return 0;
            }
        }
        
        if (runMode == RunMode::GRAPHICAL) {
#ifdef WITH_GRAPHICAL_GUI
            std::cout << "Starting pure graphical interface...\n";
            amazons::GraphicalController controller;
            controller.run();
            return 0;
#else
            std::cout << "Graphical interface not available. Falling back to text interface.\n";
            runMode = RunMode::TEXT;
#endif
        }
        
        // Text mode
        if (runMode == RunMode::TEXT) {
            auto display = std::make_unique<amazons::TextDisplay>();
            amazons::MenuController controller(std::move(display));
            controller.run();
            return 0;
        }
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown fatal error" << std::endl;
        return 1;
    }
}
