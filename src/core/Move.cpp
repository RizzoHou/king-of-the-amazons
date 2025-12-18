#include "core/Move.hpp"
#include <sstream>
#include <stdexcept>

namespace amazons {

std::string Move::toString() const {
    std::ostringstream oss;
    oss << from.toString() << " " << to.toString() << " " << arrow.toString();
    return oss.str();
}

Move Move::fromString(const std::string& str) {
    // Format: "from_row from_col to_row to_col arrow_row arrow_col"
    std::istringstream iss(str);
    int from_row, from_col, to_row, to_col, arrow_row, arrow_col;
    
    if (!(iss >> from_row >> from_col >> to_row >> to_col >> arrow_row >> arrow_col)) {
        throw std::invalid_argument("Invalid move string format: expected 6 numbers");
    }
    
    // Check for extra characters
    char remaining;
    if (iss >> remaining) {
        throw std::invalid_argument("Invalid move string format - extra characters");
    }
    
    try {
        Position from(static_cast<int8_t>(from_row), static_cast<int8_t>(from_col));
        Position to(static_cast<int8_t>(to_row), static_cast<int8_t>(to_col));
        Position arrowPos(static_cast<int8_t>(arrow_row), static_cast<int8_t>(arrow_col));
        
        return Move(from, to, arrowPos);
    } catch (const std::exception& e) {
        throw std::invalid_argument(std::string("Invalid position values: ") + e.what());
    }
}

} // namespace amazons
