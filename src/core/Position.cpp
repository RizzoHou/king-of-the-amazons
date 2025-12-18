#include "core/Position.hpp"
#include <sstream>
#include <stdexcept>

namespace amazons {

std::string Position::toString() const {
    std::ostringstream oss;
    oss << static_cast<int>(row) << " " << static_cast<int>(col);
    return oss.str();
}

Position Position::fromString(const std::string& str) {
    if (str.empty()) {
        return Position(0, 0);
    }
    
    // Parse format: "row col"
    std::istringstream iss(str);
    int row_val, col_val;
    
    if (!(iss >> row_val >> col_val)) {
        throw std::invalid_argument("Invalid position string format");
    }
    
    // Check for extra characters
    char remaining;
    if (iss >> remaining) {
        throw std::invalid_argument("Invalid position string format - extra characters");
    }
    
    return Position(static_cast<int8_t>(row_val), static_cast<int8_t>(col_val));
}

} // namespace amazons
