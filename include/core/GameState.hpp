#pragma once

#include "core/Board.hpp"
#include "core/Player.hpp"
#include "core/Move.hpp"
#include <vector>
#include <memory>

namespace amazons {

class GameState {
public:
    GameState();
    GameState(const Board& board, Player currentPlayer, int turnNumber);
    
    void initializeStandardGame();
    
    Player getCurrentPlayer() const { return currentPlayer; }
    const Board& getBoard() const { return board; }
    int getTurnNumber() const { return turnNumber; }
    
    bool isGameOver() const;
    Player getWinner() const; // Returns Player::WHITE, Player::BLACK, or throws if game not over
    
    std::vector<Move> getLegalMoves() const;
    std::vector<Move> getLegalMovesForPlayer(Player player) const;
    
    bool isValidMove(const Move& move) const;
    void makeMove(const Move& move);
    
    // For undo functionality (to be implemented later)
    bool canUndo() const { return !moveHistory.empty(); }
    void undoLastMove();
    
    // For testing and debugging
    bool operator==(const GameState& other) const;
    bool operator!=(const GameState& other) const {
        return !(*this == other);
    }

private:
    Board board;
    Player currentPlayer;
    int turnNumber;
    std::vector<Move> moveHistory;
    
    // Helper methods
    bool hasLegalMoves(Player player) const;
    void switchPlayer();
    void updateGameStatus();
};

} // namespace amazons
