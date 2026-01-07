#include "ui/MenuController.hpp"
#include "ui/TextDisplay.hpp"
#ifdef WITH_GRAPHICAL_GUI
#include "ui/GraphicalDisplay.hpp"
#endif
#include "utils/Serializer.hpp"
#include "ai/BasicAI.hpp"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <thread>
#include <chrono>

namespace amazons {

MenuController::MenuController() 
    : gameState(std::make_unique<GameState>()), 
      display(createDisplay()),
      currentGameMode(GameMode::HUMAN_VS_HUMAN) {}

MenuController::MenuController(std::unique_ptr<Display> display)
    : gameState(std::make_unique<GameState>()),
      display(std::move(display)),
      currentGameMode(GameMode::HUMAN_VS_HUMAN) {}

MenuController::~MenuController() = default;

std::unique_ptr<Display> MenuController::createDisplay(bool useGraphical) {
    if (useGraphical) {
#ifdef WITH_GRAPHICAL_GUI
        return std::make_unique<GraphicalDisplay>();
#else
        std::cout << "Warning: Graphical display requested but not available. Using text display.\n";
        return std::make_unique<TextDisplay>();
#endif
    } else {
        return std::make_unique<TextDisplay>();
    }
}

void MenuController::run() {
    mainMenu();
}

void MenuController::simpleGameLoop() {
    // Initialize new game
    gameState = std::make_unique<GameState>();
    gameState->initializeStandardGame();
    
    std::cout << "New game started. Black goes first.\n";
    
    // Main game loop
    while (!gameState->isGameOver()) {
        showGameStatus();
        if (!playerTurn()) {
            // User wants to exit to main menu
            return;
        }
    }
    
    // Game over
    showGameStatus();
    std::cout << "\nGame Over! ";
    
    try {
        Player winner = gameState->getWinner();
        std::cout << Display::playerToString(winner) << " wins!\n";
    } catch (const std::exception& e) {
        std::cout << "Error determining winner: " << e.what() << "\n";
    }
    
    std::cout << "\nPress Enter to return to main menu...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void MenuController::showGameStatus() const {
    if (!gameState || !display) return;
    
    display->showGameState(*gameState);
}

bool MenuController::playerTurn() {
    if (!gameState || !display || gameState->isGameOver()) return true;
    
    Player current = gameState->getCurrentPlayer();
    display->showMessage(Display::playerToString(current) + "'s turn.");
    
    return makePlayerMove();
}

bool MenuController::makePlayerMove() {
    if (!gameState || !display) return true;
    
    // Try to get move interactively (for graphical display)
    auto move = display->getMoveInteractively(*gameState);
    if (move.has_value()) {
        // Graphical move was successful
        gameState->makeMove(move.value());
        display->showMessage("Move made: " + move->toString());
        return true;
    }
    
    // Fall back to text input
    while (true) {
        try {
            std::cout << "Enter your move as 6 numbers: from_row from_col to_row to_col arrow_row arrow_col\n";
            std::cout << "Or enter 'help' to see legal moves, 'undo' to undo last move, 'save' to save game, or 'exit' to return to main menu: ";
            
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
                    display->showMessage("No legal moves available.");
                } else {
                    std::string message = "Legal moves:\n";
                    for (const auto& move : moves) {
                        message += "  " + move.toString() + "\n";
                    }
                    display->showMessage(message);
                }
                continue;
            }
            
            if (input == "undo" || input == "u") {
                if (gameState->canUndo()) {
                    // Check if we're in AI vs Human mode and it's human's turn
                    bool isHumanVsAI = (currentGameMode == GameMode::HUMAN_VS_AI_HUMAN_WHITE || 
                                       currentGameMode == GameMode::HUMAN_VS_AI_HUMAN_BLACK);
                    
                    if (isHumanVsAI) {
                        // Determine human color based on game mode
                        Player humanColor;
                        if (currentGameMode == GameMode::HUMAN_VS_AI_HUMAN_BLACK) {
                            humanColor = Player::BLACK;
                        } else {
                            humanColor = Player::WHITE;
                        }
                        
                        // Check if it's currently human's turn
                        if (gameState->getCurrentPlayer() == humanColor) {
                            // In AI vs Human mode during human's turn, 
                            // we want to undo both AI move and previous human move
                            if (gameState->canUndo()) {
                                gameState->undoLastMove(); // Undo AI move
                                display->showMessage("AI move undone.");
                                
                                // Check if we can undo again (human's move)
                                if (gameState->canUndo()) {
                                    gameState->undoLastMove(); // Undo human move
                                    display->showMessage("Human move undone.");
                                }
                            }
                        } else {
                            // It's AI's turn, just undo once (normal behavior)
                            gameState->undoLastMove();
                            display->showMessage("Last move undone.");
                        }
                    } else {
                        // Not AI vs Human mode, normal undo
                        gameState->undoLastMove();
                        display->showMessage("Last move undone.");
                    }
                    
                    showGameStatus();
                } else {
                    display->showMessage("No moves to undo.");
                }
                continue;
            }
            
            if (input == "save" || input == "s") {
                saveCurrentGame();
                continue;
            }
            
            if (input == "exit" || input == "quit" || input == "q") {
                if (confirmAction("Are you sure you want to exit to main menu? (y/n): ")) {
                    return false; // Signal to exit game
                }
                continue;
            }
            
            // Try to parse as move
            try {
                Move move = Move::fromString(input);
                if (gameState->isValidMove(move)) {
                    gameState->makeMove(move);
                    display->showMessage("Move made: " + move.toString());
                    return true; // Move successful
                } else {
                    display->showMessage("Invalid move: Move is not legal in current position.");
                }
            } catch (const std::exception& e) {
                display->showMessage("Invalid move format: " + std::string(e.what()));
                display->showMessage("Please use format: from_row from_col to_row to_col arrow_row arrow_col (6 numbers).");
            }
            
        } catch (const std::exception& e) {
            display->showMessage("Error: " + std::string(e.what()));
            display->showMessage("Please try again.");
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
        currentGameMode = GameMode::HUMAN_VS_HUMAN;
        std::cout << "New Human vs Human game started. Black goes first.\n";
        gameLoop();
    } else if (input == "2") {
        // Human vs AI - need to choose side
        std::cout << "\n=== Choose Your Side ===\n";
        std::cout << "1. Play as Black (move first)\n";
        std::cout << "2. Play as White (move second)\n";
        std::cout << "3. Back to Game Mode Selection\n";
        std::cout << "\nSelect your side (1-3): ";
        
        std::string sideInput;
        std::getline(std::cin, sideInput);
        
        if (sideInput == "1") {
            // Human as Black, AI as White
            gameState = std::make_unique<GameState>();
            gameState->initializeStandardGame();
            currentGameMode = GameMode::HUMAN_VS_AI_HUMAN_BLACK;
            std::cout << "New Human vs AI game started. You are Black and move first.\n";
            std::cout << "AI will play as White.\n";
            humanVsAIGameLoop();
        } else if (sideInput == "2") {
            // Human as White, AI as Black
            gameState = std::make_unique<GameState>();
            gameState->initializeStandardGame();
            currentGameMode = GameMode::HUMAN_VS_AI_HUMAN_WHITE;
            std::cout << "New Human vs AI game started. You are White and move second.\n";
            std::cout << "AI will play as Black and move first.\n";
            humanVsAIGameLoop();
        } else if (sideInput == "3") {
            // Back to game mode selection
            newGame();
        } else {
            std::cout << "Invalid option. Returning to game mode selection.\n";
            newGame();
        }
    } else if (input == "3") {
        // AI vs AI
        gameState = std::make_unique<GameState>();
        gameState->initializeStandardGame();
        currentGameMode = GameMode::AI_VS_AI;
        std::cout << "New AI vs AI game started. Black AI goes first.\n";
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
        loadAndRunGame(selectedGame);
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
    
    if (serializer.saveGame(*gameState, currentGameMode, input)) {
        std::cout << "Game saved successfully as '" << input << "'.\n";
    } else {
        std::cout << "Failed to save game.\n";
    }
    
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void MenuController::saveCurrentGame() {
    std::cout << "\n=== Save Current Game ===\n";
    if (!gameState) {
        std::cout << "No game in progress to save.\n";
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
        return;
    }
    
    // Check if save already exists
    if (serializer.saveExists(input)) {
        std::cout << "A save with name '" << input << "' already exists.\n";
        if (!confirmAction("Overwrite? (y/n): ")) {
            std::cout << "Save cancelled.\n";
            return;
        }
    }
    
    if (serializer.saveGame(*gameState, currentGameMode, input)) {
        std::cout << "Game saved successfully as '" << input << "'.\n";
    } else {
        std::cout << "Failed to save game.\n";
    }
}

void MenuController::gameLoop() {
    if (!gameState) return;
    
    // Main game loop
    while (!gameState->isGameOver()) {
        showGameStatus();
        if (!playerTurn()) {
            // User wants to exit to main menu
            return;
        }
    }
    
    // Game over
    showGameStatus();
    std::cout << "\nGame Over! ";
    
    try {
        Player winner = gameState->getWinner();
        std::cout << Display::playerToString(winner) << " wins!\n";
    } catch (const std::exception& e) {
        std::cout << "Error determining winner: " << e.what() << "\n";
    }
    
    std::cout << "\nPress Enter to return to main menu...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void MenuController::humanVsAIGameLoop() {
    if (!gameState) return;
    
    BasicAI ai;
    
    // Determine human and AI colors based on game mode
    Player humanColor;
    if (currentGameMode == GameMode::HUMAN_VS_AI_HUMAN_BLACK) {
        humanColor = Player::BLACK;
    } else { // HUMAN_VS_AI_HUMAN_WHITE
        humanColor = Player::WHITE;
    }
    
    // Main game loop
    while (!gameState->isGameOver()) {
        showGameStatus();
        
        Player current = gameState->getCurrentPlayer();
        std::cout << Display::playerToString(current) << "'s turn.\n";
        
        if (current == humanColor) {
            // Human turn
            if (!makePlayerMove()) {
                // User wants to exit to main menu
                return;
            }
        } else {
            // AI turn
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
        std::cout << Display::playerToString(winner) << " wins!\n";
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
        std::cout << Display::playerToString(current) << " AI's turn.\n";
        
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
            std::cout << Display::playerToString(winner) << " wins!\n";
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

void MenuController::loadAndRunGame(const std::string& filename) {
    Serializer serializer;
    auto result = serializer.loadGameWithMode(filename);
    
    if (!result.first) {
        std::cout << "Failed to load game.\n";
        std::cout << "Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    
    gameState = std::move(result.first);
    currentGameMode = result.second;
    
    std::cout << "Game loaded successfully. Game mode: " << gameModeToString(currentGameMode) << "\n";
    
    // Run appropriate game loop based on saved game mode
    switch (currentGameMode) {
        case GameMode::HUMAN_VS_HUMAN:
            gameLoop();
            break;
        case GameMode::HUMAN_VS_AI_HUMAN_WHITE:
        case GameMode::HUMAN_VS_AI_HUMAN_BLACK:
            humanVsAIGameLoop();
            break;
        case GameMode::AI_VS_AI:
            aiVsAiGameLoop();
            break;
    }
}

} // namespace amazons
