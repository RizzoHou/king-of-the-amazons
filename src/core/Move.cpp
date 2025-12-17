#include "core/Move.hpp"
#include <sstream>
#include <stdexcept>

namespace amazons {

std::string Move::toString() const {
    std::ostringstream oss;
    oss << from.toString() << "->" << to.toString() << "->" << arrow.toString();
    return oss.str();
}

Move Move::fromString(const std::string& str) {
    // Format: "(r1,c1)->(r2,c2)->(r3,c3)"
    size_t arrow1 = str.find("->");
    if (arrow1 == std::string::npos) {
        throw std::invalid_argument("Invalid move string format: missing first '->'");
    }
    
    size_t arrow2 = str.find("->", arrow1 + 2);
    if (arrow2 == std::string::npos) {
        throw std::invalid_argument("Invalid move string format: missing second '->'");
    }
    
    std::string fromStr = str.substr(0, arrow1);
    std::string toStr = str.substr(arrow1 + 2, arrow2 - arrow1 - 2);
    std::string arrowStr = str.substr(arrow2 + 2);
    
    // Check for empty strings
    if (fromStr.empty() || toStr.empty() || arrowStr.empty()) {
        throw std::invalid_argument("Invalid move string format: empty position");
    }
    
    try {
        Position from = Position::fromString(fromStr);
        Position to = Position::fromString(toStr);
        Position arrowPos = Position::fromString(arrowStr);
        
        return Move(from, to, arrowPos);
    } catch (const std::exception& e) {
        throw std::invalid_argument(std::string("Invalid position in move string: ") + e.what());
    }
}

} // namespace amazons
