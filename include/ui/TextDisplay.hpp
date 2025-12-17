#pragma once

#include "core/Board.hpp"
#include "core/GameState.hpp"
#include "core/Player.hpp"
#include <string>
#include <vector>

namespace amazons {

class TextDisplay {
public:
    TextDisplay() = default;
    
    void displayBoard(const Board& board) const;
    void displayGameState(const GameState& state) const;
    void displayLegalMoves(const GameState& state, const Position& from) const;
    void displayWinner(Player winner) const;
    void displayCurrentPlayer(Player player) const;
    void displayTurnNumber(int turn) const;
    
    std::string boardToString(const Board& board) const;
    std::string gameStateToString(const GameState& state) const;
    
    // Helper methods for formatting
    static std::string cellToString(Board::Cell cell);
    static std::string playerToString(Player player);
    
private:
    void displayBoardWithCoordinates(const Board& board) const;
    std::string formatMoveList(const std::vector<Move>& moves) const;
};

} // namespace amazons
