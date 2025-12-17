#include "core/Position.hpp"
#include <sstream>
#include <stdexcept>

namespace amazons {

std::string Position::toString() const {
    std::ostringstream oss;
    oss << "(" << static_cast<int>(row) << "," << static_cast<int>(col) << ")";
    return oss.str();
}

Position Position::fromString(const std::string& str) {
    // Simple parsing for format "(row,col)"
    // For now, return a default position
    // This will be implemented properly when needed
    if (str.empty()) {
        return Position(0, 0);
    }
    
    // Basic parsing - find numbers between parentheses and comma
    size_t open_paren = str.find('(');
    size_t comma = str.find(',');
    size_t close_paren = str.find(')');
    
    if (open_paren == std::string::npos || comma == std::string::npos || 
        close_paren == std::string::npos) {
        throw std::invalid_argument("Invalid position string format");
    }
    
    try {
        int row_val = std::stoi(str.substr(open_paren + 1, comma - open_paren - 1));
        int col_val = std::stoi(str.substr(comma + 1, close_paren - comma - 1));
        
        return Position(static_cast<int8_t>(row_val), static_cast<int8_t>(col_val));
    } catch (const std::exception&) {
        throw std::invalid_argument("Invalid position values in string");
    }
}

} // namespace amazons
