#pragma once

#include "core/GameState.hpp"
#include "ui/TextDisplay.hpp"
#include <memory>

namespace amazons {

class MenuController {
public:
    MenuController();
    
    void run();
    
private:
    std::unique_ptr<GameState> gameState;
    TextDisplay display;
    
    // Main menu
    void mainMenu();
    void newGame();
    void loadGame();
    void saveGame();
    void exitGame();
    
    // Game loop
    void gameLoop();
    void playerTurn();
    void makePlayerMove();
    
    // Helper methods
    void showGameStatus() const;
    bool confirmAction(const std::string& message) const;
    
    // Game modes
    void simpleGameLoop(); // Simple human vs human game
    void humanVsAIGameLoop(); // Human vs AI game
    void aiVsAiGameLoop(); // AI vs AI game
    
    // AI helper methods
    void makeAIMove(); // Make a move using AI
};

} // namespace amazons
