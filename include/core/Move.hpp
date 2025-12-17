#pragma once

#include "core/Position.hpp"
#include <string>

namespace amazons {

struct Move {
    Position from;      // Starting position of Amazon
    Position to;        // Destination position for Amazon move
    Position arrow;     // Position where arrow is shot
    
    Move() = default;
    Move(Position f, Position t, Position a) : from(f), to(t), arrow(a) {}
    
    bool operator==(const Move& other) const {
        return from == other.from && to == other.to && arrow == other.arrow;
    }
    
    bool operator!=(const Move& other) const {
        return !(*this == other);
    }
    
    std::string toString() const;
    static Move fromString(const std::string& str);
    
    bool isValid() const {
        return from.isValid() && to.isValid() && arrow.isValid();
    }
};

} // namespace amazons
