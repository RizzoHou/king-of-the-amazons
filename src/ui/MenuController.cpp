#include "ui/MenuController.hpp"
#include "utils/Serializer.hpp"
#include "ai/BasicAI.hpp"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <thread>
#include <chrono>

namespace amazons {

MenuController::MenuController() : gameState(std::make_unique<GameState>()) {}

void MenuController::run() {
    mainMenu();
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
                if (gameState->isValidMove(move)) {
                    gameState->makeMove(move);
                    std::cout << "Move made: " << move.toString() << "\n";
                    break; // Success
                } else {
                    std::cout << "Invalid move: Move is not legal in current position.\n";
                }
            } catch (const std::exception& e) {
                std::cout << "Invalid move format: " << e.what() << "\n";
                std::cout << "Please use format (r1,c1)->(r2,c2)->(r3,c3).\n";
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

void MenuController::mainMenu() {
    while (true) {
        std::cout << "\n=== King of the Amazons ===\n";
        std::cout << "Phase 2: Complete Game System\n";
        std::cout << "=============================\n";
        std::cout << "1. New Game\n";
        std::cout << "2. Load Game\n";
        std::cout << "3. Save Game\n";
        std::cout << "4. Exit\n";
        std::cout << "\nSelect an option (1-4): ";
        
        std::string input;
        std::getline(std::cin, input);
        
        if (input == "1") {
            newGame();
        } else if (input == "2") {
            loadGame();
        } else if (input == "3") {
            saveGame();
        } else if (input == "4") {
            if (confirmAction("Are you sure you want to exit? (y/n): ")) {
                exitGame();
                return;
            }
        } else {
            std::cout << "Invalid option. Please enter 1, 2, 3, or 4.\n";
        }
    }
}

void MenuController::newGame() {
    std::cout << "\n=== New Game ===\n";
    std::cout << "1. Human vs Human\n";
    std::cout << "2. Human vs AI\n";
    std::cout << "3. AI vs AI\n";
    std::cout << "4. Back to Main Menu\n";
    std::cout << "\nSelect game mode (1-4): ";
    
    std::string input;
    std::getline(std::cin, input);
    
    if (input == "1") {
        // Human vs Human
        gameState = std::make_unique<GameState>();
        gameState->initializeStandardGame();
        std::cout << "New Human vs Human game started. White goes first.\n";
        gameLoop();
    } else if (input == "2") {
        // Human vs AI
        gameState = std::make_unique<GameState>();
        gameState->initializeStandardGame();
        std::cout << "New Human vs AI game started. White (Human) goes first.\n";
        std::cout << "AI will play as Black.\n";
        humanVsAIGameLoop();
    } else if (input == "3") {
        // AI vs AI
        gameState = std::make_unique<GameState>();
        gameState->initializeStandardGame();
        std::cout << "New AI vs AI game started. White AI goes first.\n";
        aiVsAiGameLoop();
    } else if (input == "4") {
        // Back to main menu
        return;
    } else {
        std::cout << "Invalid option. Returning to main menu.\n";
    }
}

void MenuController::loadGame() {
    std::cout << "\n=== Load Game ===\n";
    
    Serializer serializer;
    auto savedGames = serializer.getSavedGames();
    
    if (savedGames.empty()) {
        std::cout << "No saved games found.\n";
        std::cout << "Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    
    std::cout << "Saved games:\n";
    for (size_t i = 0; i < savedGames.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << savedGames[i] << "\n";
    }
    std::cout << "  0. Back to Main Menu\n";
    std::cout << "\nSelect a game to load (0-" << savedGames.size() << "): ";
    
    std::string input;
    std::getline(std::cin, input);
    
    try {
        int choice = std::stoi(input);
        if (choice == 0) {
            return; // Back to main menu
        }
        
        if (choice < 1 || choice > static_cast<int>(savedGames.size())) {
            std::cout << "Invalid choice.\n";
            std::cout << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }
        
        std::string selectedGame = savedGames[choice - 1];
        auto loadedGame = serializer.loadGame(selectedGame);
        
        if (loadedGame) {
            gameState = std::move(loadedGame);
            std::cout << "Game loaded successfully.\n";
            gameLoop();
        } else {
            std::cout << "Failed to load game.\n";
            std::cout << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } catch (const std::exception& e) {
        std::cout << "Invalid input: " << e.what() << "\n";
        std::cout << "Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void MenuController::saveGame() {
    std::cout << "\n=== Save Game ===\n";
    if (!gameState) {
        std::cout << "No game in progress to save.\n";
        std::cout << "Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    
    Serializer serializer;
    
    // Show existing saves
    auto savedGames = serializer.getSavedGames();
    if (!savedGames.empty()) {
        std::cout << "Existing saved games:\n";
        for (const auto& game : savedGames) {
            std::cout << "  " << game << "\n";
        }
        std::cout << "\n";
    }
    
    std::cout << "Enter a name for your save (or press Enter to cancel): ";
    std::string input;
    std::getline(std::cin, input);
    
    if (input.empty()) {
        std::cout << "Save cancelled.\n";
        std::cout << "Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    
    // Check if save already exists
    if (serializer.saveExists(input)) {
        std::cout << "A save with name '" << input << "' already exists.\n";
        if (!confirmAction("Overwrite? (y/n): ")) {
            std::cout << "Save cancelled.\n";
            std::cout << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }
    }
    
    if (serializer.saveGame(*gameState, input)) {
        std::cout << "Game saved successfully as '" << input << "'.\n";
    } else {
        std::cout << "Failed to save game.\n";
    }
    
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void MenuController::gameLoop() {
    if (!gameState) return;
    
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

void MenuController::humanVsAIGameLoop() {
    if (!gameState) return;
    
    BasicAI ai;
    
    // Main game loop
    while (!gameState->isGameOver()) {
        showGameStatus();
        
        Player current = gameState->getCurrentPlayer();
        std::cout << display.playerToString(current) << "'s turn.\n";
        
        if (current == Player::WHITE) {
            // Human turn (White)
            makePlayerMove();
        } else {
            // AI turn (Black)
            std::cout << "AI is thinking...\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Small delay for realism
            
            try {
                Move aiMove = ai.getBestMove(*gameState);
                gameState->makeMove(aiMove);
                std::cout << "AI made move: " << aiMove.toString() << "\n";
            } catch (const std::exception& e) {
                std::cout << "AI error: " << e.what() << "\n";
                break;
            }
        }
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

void MenuController::aiVsAiGameLoop() {
    if (!gameState) return;
    
    BasicAI ai;
    int moveCount = 0;
    const int MAX_MOVES = 200; // Prevent infinite loops
    
    // Main game loop
    while (!gameState->isGameOver() && moveCount < MAX_MOVES) {
        showGameStatus();
        
        Player current = gameState->getCurrentPlayer();
        std::cout << display.playerToString(current) << " AI's turn.\n";
        
        std::cout << "AI is thinking...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Delay for visibility
        
        try {
            Move aiMove = ai.getBestMove(*gameState);
            gameState->makeMove(aiMove);
            std::cout << "AI made move: " << aiMove.toString() << "\n";
            moveCount++;
        } catch (const std::exception& e) {
            std::cout << "AI error: " << e.what() << "\n";
            break;
        }
    }
    
    // Game over
    showGameStatus();
    std::cout << "\n";
    
    if (moveCount >= MAX_MOVES) {
        std::cout << "Game stopped after " << MAX_MOVES << " moves (safety limit).\n";
    }
    
    if (gameState->isGameOver()) {
        std::cout << "Game Over! ";
        try {
            Player winner = gameState->getWinner();
            std::cout << display.playerToString(winner) << " wins!\n";
        } catch (const std::exception& e) {
            std::cout << "Error determining winner: " << e.what() << "\n";
        }
    }
    
    std::cout << "\nPress Enter to return to main menu...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void MenuController::makeAIMove() {
    if (!gameState) return;
    
    BasicAI ai;
    
    try {
        Move aiMove = ai.getBestMove(*gameState);
        gameState->makeMove(aiMove);
        std::cout << "AI made move: " << aiMove.toString() << "\n";
    } catch (const std::exception& e) {
        std::cout << "AI error: " << e.what() << "\n";
    }
}

} // namespace amazons
