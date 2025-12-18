#include "core/GameState.hpp"
#include <stdexcept>
#include <algorithm>

namespace amazons {

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
                    // Temporarily move the Amazon to get legal shots from new position
                    // For now, we'll use a simplified approach
                    auto shotPositions = board.getLegalShots(to);
                    
                    for (const auto& arrow : shotPositions) {
                        // Arrow cannot be shot to the from position (where Amazon was)
                        if (arrow != from) {
                            legalMoves.emplace_back(from, to, arrow);
                        }
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
    // For now, use simplified check
    auto legalShots = board.getLegalShots(move.to);
    if (std::find(legalShots.begin(), legalShots.end(), move.arrow) == legalShots.end()) {
        return false;
    }
    
    // Arrow cannot be shot to the from position
    if (move.arrow == move.from) {
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
