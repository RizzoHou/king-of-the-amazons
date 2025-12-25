#pragma once

#include "core/GameState.hpp"
#include "core/Move.hpp"
#include <string>
#include <memory>
#include <vector>

namespace amazons {

class BotProcess; // Forward declaration

class BotzoneAI {
public:
    // Constructor with optional bot path
    BotzoneAI(const std::string& botPath = "");
    
    // Destructor
    ~BotzoneAI();
    
    // Get the best move for the given game state
    Move getBestMove(const GameState& gameState);
    
    // Set the bot executable path
    void setBotPath(const std::string& botPath);
    
    // Get the bot executable path
    std::string getBotPath() const;
    
    // Check if bot is available
    bool isBotAvailable() const;
    
private:
    // Convert game state to Botzone protocol input
    std::string convertGameStateToBotzoneInput(const GameState& gameState) const;
    
    // Convert Botzone protocol output to Move
    Move convertBotzoneOutputToMove(const std::string& output) const;
    
    // Get move history from game state
    std::vector<std::string> getMoveHistory(const GameState& gameState) const;
    
    // Bot executable path
    std::string botPath;
    
    // Bot process manager
    std::unique_ptr<BotProcess> botProcess;
    
    // Flag to track if we're in keep-running mode
    bool keepRunningMode;
    
    // Move history for Botzone protocol
    std::vector<std::string> moveHistory;
};

} // namespace amazons