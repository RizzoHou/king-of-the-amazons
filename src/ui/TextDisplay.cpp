#include "ui/TextDisplay.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>

namespace amazons {

// Note: cellToString and playerToString are now static methods in Display class
// We'll use Display::cellToString() and Display::playerToString() instead

void TextDisplay::showBoard(const Board& board) {
    displayBoardWithCoordinates(board);
}

void TextDisplay::displayBoardWithCoordinates(const Board& board) const {
    std::cout << "\n  ";
    for (int col = 0; col < Board::SIZE; ++col) {
        std::cout << " " << col << " ";
    }
    std::cout << "\n";
    
    for (int row = 0; row < Board::SIZE; ++row) {
        std::cout << row << " ";
        for (int col = 0; col < Board::SIZE; ++col) {
            std::cout << " " << cellToString(board.getCell(row, col)) << " ";
        }
        std::cout << " " << row << "\n";
    }
    
    std::cout << "  ";
    for (int col = 0; col < Board::SIZE; ++col) {
        std::cout << " " << col << " ";
    }
    std::cout << "\n\n";
}

void TextDisplay::showGameState(const GameState& state) {
    std::cout << "=== King of the Amazons ===\n";
    showTurnNumber(state.getTurnNumber());
    showCurrentPlayer(state.getCurrentPlayer());
    showBoard(state.getBoard());
    
    if (state.isGameOver()) {
        showWinner(state.getWinner());
    }
}

void TextDisplay::showLegalMoves(const GameState& state, const Position& from) {
    auto moves = state.getLegalMoves();
    std::vector<Move> movesFromPosition;
    
    for (const auto& move : moves) {
        if (move.from == from) {
            movesFromPosition.push_back(move);
        }
    }
    
    if (movesFromPosition.empty()) {
        std::cout << "No legal moves from position " << from.toString() << "\n";
    } else {
        std::cout << "Legal moves from " << from.toString() << ":\n";
        for (const auto& move : movesFromPosition) {
            std::cout << "  " << move.toString() << "\n";
        }
    }
}

void TextDisplay::showWinner(Player winner) {
    std::cout << "Game Over! " << Display::playerToString(winner) << " wins!\n";
}

void TextDisplay::showCurrentPlayer(Player player) {
    std::cout << "Current player: " << Display::playerToString(player) << "\n";
}

void TextDisplay::showTurnNumber(int turn) {
    std::cout << "Turn: " << turn << "\n";
}

std::string TextDisplay::boardToString(const Board& board) const {
    std::ostringstream oss;
    
    oss << "  ";
    for (int col = 0; col < Board::SIZE; ++col) {
        oss << " " << col << " ";
    }
    oss << "\n";
    
    for (int row = 0; row < Board::SIZE; ++row) {
        oss << row << " ";
        for (int col = 0; col < Board::SIZE; ++col) {
            oss << " " << Display::cellToString(board.getCell(row, col)) << " ";
        }
        oss << " " << row << "\n";
    }
    
    oss << "  ";
    for (int col = 0; col < Board::SIZE; ++col) {
        oss << " " << col << " ";
    }
    oss << "\n";
    
    return oss.str();
}

std::string TextDisplay::gameStateToString(const GameState& state) const {
    std::ostringstream oss;
    
    oss << "Turn: " << state.getTurnNumber() << "\n";
    oss << "Current player: " << Display::playerToString(state.getCurrentPlayer()) << "\n";
    oss << boardToString(state.getBoard());
    
    if (state.isGameOver()) {
        oss << "Game Over! " << Display::playerToString(state.getWinner()) << " wins!\n";
    }
    
    return oss.str();
}

std::string TextDisplay::formatMoveList(const std::vector<Move>& moves) const {
    std::ostringstream oss;
    
    for (size_t i = 0; i < moves.size(); ++i) {
        oss << moves[i].toString();
        if (i != moves.size() - 1) {
            oss << ", ";
        }
        if (i > 0 && i % 3 == 0) {
            oss << "\n";
        }
    }
    
    return oss.str();
}

// New methods for Display interface
void TextDisplay::showMessage(const std::string& message) {
    std::cout << message << "\n";
}

void TextDisplay::showMenu(const std::vector<std::string>& options) {
    std::cout << "\n=== Menu ===\n";
    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << i + 1 << ". " << options[i] << "\n";
    }
    std::cout << "Select an option: ";
}

std::string TextDisplay::getInput() {
    std::string input;
    std::getline(std::cin, input);
    return input;
}

std::optional<Position> TextDisplay::getMouseClick() {
    // Text display doesn't support mouse clicks
    // Return empty optional to indicate no mouse input available
    return {};
}

std::optional<Move> TextDisplay::getMoveInteractively(const GameState& state) {
    // Text display falls back to text input
    // Just return empty optional to indicate text input should be used
    (void)state; // Mark parameter as unused to avoid warning
    return {};
}

void TextDisplay::clearScreen() {
    // Simple clear screen for cross-platform
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void TextDisplay::waitForContinue() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

} // namespace amazons
