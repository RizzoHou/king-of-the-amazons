#pragma once

#include "core/Position.hpp"
#include <array>
#include <vector>
#include <cstdint>

namespace amazons {

// Forward declaration
enum class Player;

class Board {
public:
    enum class Cell {
        EMPTY,
        ARROW,
        WHITE_AMAZON,
        BLACK_AMAZON
    };

    static constexpr int SIZE = 10;

    Board();
    
    void initializeStandardPosition();
    
    bool isValidPosition(int row, int col) const;
    bool isValidPosition(const Position& pos) const {
        return isValidPosition(pos.row, pos.col);
    }
    
    Cell getCell(int row, int col) const;
    Cell getCell(const Position& pos) const {
        return getCell(pos.row, pos.col);
    }
    
    void setCell(int row, int col, Cell value);
    void setCell(const Position& pos, Cell value) {
        setCell(pos.row, pos.col, value);
    }
    
    std::vector<Position> getLegalMoves(const Position& from) const;
    std::vector<Position> getLegalShots(const Position& from) const;
    
    int countReachableSquares(Player player) const;
    
    // For testing and debugging
    bool operator==(const Board& other) const;
    bool operator!=(const Board& other) const {
        return !(*this == other);
    }

private:
    std::array<std::array<Cell, SIZE>, SIZE> grid;
    
    // Helper methods
    bool isPathClear(const Position& from, const Position& to) const;
    std::vector<Position> getReachablePositionsInDirection(
        const Position& from, int dr, int dc) const;
};

} // namespace amazons
