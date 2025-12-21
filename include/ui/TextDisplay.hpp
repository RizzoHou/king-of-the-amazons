#pragma once

#include "ui/Display.hpp"
#include "core/Position.hpp"
#include "core/Move.hpp"
#include <string>
#include <vector>
#include <optional>

namespace amazons {

class TextDisplay : public Display {
public:
    TextDisplay() = default;
    ~TextDisplay() override = default;
    
    // Implement Display interface
    void showBoard(const Board& board) override;
    void showGameState(const GameState& state) override;
    void showLegalMoves(const GameState& state, const Position& from) override;
    void showWinner(Player winner) override;
    void showCurrentPlayer(Player player) override;
    void showTurnNumber(int turn) override;
    void showMessage(const std::string& message) override;
    
    void showMenu(const std::vector<std::string>& options) override;
    
    std::string getInput() override;
    std::optional<Position> getMouseClick() override;
    std::optional<Move> getMoveInteractively(const GameState& state) override;
    
    void clearScreen() override;
    void waitForContinue() override;
    
    std::string boardToString(const Board& board) const override;
    std::string gameStateToString(const GameState& state) const override;
    
private:
    void displayBoardWithCoordinates(const Board& board) const;
    std::string formatMoveList(const std::vector<Move>& moves) const;
    
    // Keep original method names for compatibility with existing code
    void displayBoard(const Board& board) { showBoard(board); }
    void displayGameState(const GameState& state) { showGameState(state); }
    void displayLegalMoves(const GameState& state, const Position& from) { showLegalMoves(state, from); }
    void displayWinner(Player winner) { showWinner(winner); }
    void displayCurrentPlayer(Player player) { showCurrentPlayer(player); }
    void displayTurnNumber(int turn) { showTurnNumber(turn); }
};

} // namespace amazons
