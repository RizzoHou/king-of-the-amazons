#include "ui/TextDisplay.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

namespace amazons {

std::string TextDisplay::cellToString(Board::Cell cell) {
    switch (cell) {
        case Board::Cell::EMPTY: return ".";
        case Board::Cell::ARROW: return "X";
        case Board::Cell::WHITE_AMAZON: return "W";
        case Board::Cell::BLACK_AMAZON: return "B";
        default: return "?";
    }
}

std::string TextDisplay::playerToString(Player player) {
    switch (player) {
        case Player::WHITE: return "White";
        case Player::BLACK: return "Black";
        default: return "Unknown";
    }
}

void TextDisplay::displayBoard(const Board& board) const {
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

void TextDisplay::displayGameState(const GameState& state) const {
    std::cout << "=== King of the Amazons ===\n";
    displayTurnNumber(state.getTurnNumber());
    displayCurrentPlayer(state.getCurrentPlayer());
    displayBoard(state.getBoard());
    
    if (state.isGameOver()) {
        displayWinner(state.getWinner());
    }
}

void TextDisplay::displayLegalMoves(const GameState& state, const Position& from) const {
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

void TextDisplay::displayWinner(Player winner) const {
    std::cout << "Game Over! " << playerToString(winner) << " wins!\n";
}

void TextDisplay::displayCurrentPlayer(Player player) const {
    std::cout << "Current player: " << playerToString(player) << "\n";
}

void TextDisplay::displayTurnNumber(int turn) const {
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
            oss << " " << cellToString(board.getCell(row, col)) << " ";
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
    oss << "Current player: " << playerToString(state.getCurrentPlayer()) << "\n";
    oss << boardToString(state.getBoard());
    
    if (state.isGameOver()) {
        oss << "Game Over! " << playerToString(state.getWinner()) << " wins!\n";
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

} // namespace amazons
