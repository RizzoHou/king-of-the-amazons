#include "ui/InputHandler.hpp"
#include <iostream>
#include <sstream>
#include <limits>

namespace amazons {

std::optional<Position> InputHandler::parsePosition(const std::string& input) const {
    if (!isValidPositionString(input)) {
        return std::nullopt;
    }
    
    try {
        return Position::fromString(input);
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

std::optional<Move> InputHandler::parseMove(const std::string& input) const {
    if (!isValidMoveString(input)) {
        return std::nullopt;
    }
    
    try {
        return Move::fromString(input);
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

Position InputHandler::getPosition(const std::string& prompt) const {
    while (true) {
        std::string input = getLine(prompt);
        auto pos = parsePosition(input);
        
        if (pos && pos->isValid()) {
            return *pos;
        }
        
        std::cout << "Invalid position. Please enter in format (row,col) where row and col are 0-9.\n";
    }
}

Move InputHandler::getMove(const std::string& prompt) const {
    while (true) {
        std::string input = getLine(prompt);
        auto move = parseMove(input);
        
        if (move && move->isValid()) {
            return *move;
        }
        
        std::cout << "Invalid move. Please enter in format (r1,c1)->(r2,c2)->(r3,c3).\n";
    }
}

int InputHandler::getMenuChoice(const std::string& prompt, int min, int max) const {
    while (true) {
        std::string input = getLine(prompt);
        std::istringstream iss(input);
        int choice;
        
        if (iss >> choice && choice >= min && choice <= max) {
            return choice;
        }
        
        std::cout << "Invalid choice. Please enter a number between " << min << " and " << max << ".\n";
    }
}

bool InputHandler::getYesNo(const std::string& prompt) const {
    while (true) {
        std::string input = getLine(prompt + " (y/n): ");
        
        if (input == "y" || input == "Y" || input == "yes" || input == "YES") {
            return true;
        }
        if (input == "n" || input == "N" || input == "no" || input == "NO") {
            return false;
        }
        
        std::cout << "Please enter 'y' for yes or 'n' for no.\n";
    }
}

bool InputHandler::isValidPositionString(const std::string& str) {
    // Basic check: should contain parentheses and comma
    return str.find('(') != std::string::npos && 
           str.find(',') != std::string::npos && 
           str.find(')') != std::string::npos;
}

bool InputHandler::isValidMoveString(const std::string& str) {
    // Basic check: should contain two "->" separators
    size_t firstArrow = str.find("->");
    if (firstArrow == std::string::npos) {
        return false;
    }
    
    size_t secondArrow = str.find("->", firstArrow + 2);
    return secondArrow != std::string::npos;
}

std::string InputHandler::getLine(const std::string& prompt) const {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    
    // Trim whitespace
    size_t start = input.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) {
        return "";
    }
    
    size_t end = input.find_last_not_of(" \t\n\r");
    return input.substr(start, end - start + 1);
}

void InputHandler::clearInputBuffer() const {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

} // namespace amazons
