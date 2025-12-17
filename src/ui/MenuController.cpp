#include "ui/MenuController.hpp"
#include <iostream>
#include <limits>

namespace amazons {

MenuController::MenuController() : gameState(std::make_unique<GameState>()) {}

void MenuController::run() {
    std::cout << "=== King of the Amazons ===\n";
    std::cout << "Phase 1: Basic Human vs Human Game\n\n";
    
    simpleGameLoop();
}

void MenuController::simpleGameLoop() {
    // Initialize new game
    gameState = std::make_unique<GameState>();
    gameState->initializeStandardGame();
    
    std::cout << "New game started. White goes first.\n";
    
    // Main game loop
    while (!gameState->isGameOver()) {
        showGameStatus();
        playerTurn();
    }
    
    // Game over
    showGameStatus();
    std::cout << "\nGame Over! ";
    
    try {
        Player winner = gameState->getWinner();
        std::cout << display.playerToString(winner) << " wins!\n";
    } catch (const std::exception& e) {
        std::cout << "Error determining winner: " << e.what() << "\n";
    }
    
    std::cout << "\nPress Enter to return to main menu...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void MenuController::showGameStatus() const {
    if (!gameState) return;
    
    display.displayGameState(*gameState);
}

void MenuController::playerTurn() {
    if (!gameState || gameState->isGameOver()) return;
    
    Player current = gameState->getCurrentPlayer();
    std::cout << display.playerToString(current) << "'s turn.\n";
    
    makePlayerMove();
}

void MenuController::makePlayerMove() {
    if (!gameState) return;
    
    while (true) {
        try {
            std::cout << "Enter your move in format (r1,c1)->(r2,c2)->(r3,c3)\n";
            std::cout << "Or enter 'help' to see legal moves, or 'undo' to undo last move: ";
            
            std::string input;
            std::getline(std::cin, input);
            
            // Trim whitespace
            size_t start = input.find_first_not_of(" \t\n\r");
            if (start == std::string::npos) {
                std::cout << "Empty input. Please try again.\n";
                continue;
            }
            size_t end = input.find_last_not_of(" \t\n\r");
            input = input.substr(start, end - start + 1);
            
            if (input == "help" || input == "h") {
                // Show legal moves
                auto moves = gameState->getLegalMoves();
                if (moves.empty()) {
                    std::cout << "No legal moves available.\n";
                } else {
                    std::cout << "Legal moves:\n";
                    for (const auto& move : moves) {
                        std::cout << "  " << move.toString() << "\n";
                    }
                }
                continue;
            }
            
            if (input == "undo" || input == "u") {
                if (gameState->canUndo()) {
                    gameState->undoLastMove();
                    std::cout << "Last move undone.\n";
                    showGameStatus();
                } else {
                    std::cout << "No moves to undo.\n";
                }
                continue;
            }
            
            if (input == "exit" || input == "quit" || input == "q") {
                if (confirmAction("Are you sure you want to exit the game? (y/n): ")) {
                    exitGame();
                    return;
                }
                continue;
            }
            
            // Try to parse as move
            try {
                Move move = Move::fromString(input);
                gameState->makeMove(move);
                std::cout << "Move made: " << move.toString() << "\n";
                break; // Success
            } catch (const std::exception& e) {
                std::cout << "Invalid move: " << e.what() << "\n";
                std::cout << "Please try again.\n";
            }
            
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
            std::cout << "Please try again.\n";
        }
    }
}

bool MenuController::confirmAction(const std::string& message) const {
    std::cout << message;
    std::string input;
    std::getline(std::cin, input);
    
    return input == "y" || input == "Y" || input == "yes" || input == "YES";
}

void MenuController::exitGame() {
    std::cout << "Exiting game...\n";
    // For Phase 1, just exit the program
    std::exit(0);
}

} // namespace amazons
