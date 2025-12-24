#include "core/GameState.hpp"
#include <stdexcept>
#include <algorithm>

namespace amazons {

namespace {
    // Helper function to check if path is clear for arrow shot, treating vacated square as empty
    bool isArrowPathClearWithVacated(const Board& board, 
                                     const Position& arrowFrom, 
                                     const Position& arrowTo,
                                     const Position& vacatedSquare) {
        // Check if arrow target is valid
        if (!board.isValidPosition(arrowTo)) {
            return false;
        }
        
        // Arrow target must be empty or be the vacated square (which becomes empty)
        if (board.getCell(arrowTo) != Board::Cell::EMPTY && 
            !(arrowTo.row == vacatedSquare.row && arrowTo.col == vacatedSquare.col)) {
            return false;
        }
        
        // Check if path is clear for queen move (straight or diagonal)
        int dr = (arrowTo.row > arrowFrom.row) ? 1 : (arrowTo.row < arrowFrom.row) ? -1 : 0;
        int dc = (arrowTo.col > arrowFrom.col) ? 1 : (arrowTo.col < arrowFrom.col) ? -1 : 0;
        
        // If arrowFrom == arrowTo (zero-length move), path is trivially clear
        if (dr == 0 && dc == 0) {
            return true;
        }
        
        // Start from the first cell after 'arrowFrom'
        int r = arrowFrom.row + dr;
        int c = arrowFrom.col + dc;
        
        // Check all cells up to but not including 'arrowTo'
        while (r != arrowTo.row || c != arrowTo.col) {
            if (!board.isValidPosition(r, c)) {
                return false;
            }
            
            // Cell is not empty AND it's not the vacated square
            if (board.getCell(r, c) != Board::Cell::EMPTY && 
                !(r == vacatedSquare.row && c == vacatedSquare.col)) {
                return false;
            }
            
            r += dr;
            c += dc;
        }
        
        return true;
    }
    
    // Helper function to get all legal arrow positions from a position, treating vacated square as empty
    std::vector<Position> getLegalArrowPositions(const Board& board,
                                                 const Position& arrowFrom,
                                                 const Position& vacatedSquare) {
        std::vector<Position> positions;
        
        // 8 directions: up, down, left, right, and 4 diagonals
        const std::array<std::pair<int, int>, 8> DIRECTIONS = {{
            {-1, -1}, {-1, 0}, {-1, 1},
            {0, -1},           {0, 1},
            {1, -1},  {1, 0},  {1, 1}
        }};
        
        for (const auto& [dr, dc] : DIRECTIONS) {
            int r = arrowFrom.row + dr;
            int c = arrowFrom.col + dc;
            
            while (board.isValidPosition(r, c)) {
                // Check if cell is empty or is the vacated square
                if (board.getCell(r, c) == Board::Cell::EMPTY || 
                    (r == vacatedSquare.row && c == vacatedSquare.col)) {
                    positions.emplace_back(r, c);
                } else {
                    // Stop in this direction if we hit a non-empty cell that's not the vacated square
                    break;
                }
                
                r += dr;
                c += dc;
            }
        }
        
        return positions;
    }
}

GameState::GameState() : currentPlayer(Player::WHITE), turnNumber(1) {
    board.initializeStandardPosition();
}

GameState::GameState(const Board& board, Player currentPlayer, int turnNumber) 
    : board(board), currentPlayer(currentPlayer), turnNumber(turnNumber) {
    // Move history is not restored - this is a limitation
    // In a full implementation, we would also save/restore move history
}

void GameState::initializeStandardGame() {
    board.initializeStandardPosition();
    currentPlayer = Player::WHITE;
    turnNumber = 1;
    moveHistory.clear();
}

bool GameState::isGameOver() const {
    return !hasLegalMoves(currentPlayer);
}

Player GameState::getWinner() const {
    if (!isGameOver()) {
        throw std::logic_error("Game is not over");
    }
    
    // The player who cannot move loses
    return oppositePlayer(currentPlayer);
}

std::vector<Move> GameState::getLegalMoves() const {
    return getLegalMovesForPlayer(currentPlayer);
}

std::vector<Move> GameState::getLegalMovesForPlayer(Player player) const {
    std::vector<Move> legalMoves;
    
    Board::Cell playerCell = (player == Player::WHITE) ? 
        Board::Cell::WHITE_AMAZON : Board::Cell::BLACK_AMAZON;
    
    // Find all Amazons for this player
    for (int r = 0; r < Board::SIZE; ++r) {
        for (int c = 0; c < Board::SIZE; ++c) {
            if (board.getCell(r, c) == playerCell) {
                Position from(r, c);
                
                // Get all possible moves for this Amazon
                auto movePositions = board.getLegalMoves(from);
                
                // For each move position, get all possible arrow shots
                for (const auto& to : movePositions) {
                    // Get all legal arrow positions from new position, treating 'from' as vacated
                    auto arrowPositions = getLegalArrowPositions(board, to, from);
                    
                    // Add all legal arrow shots
                    for (const auto& arrow : arrowPositions) {
                        legalMoves.emplace_back(from, to, arrow);
                    }
                }
            }
        }
    }
    
    return legalMoves;
}

bool GameState::isValidMove(const Move& move) const {
    if (!move.isValid()) {
        return false;
    }
    
    // Check if it's the correct player's turn
    Board::Cell currentPlayerCell = (currentPlayer == Player::WHITE) ? 
        Board::Cell::WHITE_AMAZON : Board::Cell::BLACK_AMAZON;
    
    if (board.getCell(move.from) != currentPlayerCell) {
        return false;
    }
    
    // Check if move is legal (to position is reachable)
    auto legalMoves = board.getLegalMoves(move.from);
    if (std::find(legalMoves.begin(), legalMoves.end(), move.to) == legalMoves.end()) {
        return false;
    }
    
    // Check if arrow shot is legal from new position
    // Use helper that treats the vacated square (move.from) as empty
    if (!isArrowPathClearWithVacated(board, move.to, move.arrow, move.from)) {
        return false;
    }
    
    return true;
}

void GameState::makeMove(const Move& move) {
    if (!isValidMove(move)) {
        throw std::invalid_argument("Invalid move");
    }
    
    // Save move for undo
    moveHistory.push_back(move);
    
    // Get player's cell type
    Board::Cell playerCell = (currentPlayer == Player::WHITE) ? 
        Board::Cell::WHITE_AMAZON : Board::Cell::BLACK_AMAZON;
    
    // Move Amazon
    board.setCell(move.from, Board::Cell::EMPTY);
    board.setCell(move.to, playerCell);
    
    // Shoot arrow
    board.setCell(move.arrow, Board::Cell::ARROW);
    
    // Switch player and increment turn
    switchPlayer();
    if (currentPlayer == Player::WHITE) {
        turnNumber++;
    }
}

void GameState::undoLastMove() {
    if (moveHistory.empty()) {
        throw std::logic_error("No moves to undo");
    }
    
    Move lastMove = moveHistory.back();
    moveHistory.pop_back();
    
    // Reverse the move
    Board::Cell playerCell = (currentPlayer == Player::WHITE) ? 
        Board::Cell::BLACK_AMAZON : Board::Cell::WHITE_AMAZON;
    
    // Remove arrow
    board.setCell(lastMove.arrow, Board::Cell::EMPTY);
    
    // Move Amazon back
    board.setCell(lastMove.to, Board::Cell::EMPTY);
    board.setCell(lastMove.from, playerCell);
    
    // Switch player back and adjust turn number
    switchPlayer();
    if (currentPlayer == Player::BLACK) {
        turnNumber--;
    }
}

bool GameState::hasLegalMoves(Player player) const {
    // Simple check: if any legal moves exist for this player
    auto moves = getLegalMovesForPlayer(player);
    return !moves.empty();
}

void GameState::switchPlayer() {
    currentPlayer = oppositePlayer(currentPlayer);
}

void GameState::updateGameStatus() {
    // Game status is checked lazily via isGameOver()
    // No need to update anything here
}

bool GameState::operator==(const GameState& other) const {
    return board == other.board && 
           currentPlayer == other.currentPlayer && 
           turnNumber == other.turnNumber;
}

} // namespace amazons
