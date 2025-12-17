#pragma once

#include "core/GameState.hpp"
#include "ui/TextDisplay.hpp"
#include "ui/InputHandler.hpp"
#include <memory>

namespace amazons {

class MenuController {
public:
    MenuController();
    
    void run();
    
private:
    std::unique_ptr<GameState> gameState;
    TextDisplay display;
    InputHandler inputHandler;
    
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
    
    // For Phase 1, we'll implement basic functionality
    void simpleGameLoop(); // Simple human vs human game
};

} // namespace amazons
