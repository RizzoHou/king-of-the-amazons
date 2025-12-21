#pragma once

#include "core/GameState.hpp"
#include "core/Player.hpp" // For GameMode
#include "ui/Display.hpp"
#include <memory>

namespace amazons {

class MenuController {
public:
    MenuController();
    explicit MenuController(std::unique_ptr<Display> display);
    ~MenuController();
    
    void run();
    
private:
    std::unique_ptr<GameState> gameState;
    std::unique_ptr<Display> display;
    GameMode currentGameMode;
    
    // Factory method to create appropriate display
    static std::unique_ptr<Display> createDisplay(bool useGraphical = false);
    
    // Main menu
    void mainMenu();
    void newGame();
    void loadGame();
    void saveGame();
    void exitGame();
    
    // Game loop
    void gameLoop();
    bool playerTurn(); // Returns true if game should continue, false if user wants to exit
    bool makePlayerMove(); // Returns true if move was made, false if user wants to exit
    
    // Helper methods
    void showGameStatus() const;
    bool confirmAction(const std::string& message) const;
    void saveCurrentGame();
    
    // Game modes
    void simpleGameLoop(); // Simple human vs human game
    void humanVsAIGameLoop(); // Human vs AI game
    void aiVsAiGameLoop(); // AI vs AI game
    
    // Game mode aware loading
    void loadAndRunGame(const std::string& filename);
    
    // AI helper methods
    void makeAIMove(); // Make a move using AI
};

} // namespace amazons
