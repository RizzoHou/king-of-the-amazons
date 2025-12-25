#pragma once

#include <string>
#include <cstdint>

namespace amazons {

// Forward declaration
class Board;

struct Position {
    int8_t row{0};
    int8_t col{0};

    Position() = default;
    Position(int8_t r, int8_t c) : row(r), col(c) {}

    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }

    bool operator!=(const Position& other) const {
        return !(*this == other);
    }

    std::string toString() const;
    static Position fromString(const std::string& str);

    bool isValid() const;
};

} // namespace amazons