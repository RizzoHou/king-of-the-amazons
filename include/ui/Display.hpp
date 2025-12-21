#pragma once

#include "core/Board.hpp"
#include "core/GameState.hpp"
#include "core/Player.hpp"
#include "core/Move.hpp"
#include <string>
#include <vector>
#include <optional>

namespace amazons {

class Display {
public:
    virtual ~Display() = default;
    
    // Board and game state display
    virtual void showBoard(const Board& board) = 0;
    virtual void showGameState(const GameState& state) = 0;
    virtual void showLegalMoves(const GameState& state, const Position& from) = 0;
    virtual void showWinner(Player winner) = 0;
    virtual void showCurrentPlayer(Player player) = 0;
    virtual void showTurnNumber(int turn) = 0;
    virtual void showMessage(const std::string& message) = 0;
    
    // Menu display
    virtual void showMenu(const std::vector<std::string>& options) = 0;
    
    // Input methods
    virtual std::string getInput() = 0;  // For text mode
    virtual std::optional<Position> getMouseClick() = 0;  // For graphical mode
    virtual std::optional<Move> getMoveInteractively(const GameState& state) = 0;  // For graphical mode
    
    // Game flow control
    virtual void clearScreen() = 0;
    virtual void waitForContinue() = 0;
    
    // Helper methods
    virtual std::string boardToString(const Board& board) const = 0;
    virtual std::string gameStateToString(const GameState& state) const = 0;
    
    // Static helper methods (can be inherited)
    static std::string cellToString(Board::Cell cell) {
        switch (cell) {
            case Board::Cell::EMPTY: return ".";
            case Board::Cell::WHITE_AMAZON: return "W";
            case Board::Cell::BLACK_AMAZON: return "B";
            case Board::Cell::ARROW: return "X";
            default: return "?";
        }
    }
    
    static std::string playerToString(Player player) {
        switch (player) {
            case Player::WHITE: return "White";
            case Player::BLACK: return "Black";
            default: return "Unknown";
        }
    }
};

} // namespace amazons
