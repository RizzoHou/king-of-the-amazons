#include "core/Board.hpp"
#include "core/Player.hpp"
#include <algorithm>
#include <queue>
#include <array>

namespace amazons {

namespace {
    // Direction vectors for queen moves (8 directions)
    constexpr std::array<std::pair<int, int>, 8> DIRECTIONS = {{
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    }};
}

Board::Board() {
    // Initialize all cells to EMPTY
    for (auto& row : grid) {
        row.fill(Cell::EMPTY);
    }
}

void Board::initializeStandardPosition() {
    // Clear the board first
    for (auto& row : grid) {
        row.fill(Cell::EMPTY);
    }
    
    // White Amazons (bottom positions - rows 6-9)
    setCell(6, 0, Cell::WHITE_AMAZON);
    setCell(9, 3, Cell::WHITE_AMAZON);
    setCell(9, 6, Cell::WHITE_AMAZON);
    setCell(6, 9, Cell::WHITE_AMAZON);
    
    // Black Amazons (top positions - rows 0-3)
    setCell(0, 3, Cell::BLACK_AMAZON);
    setCell(0, 6, Cell::BLACK_AMAZON);
    setCell(3, 0, Cell::BLACK_AMAZON);
    setCell(3, 9, Cell::BLACK_AMAZON);
}

bool Board::isValidPosition(int row, int col) const {
    return row >= 0 && row < SIZE && col >= 0 && col < SIZE;
}

Board::Cell Board::getCell(int row, int col) const {
    if (!isValidPosition(row, col)) {
        return Cell::EMPTY; // Or throw an exception
    }
    return grid[row][col];
}

void Board::setCell(int row, int col, Cell value) {
    if (isValidPosition(row, col)) {
        grid[row][col] = value;
    }
}

bool Board::isPathClear(const Position& from, const Position& to) const {
    // Check if path is clear for queen move (straight or diagonal)
    int dr = (to.row > from.row) ? 1 : (to.row < from.row) ? -1 : 0;
    int dc = (to.col > from.col) ? 1 : (to.col < from.col) ? -1 : 0;
    
    int r = from.row + dr;
    int c = from.col + dc;
    
    while (r != to.row || c != to.col) {
        if (!isValidPosition(r, c) || getCell(r, c) != Cell::EMPTY) {
            return false;
        }
        r += dr;
        c += dc;
    }
    
    return true;
}

std::vector<Position> Board::getReachablePositionsInDirection(
    const Position& from, int dr, int dc) const {
    
    std::vector<Position> positions;
    int r = from.row + dr;
    int c = from.col + dc;
    
    while (isValidPosition(r, c) && getCell(r, c) == Cell::EMPTY) {
        positions.emplace_back(r, c);
        r += dr;
        c += dc;
    }
    
    return positions;
}

std::vector<Position> Board::getLegalMoves(const Position& from) const {
    std::vector<Position> moves;
    
    if (!isValidPosition(from) || getCell(from) == Cell::EMPTY) {
        return moves; // Empty vector for invalid or empty starting position
    }
    
    // Queen moves in all 8 directions
    for (const auto& [dr, dc] : DIRECTIONS) {
        auto directionMoves = getReachablePositionsInDirection(from, dr, dc);
        moves.insert(moves.end(), directionMoves.begin(), directionMoves.end());
    }
    
    return moves;
}

std::vector<Position> Board::getLegalShots(const Position& from) const {
    std::vector<Position> shots;
    
    if (!isValidPosition(from)) {
        return shots;
    }
    
    // Queen moves in all 8 directions (same as getLegalMoves but don't check if from has Amazon)
    for (const auto& [dr, dc] : DIRECTIONS) {
        auto directionShots = getReachablePositionsInDirection(from, dr, dc);
        shots.insert(shots.end(), directionShots.begin(), directionShots.end());
    }
    
    return shots;
}

int Board::countReachableSquares(Player player) const {
    Cell targetCell = (player == Player::WHITE) ? Cell::WHITE_AMAZON : Cell::BLACK_AMAZON;
    std::vector<std::vector<bool>> visited(SIZE, std::vector<bool>(SIZE, false));
    std::queue<Position> queue;
    int reachableCount = 0;
    
    // Find all Amazons for this player
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            if (getCell(r, c) == targetCell) {
                queue.emplace(r, c);
                visited[r][c] = true;
            }
        }
    }
    
    // BFS to find all reachable empty squares
    while (!queue.empty()) {
        Position current = queue.front();
        queue.pop();
        
        // Check all 8 directions
        for (const auto& [dr, dc] : DIRECTIONS) {
            int r = current.row + dr;
            int c = current.col + dc;
            
            while (isValidPosition(r, c) && getCell(r, c) == Cell::EMPTY && !visited[r][c]) {
                visited[r][c] = true;
                reachableCount++;
                // Continue in same direction
                r += dr;
                c += dc;
            }
        }
    }
    
    return reachableCount;
}

bool Board::operator==(const Board& other) const {
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            if (grid[r][c] != other.grid[r][c]) {
                return false;
            }
        }
    }
    return true;
}

} // namespace amazons
