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
        
        std::cout << "Invalid position. Please enter in format: row col (e.g., 3 5).\n";
    }
}

Move InputHandler::getMove(const std::string& prompt) const {
    while (true) {
        std::string input = getLine(prompt);
        auto move = parseMove(input);
        
        if (move && move->isValid()) {
            return *move;
        }
        
        std::cout << "Invalid move. Please enter 6 numbers: from_row from_col to_row to_col arrow_row arrow_col\n";
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
    // Check for format: "row col" - two numbers separated by space
    std::istringstream iss(str);
    int row, col;
    
    if (!(iss >> row >> col)) {
        return false;
    }
    
    // Check for extra characters
    char remaining;
    if (iss >> remaining) {
        return false;
    }
    
    return true;
}

bool InputHandler::isValidMoveString(const std::string& str) {
    // Check for format: "from_row from_col to_row to_col arrow_row arrow_col" - 6 numbers
    std::istringstream iss(str);
    int values[6];
    
    for (int i = 0; i < 6; ++i) {
        if (!(iss >> values[i])) {
            return false;
        }
    }
    
    // Check for extra characters
    char remaining;
    if (iss >> remaining) {
        return false;
    }
    
    return true;
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
